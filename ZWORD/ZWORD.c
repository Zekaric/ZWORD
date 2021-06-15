/******************************************************************************
file:       ZWORD
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2020, Robbert de Groot

description:
Entry point for the WORD program

WORD : W)ords O)rganized R)udimentary D)ocumentation

zword all D:\pzgithub\ZWORD\README.zword D:\pzgithub\ZWORD\ZWORD\zword.paper
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "ZWORD.h"

/******************************************************************************
local:
type:
******************************************************************************/
typedef struct
{
   Gs    *key;
   Gs    *value;
} Variable;

/******************************************************************************
Variable containers.
******************************************************************************/
// Same as G_Array ////////////////////////////////////////////////////////////
typedef struct 
{
   GCONTAINER_VAR;

   Gcount                   countTotal;
   Gbit                     isVectorSizing   : 1,
                            isSorted         : 1,
                            isNullEnding     : 1;
   GrlCompareFunc           compareFunc;
   Variable                 **p;
} VariableArray;

#define variableArrayAdd(                ARRAY,        VALUE)                             g_ArrayAdd(                (G_Array *) ARRAY,        (Gp *) VALUE) 
#define variableArrayAddAt(              ARRAY, INDEX, VALUE)                             g_ArrayAddAt(              (G_Array *) ARRAY, INDEX, (Gp *) VALUE) 
#define variableArrayAddBegin(           ARRAY,        VALUE)                             g_ArrayAddBegin(           (G_Array *) ARRAY,        (Gp *) VALUE) 
#define variableArrayAddEnd(             ARRAY,        VALUE)                             g_ArrayAddEnd(             (G_Array *) ARRAY,        (Gp *) VALUE) 
#define variableArrayClear(              ARRAY, COUNT, INDEX)                             g_ArrayClear(              (G_Array *) ARRAY, COUNT, INDEX) 
#define variableArrayCopy(               ARRAY, COUNT, INDEXSRC, INDEXDST)                g_ArrayCopy(               (G_Array *) ARRAY, COUNT, INDEXSRC, INDEXDST) 
#define variableArrayCopyFrom(           ARRAYDST, INDEXDST, ARRAYSRC, COUNT, INDEXSRC)   g_ArrayCopyFrom(           (G_Array *) ARRAYDST, INDEXDST, (G_Array *) ARRAYSRC, COUNT, INDEXSRC) 
#define variableArrayCreate(                  )                   (VariableArray *)       g_ArrayCreate(                                VariableArray, Variable *, gbTRUE, NULL, gbTRUE, gbFALSE)
#define variableArrayCreateContent(      ARRAY)                                           g_ArrayCreateContent(      (G_Array *) ARRAY, VariableArray, Variable *, gbTRUE, NULL, gbTRUE, gbFALSE)
#define variableArrayDestroy(            ARRAY)                                           g_ArrayDestroy(            (G_Array *) ARRAY) 
#define variableArrayDestroyContent(     ARRAY)                                           g_ArrayDestroyContent(     (G_Array *) ARRAY) 
#define variableArrayErase(              ARRAY, VALUE)                                    g_ArrayErase(              (G_Array *) ARRAY, (Gp *) VALUE) 
#define variableArrayEraseAt(            ARRAY, COUNT, INDEX)                             g_ArrayEraseAt(            (G_Array *) ARRAY, COUNT, INDEX) 
#define variableArrayEraseBegin(         ARRAY)                                           g_ArrayEraseBegin(         (G_Array *) ARRAY) 
#define variableArrayEraseEnd(           ARRAY)                                           g_ArrayEraseEnd(           (G_Array *) ARRAY) 
#define variableArrayFind(               ARRAY, VALUE)                                    g_ArrayFind(               (G_Array *) ARRAY, (Gp *) VALUE) 
#define variableArrayFlush(              ARRAY)                                           g_ArrayFlush(              (G_Array *) ARRAY) 
#define variableArrayForEach(            ARRAY, FUNC)                                     g_ArrayForEach(            (G_Array *) ARRAY, FUNC) 
#define variableArrayGet(                ARRAY)                  ((Variable **)           g_ArrayGet(                (G_Array *) ARRAY))
#define variableArrayGetAt(              ARRAY, INDEX)           ((Variable *)            g_ArrayGetAt(              (G_Array *) ARRAY, INDEX))
#define variableArrayGetBegin(           ARRAY)                  ((Variable *)            g_ArrayGetBegin(           (G_Array *) ARRAY))
#define variableArrayGetCount(           ARRAY)                                           g_ArrayGetCount(           (G_Array *) ARRAY) 
#define variableArrayGetEnd(             ARRAY)                  ((Variable *)            g_ArrayGetEnd(             (G_Array *) ARRAY))
#define variableArrayGetSize(            ARRAY)                                           g_ArrayGetSize(            (G_Array *) ARRAY) 
#define variableArraySetCount(           ARRAY, COUNT)                                    g_ArraySetCount(           (G_Array *) ARRAY, COUNT) 
#define variableArraySort(               ARRAY)                                           g_ArraySort(               (G_Array *) ARRAY) 
#define variableArraySwap(               ARRAY, INDEXA, INDEXB)                           g_ArraySwap(               (G_Array *) ARRAY, INDEXA, INDEXB) 
#define variableArrayUpdateAt(           ARRAY, INDEX, VALUE)                             g_ArrayUpdateAt(           (G_Array *) ARRAY, INDEX, (Gp *) VALUE) 

