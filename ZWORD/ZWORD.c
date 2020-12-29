/******************************************************************************
file:       ZWORD
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2020, Robbert de Groot

description:
Entry point for the WORD program

WORD : W)ords O)rganized R)udimentary D)ocumentation
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "ZWORD.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static Gs *_chapterString = NULL;

/******************************************************************************
prototype:
******************************************************************************/
static Gs   *_ChapterGetString(           ParaChapter const chapter);

static Gs   *_ParaListGetTitle(           ParaArray const * const paraList);

static int   _Process(                    Gs const * const command, Gpath const * const path);

static Gb    _Write(                      WriteFunctions * const func, Gpath const * const path, ParaArray const * const paraList);
static void  _Write_ScopePopAll(          WriteFunctions * const func, Gfile * const file, Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType);
static void  _Write_ScopePopToOpenScope(  WriteFunctions * const func, Gfile * const file, Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType);
static void  _Write_ScopePopToListScope(  WriteFunctions * const func, Gfile * const file, Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType);
static void  _Write_ScopePopToTableRow(   WriteFunctions * const func, Gfile * const file, Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType);
static void  _Write_ScopePopTable(        WriteFunctions * const func, Gfile * const file, Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: main
******************************************************************************/
int main(int acount, char **alist)
{
   Gs    *command,
         *file,
         *filePaper;
   Gpath *path;
   Gi4    result;

   if (!grlStart())
   {
      return 1;
   }

   gconSetA(
      "Z:WORD\n"
      "========\n"
      "\n");

   // Print the help.
   if (acount <= 1)
   {
      gconSetA(
         "zword [command] [file] [paperFile]\n"
         "\n"
         "[command]  =\n"
         "   \"all\"        convert file to all the formats.\n"
         //"   \"confluence\" convert file to HTML format.\n"
         "   \"html\"       convert file to HTML format.\n"
         "   \"md\"         convert file to MD markup.\n"
         // future "   \"PDF\"  convert file to PDF format.\n"
         "   \"rtf\"        convert file to RTF format.  (Currently does not do tables.)\n"
         //"   \"zlyt\"       convert file to Z:LYT markup.\n"
         "\n"
         "[file]     =\n"
         "   Fully qualified file path.  Including .zword extension.\n"
         "\n"
         "[paperFile] =\n"
         "   Fully qualified file path.  Including extension.  File contains what the document\n"
         "   will look like on paper.");

      return 0;
   }

   // Get the command and the file.
   command   = gsCreateFromA(alist[1]);
   file      = gsCreateFromA(alist[2]);
   
   // Initialize a default paper.
   PaperStart();

   // Load in a paper if one is set.
   filePaper = NULL;
   if (acount == 4)
   {
      filePaper = gsCreateFromA(alist[3]);
      PaperLoad(filePaper);
   }

   // Convert the file string to a path.
   path = gpathCreateFrom(file);
   if (!gpathSetFromSystem(path))
   {
      gconSetA("ERROR: Path is mal formed.\n");
      return 1;
   }

   // Perform the conversion.
   result = _Process(command, path);

   if (result == 0)
   {
      gconSetA("File \"");
      gconSetS(file);
      gconSetA("\" was converted.\n");
   }
   else
   {
      gconSetA("ERROR: Problem occurred when attempting to process the file.\n");
   }

   // Clean up.
   gsDestroy(command);
   gsDestroy(file);
   gpathDestroy(path);

   grlStop();

   return result;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _ChapterGetString

Convert a chapter to a link string.
******************************************************************************/
static Gs *_ChapterGetString(ParaChapter const chapter)
{
   Gindex index;
   Gs    *str;

   genter;

   str = gsCreate();

   // For all chapters...
   forCount (index, 10)
   {
      breakIf(chapter.value[index] == 0);

      if (index)
      {
         gsAppendU2(str, L".");
      }
      gsAppendI(str, chapter.value[index]);
   }

   greturn str;
}

/******************************************************************************
func: _ParaListGetTitle
******************************************************************************/
static Gs *_ParaListGetTitle(ParaArray const * const paraList)
{
   Gindex index;
   Para  *para;

   genter;

   // Find the first major title.
   // For all paragraphs...
   forCount (index, paraArrayGetCount(paraList))
   {
      // Get the paragraph.
      para = paraArrayGetAt(paraList, index);

      if (para->type == paraTypeTITLE_1 ||
          para->type == paraTypeTITLE_TOC_1)
      {
         greturn gsCreateFrom(para->str);
      }
   }

   greturn NULL;
}

/******************************************************************************
func: _ParaListGetToc

Get all the titles that need to be in the table of contents.
******************************************************************************/
static ParaArray *_ParaListGetToc(ParaArray const * const paraList)
{
   Gindex     index;
   ParaArray *tocList;
   Para      *para;

   genter;

   // Create the toc list.
   tocList = paraArrayCreate();

   // For all paragraphs...
   forCount (index, paraArrayGetCount(paraList))
   {
      // Get the paragraph.
      para = paraArrayGetAt(paraList, index);

      if (paraTypeTITLE_TOC_1 <= para->type && para->type <= paraTypeTITLE_TOC_5)
      {
         paraArrayAdd(tocList, para);
      }
   }

   return tocList;
}

/******************************************************************************
func: _Process
******************************************************************************/
static int _Process(Gs const * const command, Gpath const * const path)
{
   Gi4             result;
   Gfile          *file;
   GsArray        *strList;
   Gcount          strListCount;
   Gindex          index,
                   cindex,
                   position;
   Gs             *stemp,
                  *markup;
   Para           *para;
   ParaArray      *paraList;
   ParaChapter     chapter;
   WriteFunctions  func;
   
   genter;

   // Nothing wrong
   result = 0;

   strList = gsArrayCreate(NULL, gbTRUE);

   // Open the file.
   file = gfileOpen(path, gfileOpenModeREAD_ONLY);
   if (!file)
   {
      return 1;
   }

   // Read in the entire file.
   strListCount = gfileGetContentSArray(file, gcTypeU1, strList);

   // Close the file.
   gfileClose(file);

   // Prep the chapter 
   forCount (index, 10)
   {
      chapter.value[index] = 0;
   }
   
   // Identify the paragraphs.
   paraList = paraArrayCreate();

   // For all strings...
   forCount (index, strListCount)
   {
      // Get the string 
      stemp = gsArrayGetAt(strList, index);

      // Find the first white space character.
      position = gsFindFirstOfU2(stemp, 0, WHITESPACE_U2);
      if (position == gsFIND_FAIL)
      {
         // Assume a regular paragraph.
         position = 0;
      }
      
      // We have a basic paragraph.
      if (position == 0)
      {
         stemp = gsTrimU2(stemp, WHITESPACE_U2);

         // Blank line, nothing to do.
         if (gsIsBlank(stemp))
         {
            continue;
         }
      }

      // Create a new paragraph.
      para = gmemCreateType(Para);

      // Regular paragraph.
      if (position == 0)
      {
         para->type        = paraTypeREGULAR;
         para->chapterStr  = _ChapterGetString(chapter);
         para->str         = stemp;
      }
      else
      {
         // We have a special paragraph
         markup = gsCreateFromSub(stemp, 0, position);
         gsEraseSub(stemp, 0, position);
         gsTrimU2(  stemp, WHITESPACE_U2);

         if      (gsIsEqualU2(markup, L"/") ||
                  gsIsEqualU2(markup, L"//"))
         {
            para->type = paraTypeCOMMENT;
            para->str  = stemp;
         }
         // Title Lines.
         else if (gsIsEqualU2(markup, L"h")     ||
                  gsIsEqualU2(markup, L"hh")    ||
                  gsIsEqualU2(markup, L"hhh")   ||
                  gsIsEqualU2(markup, L"hhhh")  ||
                  gsIsEqualU2(markup, L"hhhhh") ||
                  gsIsEqualU2(markup, L"H")     ||
                  gsIsEqualU2(markup, L"HH")    ||
                  gsIsEqualU2(markup, L"HHH")   ||
                  gsIsEqualU2(markup, L"HHHH")  ||
                  gsIsEqualU2(markup, L"HHHHH"))
         {
            if (*gsGetAt(markup, 0) == L'h') para->type = paraTypeTITLE_TOC_1;
            else                             para->type = paraTypeTITLE_1;

            para->type += gsGetCount(markup) - 1;

            if (*gsGetAt(markup, 0) == L'h')
            {
               // Increment the chapter.
               chapter.value[position - 1]++;
   
               // Reset the subchapter counts.
               for (cindex = position; cindex < 10; cindex++)
               {
                  chapter.value[cindex] = 0;
               }
            }

            para->str = stemp;
         }
         // List Lines.
         else if (gsIsEqualU2(markup, L"-"))
         {
            para->type = paraTypeITEM;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L":["))
         {
            para->type = paraTypeSCOPE_LIST_BULLET;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"=["))
         {
            para->type = paraTypeSCOPE_LIST_KEY_VALUE;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"#["))
         {
            para->type = paraTypeSCOPE_LIST_NUMBER;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"-["))
         {
            para->type = paraTypeSCOPE_ITEM;
            para->str  = stemp;
         }
         // Table Lines.
         else if (gsIsEqualU2(markup, L"t["))
         {
            para->type = paraTypeSCOPE_TABLE;
            para->str  = stemp;

         }
         else if (gsIsEqualU2(markup, L"t-"))
         {
            para->type = paraTypeTABLE_ROW;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"th"))
         {
            para->type = paraTypeTABLE_COLUMN_HEADER;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"th["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN_HEADER;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"thx"))
         {
            para->type = paraTypeTABLE_COLUMN_HEADER_NO_BREAK;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"thx["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"th*"))
         {
            para->type = paraTypeTABLE_COLUMN_HEADER_FILL;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"th*["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc"))
         {
            para->type = paraTypeTABLE_COLUMN;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tcx"))
         {
            para->type = paraTypeTABLE_COLUMN_NO_BREAK;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tcx["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN_NO_BREAK;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc*"))
         {
            para->type = paraTypeTABLE_COLUMN_FILL;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc*["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN_FILL;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc1"))
         {
            para->type = paraTypeTABLE_COLUMN_NUMBER;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc1["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN_NUMBER;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc1*"))
         {
            para->type = paraTypeTABLE_COLUMN_NUMBER_FILL;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc1*["))
         {
            para->type = paraTypeSCOPE_TABLE_COLUMN_NUMBER_FILL;
            para->str  = stemp;
         }
         // Formated Lines.
         else if (gsIsEqualU2(markup, L"."))
         {
            para->type = paraTypeFORMATED;
            para->str  = gsTrimU2(stemp, WHITESPACE_U2);
         }
         else if (gsIsEqualU2(markup, L".["))
         {
            para->type     = paraTypeSCOPE_FORMATED;

            // Special case.  No processing of the lines inside.
            // Keep appending strings until a terminator.
            for (index++; index < strListCount; index++)
            {
               stemp = gsArrayGetAt(strList, index);

               // Break; on terminator.
               breakIf(
                  gsGetCount(stemp)  != 1 &&
                  *gsGetAt(stemp, 0) == L']');

               if (para->str)
               {
                  gsAppendA(para->str, "\n");
                  gsAppend( para->str, gsTrimU2(stemp, WHITESPACE_U2));
               }
               else
               {
                  para->str = gsTrimU2(stemp, WHITESPACE_U2);
               }
            }
         }
         else if (gsIsEqualU2(markup, L"]"))
         {
            para->type = paraTypeSCOPE_STOP;
            para->str  = stemp;
         }
         // TOC
         else if (gsIsEqualU2(markup, L"toc"))
         {
            para->type = paraTypeTABLE_OF_CONTENTS;
            para->str  = stemp;
         }
         // page
         else if (gsIsEqualU2(markup, L"==="))
         {
            para->type = paraTypePAGE_BREAK;
            para->str  = stemp;
         }

         para->chapterStr = _ChapterGetString(chapter);

         // Clean up
         gsDestroy(markup);
      }

      // Add the paragraph to the list of paragraphs.
      paraArrayAdd(paraList, para);
   }

   // Send to the output.
   if (gsIsEqualU2(command, L"all") ||
       gsIsEqualU2(command, L"html"))
   {
      func = GetFunctionsHTML();
      _Write(&func, path, paraList);
   }
   if (gsIsEqualU2(command, L"all") ||
       gsIsEqualU2(command, L"md"))
   {
      func = GetFunctionsMD();
      _Write(&func, path, paraList);
   }
   if (gsIsEqualU2(command, L"all") ||
       gsIsEqualU2(command, L"rtf"))
   {
      func = GetFunctionsRTF();
      _Write(&func, path, paraList);
   }
#if 0
   if (gsIsEqualU2(command, L"all") ||
       gsIsEqualU2(command, L"ZLYT"))
   {
      func = GetFunctionsZLYT();
      _Write(&func, path, paraList);
   }
#endif

   // Clean up
   gsArrayForEach(strList, gsDestroyFunc);
   gsArrayDestroy(strList);

   forCount (index, paraArrayGetCount(paraList))
   {
      para = paraArrayGetAt(paraList, index);

      gsDestroy(para->chapterStr);
   }
   paraArrayForEach(paraList, gmemDestroy);
   paraArrayDestroy(paraList);

   greturn result;
}

/******************************************************************************
func: _Write
******************************************************************************/
static Gb _Write(WriteFunctions * const func, Gpath const * const path, ParaArray const * const paraList)
{
   Gfile          *file;
   Gindex          index;
   Para           *para;
   ParaType        scopeType[20];
   Gindex          scopeLevel,
                   indentLevel;
   Gb              isTableHeaderSeparatorWritten[20];
   ParaTypeArray  *tableHeaders[20];
   Gs             *stemp;
   ParaArray      *tocList;

   genter;

   scopeLevel  = -1;
   indentLevel = -1;
   
   forCount (index, 20)
   {
      scopeType[index]                     = paraTypeNONE;
      isTableHeaderSeparatorWritten[index] = gbFALSE;
      tableHeaders[index]                  = paraTypeArrayCreate();
   }
      
   // Open the file.
   file = func->FileOpen(path);
   greturnFalseIf(!file);

   // Write the header
   stemp = _ParaListGetTitle(paraList);
   func->FileWriteFormatHeader(file, stemp);
   gsDestroy(stemp);

   // Write out the contents of the document.
   // For all paragraphs...
   forCount (index, paraArrayGetCount(paraList))
   {
      // Get the paragraph.
      para = paraArrayGetAt(paraList, index);

      // Depending on the paragraph type...
      switch (para->type)
      {
      case paraTypeREGULAR:
         debugPrintMsg(L"paragraph>>>>>>>>>>>\n");
         _Write_ScopePopToOpenScope(func, file, &indentLevel, &scopeLevel, scopeType);

         func->FileWriteParagraph(file, para);
         break;

      case paraTypeCOMMENT:
         debugPrintMsg(L"comment>>>>>>>>>>>>>\n");
         // Comments should be allowed anywhere.  And they may not even be written out.
         func->FileWriteComment(file, para);
         break;

      case paraTypeTITLE_1:
      case paraTypeTITLE_2:
      case paraTypeTITLE_3:
      case paraTypeTITLE_4:
      case paraTypeTITLE_5:
      case paraTypeTITLE_TOC_1:
      case paraTypeTITLE_TOC_2:
      case paraTypeTITLE_TOC_3:
      case paraTypeTITLE_TOC_4:
      case paraTypeTITLE_TOC_5:

         switch (para->type)
         {
         case paraTypeTITLE_1:
         case paraTypeTITLE_2:
         case paraTypeTITLE_3:
         case paraTypeTITLE_4:
         case paraTypeTITLE_5:
            debugPrintMsg(L"title>>>>>>>>>>>>>>> %d\n", para->type - paraTypeTITLE_1 + 1);
            break;

         case paraTypeTITLE_TOC_1:
         case paraTypeTITLE_TOC_2:
         case paraTypeTITLE_TOC_3:
         case paraTypeTITLE_TOC_4:
         case paraTypeTITLE_TOC_5:
            debugPrintMsg(L"title>>>>>>>>>>>>>>> %d TOC\n", para->type - paraTypeTITLE_TOC_1 + 1);
            break;
         }

         // Titles will never appear in a list or table.  Force any open list or table to close closed.
         _Write_ScopePopAll(func, file, &indentLevel, &scopeLevel, scopeType);

         func->FileWriteTitle(file, para);
         break;

      case paraTypeSCOPE_ITEM:
      case paraTypeSCOPE_LIST_BULLET:
      case paraTypeSCOPE_LIST_KEY_VALUE:
      case paraTypeSCOPE_LIST_NUMBER:
      case paraTypeSCOPE_TABLE:
      case paraTypeSCOPE_TABLE_COLUMN_HEADER:
      case paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK:
      case paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL:
      case paraTypeSCOPE_TABLE_COLUMN:
      case paraTypeSCOPE_TABLE_COLUMN_NO_BREAK:
      case paraTypeSCOPE_TABLE_COLUMN_FILL:

         switch (para->type)
         {
         case paraTypeSCOPE_ITEM:
            debugPrintMsg(L"item>>>>>>>>>>>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_LIST_BULLET:
            debugPrintMsg(L"list bullet>>>>>>>>> %d\n", scopeLevel + 1);
            indentLevel++;
            break;

         case paraTypeSCOPE_LIST_KEY_VALUE:
            debugPrintMsg(L"list key value>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_LIST_NUMBER:
            debugPrintMsg(L"list number>>>>>>>>> %d\n", scopeLevel + 1);
            indentLevel++;
            break;

         case paraTypeSCOPE_TABLE:
            debugPrintMsg(L"table>>>>>>>>>>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_TABLE_COLUMN_HEADER:
            debugPrintMsg(L"table col h>>>>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK:
            debugPrintMsg(L"table col h nb>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL:
            debugPrintMsg(L"table col h f>>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_TABLE_COLUMN:
            debugPrintMsg(L"table col>>>>>>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_TABLE_COLUMN_NO_BREAK:
            debugPrintMsg(L"table col nb>>>>>>>> %d\n", scopeLevel + 1);
            break;

         case paraTypeSCOPE_TABLE_COLUMN_FILL:
            debugPrintMsg(L"table col f>>>>>>>>> %d\n", scopeLevel + 1);
            break;
         }

         _Write_ScopePopToOpenScope(func, file, &indentLevel, &scopeLevel, scopeType);

         scopeLevel++;
         greturnFalseIf(scopeLevel >= 20);

         scopeType[scopeLevel] = para->type;

         paraArrayFlush(tableHeaders[scopeLevel]);
         isTableHeaderSeparatorWritten[scopeLevel] = gbFALSE;

         if (para->type == paraTypeSCOPE_TABLE)
         {
            scopeLevel++;
            scopeType[scopeLevel] = paraTypeTABLE_ROW;
         }

         func->FileWriteScopeStart(
            file, 
            para->type, 
            scopeLevel, 
            (scopeLevel == 0) ?
               paraTypeNONE :
               scopeType[scopeLevel]);
         break;

      case paraTypeSCOPE_STOP:
         debugPrintMsg(L"scope pop>>>>>>>>>>> %d\n", scopeLevel);
         returnFalseIf(scopeLevel < 0);

         if (scopeType[scopeLevel] == paraTypeSCOPE_LIST_BULLET ||
             scopeType[scopeLevel] == paraTypeSCOPE_LIST_NUMBER)
         {
            indentLevel--;
         }

         func->FileWriteScopeStop(
            file, 
            scopeType[scopeLevel], 
            (scopeLevel > 0) ?
               scopeType[scopeLevel - 1] :
               paraTypeNONE);

         scopeLevel--;
         break;

      case paraTypeITEM:
         debugPrintMsg(L"item 1 line>>>>>>>>> %d\n", scopeLevel);
         _Write_ScopePopToListScope(func, file, &indentLevel, &scopeLevel, scopeType);

         returnFalseIf(
            !(scopeType[scopeLevel] == paraTypeSCOPE_LIST_BULLET    ||
              scopeType[scopeLevel] == paraTypeSCOPE_LIST_KEY_VALUE ||
              scopeType[scopeLevel] == paraTypeSCOPE_LIST_NUMBER));

         if (scopeType[scopeLevel] == paraTypeSCOPE_LIST_KEY_VALUE)
         {
            func->FileWriteKeyValue(file, para);
         }
         else
         {
            func->FileWriteScopeStart(file, para->type, indentLevel, scopeType[scopeLevel]);
            func->FileWriteString(    file, para);
            func->FileWriteScopeStop( file, para->type, scopeType[scopeLevel]);
         }
         break;

      case paraTypeTABLE_ROW:
         debugPrintMsg(L"table row>>>>>>>>>>> %d\n", scopeLevel);
         if (scopeType[scopeLevel] == paraTypeSCOPE_TABLE)
         {
            scopeType[scopeLevel++] = para->type;
         }
         else
         {
            _Write_ScopePopToTableRow(func, file, &indentLevel, &scopeLevel, scopeType);
            func->FileWriteScopeStop(file, para->type, paraTypeNONE);
         }

         if (!isTableHeaderSeparatorWritten[scopeLevel - 1])
         {
            func->FileWriteTableHeaderSeparator(file, tableHeaders[scopeLevel - 1]);
            isTableHeaderSeparatorWritten[scopeLevel - 1] = gbTRUE;
         }

         func->FileWriteScopeStart(file, para->type, scopeLevel, scopeType[scopeLevel]);
         break;

      case paraTypeTABLE_COLUMN_HEADER:
      case paraTypeTABLE_COLUMN_HEADER_NO_BREAK:
      case paraTypeTABLE_COLUMN_HEADER_FILL:
      case paraTypeTABLE_COLUMN:
      case paraTypeTABLE_COLUMN_NO_BREAK:
      case paraTypeTABLE_COLUMN_FILL:

         switch (para->type)
         {
         case paraTypeTABLE_COLUMN_HEADER:
            debugPrintMsg(L"table col h 1>>>>>>> %d\n", scopeLevel);
            break;

         case paraTypeTABLE_COLUMN_HEADER_NO_BREAK:
            debugPrintMsg(L"table col h nb 1>>>> %d\n", scopeLevel);
            break;

         case paraTypeTABLE_COLUMN_HEADER_FILL:
            debugPrintMsg(L"table col h f 1>>>>> %d\n", scopeLevel);
            break;

         case paraTypeTABLE_COLUMN:
            debugPrintMsg(L"table col 1>>>>>>>>> %d\n", scopeLevel);
            break;

         case paraTypeTABLE_COLUMN_NO_BREAK:
            debugPrintMsg(L"table col nb 1>>>>>> %d\n", scopeLevel);
            break;

         case paraTypeTABLE_COLUMN_FILL:
            debugPrintMsg(L"table col f 1>>>>>>> %d\n", scopeLevel);
            break;
         }
         
         if (!isTableHeaderSeparatorWritten[scopeLevel - 1])
         {
            paraTypeArrayAddEnd(tableHeaders[scopeLevel - 1], &para->type);
         }

         greturnFalseIf(scopeType[scopeLevel] != paraTypeTABLE_ROW);

         func->FileWriteScopeStart(file, para->type, scopeLevel, scopeType[scopeLevel]);
         func->FileWriteString(    file, para);
         func->FileWriteScopeStop( file, para->type, paraTypeNONE);
         break;

      case paraTypeFORMATED:
      case paraTypeSCOPE_FORMATED:
         switch (para->type)
         {
         case paraTypeFORMATED:
            debugPrintMsg(L"mono 1>>>>>>>>>>>>>> %d\n", scopeLevel);
            break;

         case paraTypeSCOPE_FORMATED:
            debugPrintMsg(L"mono>>>>>>>>>>>>>>>> %d\n", scopeLevel);
            break;
         }
         _Write_ScopePopToOpenScope(func, file, &indentLevel, &scopeLevel, scopeType);

         func->FileWriteScopeStart(     file, para->type, scopeLevel, scopeType[scopeLevel]);
         func->FileWriteStringUnaltered(file, para);
         func->FileWriteScopeStop(      file, para->type, paraTypeNONE);
         break;

      case paraTypeTABLE_OF_CONTENTS:
         debugPrintMsg(L"toc>>>>>>>>>>>>>>>>> %d\n", scopeLevel);
         _Write_ScopePopAll(func, file, &indentLevel, &scopeLevel, scopeType);

         // Get the table of contents.
         tocList = _ParaListGetToc(paraList);

         func->FileWriteTOC(file, tocList);

         // Clean up
         paraArrayDestroy(tocList);
         break;

      case paraTypePAGE_BREAK:
         debugPrintMsg(L"new page>>>>>>>>>>>> %d\n", scopeLevel);
         func->FileWritePageBreak(file);
         break;
      }
   }

   func->FileWriteFormatFooter(file);

   // Clean up.
   gfileClose(file);

   forCount (index, 20)
   {
      paraTypeArrayDestroy(tableHeaders[index]);
   }

   greturn gbTRUE;
}

/******************************************************************************
func: _Write_ScopePopAll
******************************************************************************/
static void _Write_ScopePopAll(WriteFunctions * const func, Gfile * const file, 
   Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType)
{
   genter;

   loop
   {
      // No more scopes.
      breakIf(*scopeLevel == -1);

      if (scopeType[*scopeLevel] == paraTypeSCOPE_LIST_BULLET ||
          scopeType[*scopeLevel] == paraTypeSCOPE_LIST_NUMBER)
      {
         *indentLevel = *indentLevel - 1;
      }

      // Pop the scope.
      func->FileWriteScopeStop(
         file, 
         scopeType[*scopeLevel], 
         (*scopeLevel == 0) ?
            paraTypeNONE :
            scopeType[*scopeLevel] - 1);
      *scopeLevel = *scopeLevel - 1;
   }

   greturn;
}

/******************************************************************************
func: _Write_ScopePopToOpenScope
******************************************************************************/
static void _Write_ScopePopToOpenScope(WriteFunctions * const func, Gfile * const file, 
   Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType)
{
   genter;

   loop
   {
      // No more scopes.
      breakIf(*scopeLevel == -1);

      // We are in a scope that can accept anything.
      if (scopeType[*scopeLevel] == paraTypeSCOPE_ITEM                           ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_HEADER            ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK   ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL       ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN                   ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_NO_BREAK          ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_FILL              ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_NUMBER            ||
          scopeType[*scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_NUMBER_FILL)
      {
         break;
      }

      if (scopeType[*scopeLevel] == paraTypeSCOPE_LIST_BULLET ||
          scopeType[*scopeLevel] == paraTypeSCOPE_LIST_NUMBER)
      {
         *indentLevel = *indentLevel - 1;
      }

      // We are nto in a scope that can accept anything.
      func->FileWriteScopeStop(
         file,
         scopeType[*scopeLevel], 
         (*scopeLevel == 0) ?
            paraTypeNONE :
            scopeType[*scopeLevel] - 1);
      *scopeLevel = *scopeLevel - 1;
   }

   greturn;
}

/******************************************************************************
func: _Write_ScopePopToListScope
******************************************************************************/
static void _Write_ScopePopToListScope(WriteFunctions * const func, 
   Gfile * const file, Gindex * const indentLevel, Gindex * const scopeLevel, 
   ParaType * const scopeType)
{
   genter;

   indentLevel;

   loop
   {
      // No more scopes.
      breakIf(*scopeLevel == -1);

      // Found a table row scope.
      if (scopeType[*scopeLevel] == paraTypeSCOPE_LIST_BULLET    ||
          scopeType[*scopeLevel] == paraTypeSCOPE_LIST_KEY_VALUE ||
          scopeType[*scopeLevel] == paraTypeSCOPE_LIST_NUMBER)
      {
         break;
      }

      // We are nto in a scope that can accept anything.
      func->FileWriteScopeStop(
         file, 
         scopeType[*scopeLevel], 
         (*scopeLevel == 0) ?
            paraTypeNONE :
            scopeType[*scopeLevel] - 1);
      *scopeLevel = *scopeLevel - 1;
   }

   greturn;
}

/******************************************************************************
func: _Write_ScopePopToTableRow
******************************************************************************/
static void _Write_ScopePopToTableRow(WriteFunctions * const func, 
   Gfile * const file, Gindex * const indentLevel, Gindex * const scopeLevel, 
   ParaType * const scopeType)
{
   genter;

   loop
   {
      // No more scopes.
      breakIf(*scopeLevel == -1);

      // Found a table row scope.
      if (scopeType[*scopeLevel] == paraTypeTABLE_ROW)
      {
         break;
      }

      if (scopeType[*scopeLevel] == paraTypeSCOPE_LIST_BULLET ||
          scopeType[*scopeLevel] == paraTypeSCOPE_LIST_NUMBER)
      {
         *indentLevel = *indentLevel - 1;
      }

      // We are nto in a scope that can accept anything.
      func->FileWriteScopeStop(
         file, 
         scopeType[*scopeLevel], 
         (*scopeLevel == 0) ?
            paraTypeNONE :
            scopeType[*scopeLevel] - 1);
      *scopeLevel = *scopeLevel - 1;
   }

   greturn;
}

/******************************************************************************
func: _Write_ScopePopToTable
******************************************************************************/
static void _Write_ScopePopTable(WriteFunctions * const func, Gfile * const file, 
   Gindex * const indentLevel, Gindex * const scopeLevel, ParaType * const scopeType)
{
   genter;

   loop
   {
      // No more scopes.
      breakIf(*scopeLevel == -1);

      // We are in a scope that can accept anything.
      if (scopeType[*scopeLevel] == paraTypeSCOPE_TABLE)
      {
         break;
      }

      if (scopeType[*scopeLevel] == paraTypeSCOPE_LIST_BULLET ||
          scopeType[*scopeLevel] == paraTypeSCOPE_LIST_NUMBER)
      {
         *indentLevel = *indentLevel - 1;
      }

      // We are nto in a scope that can accept anything.
      func->FileWriteScopeStop(
         file, 
         scopeType[*scopeLevel], 
         (*scopeLevel == 0) ?
            paraTypeNONE :
            scopeType[*scopeLevel] - 1);
      *scopeLevel = *scopeLevel - 1;
   }

   greturn;
}