/******************************************************************************
local:
variable:
******************************************************************************/
static Gs            *_chapterString   = NULL;
static VariableArray *_varList         = NULL;

/******************************************************************************
prototype:
******************************************************************************/
static Gs   *_ChapterGetString(  ParaChapter const chapter);
                                 
static Gb    _IsInListScope(     Gindex const scopeLevel, ParaType const * const scopeType);
static Gb    _IsInOpenScope(     Gindex const scopeLevel, ParaType const * const scopeType);
                                 
static Gs   *_ParaListGetTitle(  ParaArray const * const paraList);
                                 
static int   _Process(           Gs const * const command, Gpath const * const path);
                                 
static Gb    _Write(             WriteFunctions * const func, Gpath const * const path, ParaArray const * const paraList);
static void  _WriteError(        Para const * const para, Gc2 const * const type);

static Gb    _VariableSet(       Gs *key, Gs *value);

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
   _varList  = variableArrayCreate();

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
   PaperStop();

   variableArrayDestroy(_varList);

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
func: _IsInListScope
******************************************************************************/
static Gb _IsInListScope(Gindex const scopeLevel, ParaType const * const scopeType)
{
   genter;

   greturnTrueIf(
      scopeType[scopeLevel] == paraTypeSCOPE_LIST_BULLET    ||
      scopeType[scopeLevel] == paraTypeSCOPE_LIST_KEY_VALUE ||
      scopeType[scopeLevel] == paraTypeSCOPE_LIST_NUMBER);

   greturn gbFALSE;
}

/******************************************************************************
func: _IsInOpenScope
******************************************************************************/
static Gb _IsInOpenScope(Gindex const scopeLevel, ParaType const * const scopeType)
{
   genter;

   greturnTrueIf(scopeLevel == -1);

   greturnTrueIf(
      scopeType[scopeLevel] == paraTypeSCOPE_ITEM                          ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_HEADER           ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK  ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL      ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN                  ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_NO_BREAK         ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_FILL             ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_NUMBER           ||
      scopeType[scopeLevel] == paraTypeSCOPE_TABLE_COLUMN_NUMBER_FILL);

   greturn gbFALSE;
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

      // Set the line number.
      para->line = index;

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
                  gsAppend( para->str, gsTrimRightU2(stemp, WHITESPACE_U2));
               }
               else
               {
                  para->str = gsTrimRightU2(stemp, WHITESPACE_U2);
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
         // Variable definition
         else if (gsIsEqualU2(markup, L"v"))
         {
            GsArray  *stempArray;
            Gs       *skey,
                     *svalue;

            stempArray = gsCreateSplit(stemp, L'\t');
            
            skey   = gsArrayGetAt(stempArray, 0);
            svalue = gsArrayGetAt(stempArray, 1);

            gsAddBegin(skey, L"|");
            gsAddEnd(  skey, L"|");
         
            _VariableSet(skey, svalue);

            gsArrayDestroy(stempArray);
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
         if (!_IsInOpenScope(scopeLevel, scopeType))
         {
            _WriteError(para, L"paragraph");
            greturn gbFALSE;
         }

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

         if (scopeLevel != -1)
         {
            _WriteError(para, L"header");
            greturn gbFALSE;
         }

         func->FileWriteTitle(file, para);
         break;

      case paraTypeSCOPE_ITEM:
         debugPrintMsg(L"item>>>>>>>>>>>>>>>> %d\n", scopeLevel + 1);

         // List Item not found in a list scope.
         greturnFalseIf(
            !(scopeType[scopeLevel] == paraTypeSCOPE_LIST_BULLET    ||
              scopeType[scopeLevel] == paraTypeSCOPE_LIST_KEY_VALUE ||
              scopeType[scopeLevel] == paraTypeSCOPE_LIST_NUMBER));

         scopeLevel++;
         greturnFalseIf(scopeLevel >= 20);

         scopeType[scopeLevel] = para->type;

         func->FileWriteScopeStart(
            file, 
            para->type, 
            scopeLevel, 
            scopeType[scopeLevel - 1]);
         break;

      case paraTypeSCOPE_LIST_BULLET:
      case paraTypeSCOPE_LIST_KEY_VALUE:
      case paraTypeSCOPE_LIST_NUMBER:
         switch (para->type)
         {
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
         }

         // Ensure we are in an open scope that can accept a list.
         if (!_IsInOpenScope(scopeLevel, scopeType))
         {
            _WriteError(para, L"list");
            greturn gbFALSE;
         }

         scopeLevel++;
         greturnFalseIf(scopeLevel >= 20);

         scopeType[scopeLevel] = para->type;

         func->FileWriteScopeStart(file, para->type, scopeLevel, paraTypeNONE);
         break;

      case paraTypeSCOPE_TABLE:
         debugPrintMsg(L"table>>>>>>>>>>>>>>> %d\n", scopeLevel + 1);
         
         // Ensure we are in an open scope that can accept a table.
         if (!_IsInOpenScope(scopeLevel, scopeType))
         {
            _WriteError(para, L"table");
            greturn gbFALSE;
         }

         scopeLevel++;
         greturnFalseIf(scopeLevel >= 20);

         scopeType[scopeLevel] = para->type;

         paraArrayFlush(tableHeaders[scopeLevel]);
         isTableHeaderSeparatorWritten[scopeLevel] = gbFALSE;

         // New table. Assume the first row comes along for the ride
         scopeLevel++;
         scopeType[scopeLevel] = paraTypeTABLE_ROW;

         func->FileWriteScopeStart(file, para->type, scopeLevel, paraTypeNONE);
         break;

      case paraTypeSCOPE_TABLE_COLUMN_HEADER:
      case paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK:
      case paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL:
      case paraTypeSCOPE_TABLE_COLUMN:
      case paraTypeSCOPE_TABLE_COLUMN_NO_BREAK:
      case paraTypeSCOPE_TABLE_COLUMN_FILL:
         switch (para->type)
         {
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

         if (scopeType[scopeLevel] != paraTypeTABLE_ROW)
         {
            _WriteError(para, L"table column");
            greturn gbFALSE;
         }

         scopeLevel++;
         greturnFalseIf(scopeLevel >= 20);

         scopeType[scopeLevel] = para->type;

         func->FileWriteScopeStart(file, para->type, scopeLevel, paraTypeNONE);
         break;

      case paraTypeSCOPE_STOP:
         debugPrintMsg(L"scope pop>>>>>>>>>>> %d\n", scopeLevel);
         returnFalseIf(scopeLevel < 0);

         if (scopeType[scopeLevel] == paraTypeSCOPE_LIST_BULLET ||
             scopeType[scopeLevel] == paraTypeSCOPE_LIST_NUMBER)
         {
            indentLevel--;
         }

         // Ending a table.  Need to end the row scope as well.
         if (scopeType[scopeLevel] == paraTypeTABLE_ROW)
         {
            func->FileWriteScopeStop(
               file, 
               scopeType[scopeLevel], 
               (scopeLevel > 0) ?
                  scopeType[scopeLevel - 1] :
                  paraTypeNONE);

            scopeLevel--;
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

         if (!_IsInListScope(scopeLevel, scopeType))
         {
            _WriteError(para, L"list item");
            greturn gbFALSE;
         }

         if (scopeType[scopeLevel] == paraTypeSCOPE_LIST_KEY_VALUE)
         {
            func->FileWriteKeyValue(file, para);
         }
         else
         {
            func->FileWriteScopeStart(
               file,
               para->type,
               indentLevel,
               scopeType[scopeLevel]);
            func->FileWriteString(    file, para);
            func->FileWriteScopeStop( file, para->type, scopeType[scopeLevel]);
         }
         break;

      case paraTypeTABLE_ROW:
         debugPrintMsg(L"table row>>>>>>>>>>> %d\n", scopeLevel);
         if (scopeType[scopeLevel] != paraTypeTABLE_ROW)
         {
            _WriteError(para, L"table row");
            greturn gbFALSE;
         }

         func->FileWriteScopeStop(file, para->type, paraTypeNONE);

         if (!isTableHeaderSeparatorWritten[scopeLevel - 1])
         {
            func->FileWriteTableHeaderSeparator(file, tableHeaders[scopeLevel - 1]);
            isTableHeaderSeparatorWritten[scopeLevel - 1] = gbTRUE;
         }

         func->FileWriteScopeStart(file, para->type, scopeLevel, paraTypeNONE);
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

         // Ensure we can write this here.
         if (!_IsInOpenScope(scopeLevel, scopeType))
         {
            _WriteError(para, L"formatted paragraph");
            greturn gbFALSE;
         }

         func->FileWriteScopeStart(     file, para->type, scopeLevel, scopeType[scopeLevel]);
         func->FileWriteStringUnaltered(file, para);
         func->FileWriteScopeStop(      file, para->type, paraTypeNONE);
         break;

      case paraTypeTABLE_OF_CONTENTS:
         debugPrintMsg(L"toc>>>>>>>>>>>>>>>>> %d\n", scopeLevel);

         if (scopeLevel != -1)
         {
            _WriteError(para, L"table of contents");
            greturn gbFALSE;
         }

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
func: _WriteError
******************************************************************************/
static void _WriteError(Para const * const para, Gc2 const * const type)
{
   Gs *stemp;

   genter;

   stemp = gsCreateFromFormatted(
      L"ERROR: Attempted to add [TYPE] is a scope where it is not allowed and line [LINE].\n",
      L"[TYPE]", gsFormattedTypeU2, type,
      L"[LINE]", gsFormattedTypeN,  (Gn) para->line,
      NULL);

   gconSetS(stemp);

   gsDestroy(stemp);

   greturn;
}

/******************************************************************************
func: variableSet
******************************************************************************/
Gb _VariableSet(Gs *key, Gs *value)
{
   Variable *v;

   genter;

   greturnFalseIf(!key || !value);

   v = memCreateType(Variable);
   greturnFalseIf(!v);

   v->key   = key;
   v->value = value;

   variableArrayAddBegin(_varList, v);

   greturn gbTRUE;
}

/******************************************************************************
func: variableReplace
******************************************************************************/
void VariableReplace(Gs * const value)
{
   Gindex    index;
   Gcount    count;
   Variable *var;

   genter;

   loop
   {
      count = 0;

      forCount(index, variableArrayGetCount(_varList))
      {
         var = variableArrayGetAt(_varList, index);

         gsFindAndReplace(value, var->key, var->value, &count);

         breakIf(count);
      }

      breakIf(!count);
   }
}
