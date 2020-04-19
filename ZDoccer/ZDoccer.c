/******************************************************************************
file:       ZDoccer
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2020, Robbert de Groot

description:
Entry point for the Doccer program
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "grl.h"

/******************************************************************************
local:
constant:
******************************************************************************/

/******************************************************************************
type:
******************************************************************************/
typedef enum
{
   paraTypeNONE,

   paraTypeREGULAR,

   paraTypeTITLE_1,
   paraTypeTITLE_2,
   paraTypeTITLE_3,
   paraTypeTITLE_4,
   paraTypeTITLE_5,
   paraTypeTITLE_6,
   paraTypeTITLE_7,
   paraTypeTITLE_8,
   paraTypeTITLE_9,
   paraTypeTITLE_TOC_1,
   paraTypeTITLE_TOC_2,
   paraTypeTITLE_TOC_3,
   paraTypeTITLE_TOC_4,
   paraTypeTITLE_TOC_5,
   paraTypeTITLE_TOC_6,
   paraTypeTITLE_TOC_7,
   paraTypeTITLE_TOC_8,
   paraTypeTITLE_TOC_9,
   
   paraTypeLIST_START_BULLET,
   paraTypeLIST_START_NUMBER,
   paraTypeLIST_STOP_BULLET,
   paraTypeLIST_STOP_NUMBER,
   paraTypeLIST_ITEM,
   paraTypeLIST_ITEM_START,
   paraTypeLIST_ITEM_STOP,

   paraTypeTABLE_START,
   paraTypeTABLE_STOP,
   paraTypeTABLE_ROW,
   paraTypeTABLE_COL_HEADER,
   paraTypeTABLE_COL_HEADER_NO_BREAK,
   paraTypeTABLE_COL_HEADER_FILL,
   paraTypeTABLE_COL,
   paraTypeTABLE_COL_NO_BREAK,
   paraTypeTABLE_COL_FILL,

   paraTypeFORMATED_START,
   paraTypeFORMATED_STOP,

   paraTypeTABLE_OF_CONTENTS,

   paraTypePAGE_BREAK,

   paraTypeCOUNT
} ParaType;

typedef struct 
{
   Gindex          value[10];
} ParaChapter;

typedef struct
{
   ParaType        type;
   ParaChapter     chapter;
   Gs             *str;
} Para;

/******************************************************************************
Para containers.
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
   Para                 **p;
} ParaArray;

#define paraArrayAdd(             ARRAY,        VALUE)                                            g_ArrayAdd(             (G_Array *) ARRAY,        (Gp *) VALUE) 
#define paraArrayAddAt(           ARRAY, INDEX, VALUE)                                            g_ArrayAddAt(           (G_Array *) ARRAY, INDEX, (Gp *) VALUE) 
#define paraArrayAddBegin(        ARRAY,        VALUE)                                            g_ArrayAddBegin(        (G_Array *) ARRAY,        (Gp *) VALUE) 
#define paraArrayAddEnd(          ARRAY,        VALUE)                                            g_ArrayAddEnd(          (G_Array *) ARRAY,        (Gp *) VALUE) 
#define paraArrayClear(           ARRAY, COUNT, INDEX)                                            g_ArrayClear(           (G_Array *) ARRAY, COUNT, INDEX) 
#define paraArrayCopy(            ARRAY, COUNT, INDEXSRC, INDEXDST)                               g_ArrayCopy(            (G_Array *) ARRAY, COUNT, INDEXSRC, INDEXDST) 
#define paraArrayCopyFrom(        ARRAYDST, INDEXDST, ARRAYSRC, COUNT, INDEXSRC)                  g_ArrayCopyFrom(        (G_Array *) ARRAYDST, INDEXDST, (G_Array *) ARRAYSRC, COUNT, INDEXSRC) 
#define paraArrayCreate(               )                                            (ParaArray *) g_ArrayCreate(                             ParaArray, Para *, gbTRUE, NULL, gbTRUE, gbFALSE)
#define paraArrayCreateContent(   ARRAY)                                                          g_ArrayCreateContent(   (G_Array *) ARRAY, ParaArray, Para *, gbTRUE, NULL, gbTRUE, gbFALSE)
#define paraArrayDestroy(         ARRAY)                                                          g_ArrayDestroy(         (G_Array *) ARRAY) 
#define paraArrayDestroyContent(  ARRAY)                                                          g_ArrayDestroyContent(  (G_Array *) ARRAY) 
#define paraArrayErase(           ARRAY, VALUE)                                                   g_ArrayErase(           (G_Array *) ARRAY, (Gp *) VALUE) 
#define paraArrayEraseAt(         ARRAY, COUNT, INDEX)                                            g_ArrayEraseAt(         (G_Array *) ARRAY, COUNT, INDEX) 
#define paraArrayEraseBegin(      ARRAY)                                                          g_ArrayEraseBegin(      (G_Array *) ARRAY) 
#define paraArrayEraseEnd(        ARRAY)                                                          g_ArrayEraseEnd(        (G_Array *) ARRAY) 
#define paraArrayFind(            ARRAY, VALUE)                                                   g_ArrayFind(            (G_Array *) ARRAY, (Gp *) VALUE) 
#define paraArrayFlush(           ARRAY)                                                          g_ArrayFlush(           (G_Array *) ARRAY) 
#define paraArrayForEach(         ARRAY, FUNC)                                                    g_ArrayForEach(         (G_Array *) ARRAY, FUNC) 
#define paraArrayGet(             ARRAY)                                            ((Para **)    g_ArrayGet(             (G_Array *) ARRAY))
#define paraArrayGetAt(           ARRAY, INDEX)                                     ((Para *)     g_ArrayGetAt(           (G_Array *) ARRAY, INDEX))
#define paraArrayGetBegin(        ARRAY)                                            ((Para *)     g_ArrayGetBegin(        (G_Array *) ARRAY))
#define paraArrayGetCount(        ARRAY)                                                          g_ArrayGetCount(        (G_Array *) ARRAY) 
#define paraArrayGetEnd(          ARRAY)                                            ((Para *)     g_ArrayGetEnd(          (G_Array *) ARRAY))
#define paraArrayGetSize(         ARRAY)                                                          g_ArrayGetSize(         (G_Array *) ARRAY) 
#define paraArraySetCount(        ARRAY, COUNT)                                                   g_ArraySetCount(        (G_Array *) ARRAY, COUNT) 
#define paraArraySort(            ARRAY)                                                          g_ArraySort(            (G_Array *) ARRAY) 
#define paraArraySwap(            ARRAY, INDEXA, INDEXB)                                          g_ArraySwap(            (G_Array *) ARRAY, INDEXA, INDEXB) 
#define paraArrayUpdateAt(        ARRAY, INDEX, VALUE)                                            g_ArrayUpdateAt(        (G_Array *) ARRAY, INDEX, (Gp *) VALUE) 

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
static Gs   *_ChapterGetLink(    ParaChapter const chapter);

static Gs   *_ParaListGetTitle(  ParaArray const * const paraList);

static int   _Process(           Gs const * const command, Gpath const * const path);

static Gb    _WriteHTML(         Gpath const * const path, ParaArray const * const paraList);
static Gb    _WriteHTMLTOC(      Gfile       * const file, ParaArray const * const paraList);
static Gb    _WriteMD(           Gpath const * const path, ParaArray const * const paraList);
static Gb    _WriteMDTOC(        Gfile       * const file, ParaArray const * const paraList);
static Gb    _WriteZLYT(         Gpath const * const path, ParaArray const * const paraList);

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
         *file;
   Gpath *path;
   Gi4    result;

   if (!grlStart())
   {
      return 1;
   }

   gconSetA(
      "Z:Doccer\n"
      "========\n"
      "\n");

   // Print the help.
   if (acount <= 1)
   {
      gconSetA(
         "zdoccer [command] [file]\n"
         "\n"
         "[command] = \n"
         "   \"ALL\"  convert file to all the formats.\n"
         "   \"HTML\" convert file to HTML format.\n"
         "   \"MD\"   convert file to MD markup.\n"
         // future "   \"PDF\"  convert file to PDF format.\n"
         // future "   \"RTF\"  convert file to RTF format.\n"
         "   \"ZLYT\" convert file to Z:LYT markup.\n"
         "\n"
         "[file]    =\n"
         "   Fully qualified file path.  Including .zdoc extension.\n"
         "\n");

      return 0;
   }

   // Get the command and the file.
   command = gsCreateFromA(alist[1]);
   file    = gsCreateFromA(alist[2]);

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
func: _ChapterGetLink

Convert a chapter to a link string.
******************************************************************************/
static Gs *_ChapterGetLink(ParaChapter const chapter)
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
         gsAppendU2(str, L"_");
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
         greturn para->str;
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

      if (paraTypeTITLE_TOC_1 <= para->type && para->type <= paraTypeTITLE_TOC_9)
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
   Gi4          result;
   Gfile       *file;
   GsArray     *strList;
   Gcount       strListCount;
   Gindex       index,
                cindex,
                position;
   Gs          *stemp,
               *markup;
   Para        *para;
   ParaArray   *paraList;
   ParaChapter  chapter;

   genter;

   // Nothing wrong
   result = 0;

   strList = gsArrayCreate(NULL, gbTRUE);

   // Open the file.
   file = gfileOpen(path, gfileOpenModeREAD_ONLY);

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
         stemp = gsStrip(stemp, gcStripWHITE_SPACE_LEADING | gcStripWHITE_SPACE_TRAILING);

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
         para->type    = paraTypeREGULAR;
         para->chapter = chapter;
         para->str     = stemp;

         // Keep appending strings until a blank line.
         for (index++; index < strListCount; index++)
         {
            stemp = gsArrayGetAt(strList, index);

            // Blank line or new tag at 
            breakIf(gsIsBlank(stemp));

            position = gsFindFirstOfU2(stemp, 0, WHITESPACE_U2);
            
            // Regular paragraph.
            if (position == 0)
            {
               index--;
               break;
            }

            // Do we markup that isn't blank.
            markup = gsCreateFromSub(stemp, 0, position);
            if (*gsGetAt(markup, 0) == L'*' ||
                *gsGetAt(markup, 0) == L'~' ||
                gsIsEqualU2(markup, L"=[") ||
                gsIsEqualU2(markup, L"]=") ||
                gsIsEqualU2(markup, L"#[") ||
                gsIsEqualU2(markup, L"]#") ||
                gsIsEqualU2(markup, L"-")  ||
                gsIsEqualU2(markup, L"-[") ||
                gsIsEqualU2(markup, L"]-") ||
                gsIsEqualU2(markup, L"t[") ||
                gsIsEqualU2(markup, L"]t") ||
                gsIsEqualU2(markup, L"t-") ||
                gsIsEqualU2(markup, L"th") ||
                gsIsEqualU2(markup, L"t|") ||
                gsIsEqualU2(markup, L"[") ||
                gsIsEqualU2(markup, L"[[") ||
                gsIsEqualU2(markup, L"]]") ||
                gsIsEqualU2(markup, L"toc") ||
                gsIsEqualU2(markup, L"==="))
            {
               gsDestroy(markup);
               index--;
               break;
            }
            gsDestroy(markup);

            // Just a continuation of the paragraph.
            gsAppendA(para->str, " ");
            gsAppend(para->str, gsStrip(stemp, gcStripWHITE_SPACE_LEADING | gcStripWHITE_SPACE_TRAILING));
         }
      }
      else
      {
         // We have a special paragraph
         markup = gsCreateFromSub(stemp, 0, position);
         gsEraseSub(stemp, 0, position);
         gsStrip(stemp, gcStripWHITE_SPACE_LEADING | gcStripWHITE_SPACE_TRAILING);

         // Title Lines.
         if (*gsGetAt(markup, 0) == L'*' ||
             *gsGetAt(markup, 0) == L'~')
         {
            if (*gsGetAt(markup, 0) == L'*') para->type = paraTypeTITLE_TOC_1;
            else                             para->type = paraTypeTITLE_1;

            para->type += position - 1;

            // Increment the chapter.
            chapter.value[position - 1]++;

            // Reset the subchapter counts.
            for (cindex = position; cindex < 10; cindex++)
            {
               chapter.value[cindex] = 0;
            }

            para->str     = stemp;
         }
         // List Lines.
         else if (gsIsEqualU2(markup, L"=["))
         {
            para->type     = paraTypeLIST_START_BULLET;
         }
         else if (gsIsEqualU2(markup, L"]="))
         {
            para->type     = paraTypeLIST_STOP_BULLET;
         }
         else if (gsIsEqualU2(markup, L"#["))
         {
            para->type     = paraTypeLIST_START_NUMBER;
         }
         else if (gsIsEqualU2(markup, L"]#"))
         {
            para->type     = paraTypeLIST_STOP_NUMBER;
         }
         else if (gsIsEqualU2(markup, L"-"))
         {
            para->type     = paraTypeLIST_ITEM;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"-["))
         {
            para->type     = paraTypeLIST_ITEM_START;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"]-"))
         {
            para->type     = paraTypeLIST_ITEM_STOP;
            para->str      = stemp;
         }
         // Table Lines.
         else if (gsIsEqualU2(markup, L"t["))
         {
            para->type     = paraTypeTABLE_START;
         }
         else if (gsIsEqualU2(markup, L"]t"))
         {
            para->type     = paraTypeTABLE_STOP;
         }
         else if (gsIsEqualU2(markup, L"t-"))
         {
            para->type     = paraTypeTABLE_ROW;
         }
         else if (gsIsEqualU2(markup, L"th"))
         {
            para->type     = paraTypeTABLE_COL_HEADER;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"thx"))
         {
            para->type     = paraTypeTABLE_COL_HEADER_NO_BREAK;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"th*"))
         {
            para->type     = paraTypeTABLE_COL_HEADER_FILL;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"t|"))
         {
            para->type     = paraTypeTABLE_COL;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"t|x"))
         {
            para->type     = paraTypeTABLE_COL_NO_BREAK;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"t|*"))
         {
            para->type     = paraTypeTABLE_COL_FILL;
            para->str      = stemp;
         }
         // Formated Lines.
         else if (gsIsEqualU2(markup, L"["))
         {
            para->type     = paraTypeFORMATED_START;
            para->str      = gsStrip(stemp, gcStripWHITE_SPACE_LEADING | gcStripWHITE_SPACE_TRAILING);
         }
         else if (gsIsEqualU2(markup, L"[["))
         {
            para->type     = paraTypeFORMATED_START;

            // Special case.  No processing of the lines inside.
            // Keep appending strings until a terminator.
            for (index++; index < strListCount; index++)
            {
               stemp = gsArrayGetAt(strList, index);

               // Break; on terminator.
               breakIf(
                  gsGetCount(stemp) >= 2     &&
                  *gsGetAt(stemp, 0) == L']' &&
                  *gsGetAt(stemp, 1) == L']');

               if (para->str)
               {
                  gsAppendA(para->str, "\n");
                  gsAppend( para->str, gsStrip(stemp, gcStripWHITE_SPACE_TRAILING));
               }
               else
               {
                  para->str = gsStrip(stemp, gcStripWHITE_SPACE_TRAILING);
               }
            }
         }
         else if (gsIsEqualU2(markup, L"]]"))
         {
            // This should never happen.
            para->type     = paraTypeFORMATED_STOP;
         }
         // TOC
         else if (gsIsEqualU2(markup, L"toc"))
         {
            para->type = paraTypeTABLE_OF_CONTENTS;
         }
         // page
         else if (gsIsEqualU2(markup, L"==="))
         {
            para->type = paraTypePAGE_BREAK;
         }

         para->chapter = chapter;

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
      _WriteHTML(path, paraList);
   }
   if (gsIsEqualU2(command, L"all") ||
       gsIsEqualU2(command, L"md"))
   {
      _WriteMD(path, paraList);
   }
   if (gsIsEqualU2(command, L"all") ||
       gsIsEqualU2(command, L"ZLYT"))
   {
      _WriteZLYT(path, paraList);
   }

   // Clean up
   gsArrayForEach(strList, gsDestroyFunc);
   gsArrayDestroy(strList);

   forCount (index, paraArrayGetCount(paraList))
   {
      para = paraArrayGetAt(paraList, index);

      gmemDestroy(para);
   }
   paraArrayDestroy(paraList);

   greturn result;
}

/******************************************************************************
func: _WriteHTML
******************************************************************************/
static Gb _WriteHTML(Gpath const * const path, ParaArray const * const paraList)
{
   Gpath *pathHtml;
   Gfile *file;
   Gindex index;
   Para  *para;
   Gs    *title;
   Gs    *chapter;
   Gb     isTableRowActive,
          isTableColHActive,
          isTableColActive,
          isListItemActive;

   genter;

   isTableRowActive  = gbFALSE;
   isTableColHActive = gbFALSE;
   isTableColActive  = gbFALSE;
   isListItemActive  = gbFALSE;
      
   // Make the destination file path.
   pathHtml = gpathCreateFrom(path);
   gpathPopExtension(pathHtml);
   gpathPushExtensionA(pathHtml, "html");

   // Create the file.
   file = gfileOpen(pathHtml, gfileOpenModeREAD_WRITE_NEW);

   gpathDestroy(pathHtml);

   greturnFalseIf(!file);

   // Write the header
   gfileSetA(
      file,
      gcTypeU1,
      "<!DOCTYPE html>\n"
      "<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n"
      "\n"
      " <head>\n"
      "  <meta charset=\"utf-8\" />\n"
      "  <link rel=\"stylesheet\" type=\"text/css\" href=\"style_reset.css\">\n"
      "  <link rel=\"stylesheet\" type=\"text/css\" href=\"style_zdoc.css\">\n"
      "  <title>",
      NULL);

   // Write the title
   title = _ParaListGetTitle(paraList);
   if (title)
   {
      gfileSetS(file, gcTypeU1, title, NULL);
   }

   // Finish header and start the body.
   gfileSetA(
      file,
      gcTypeU1,
      "</title>\n"
      " </head>\n"
      "\n"
      " <body class=\"zdoc\">\n",
      NULL);

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
         gfileSetA(file, gcTypeU1, "<p class=\"zdoc\">", NULL);
         gfileSetS(file, gcTypeU1, para->str, NULL);
         gfileSetA(file, gcTypeU1, "</p>\n", NULL);
         break;

      case paraTypeTITLE_1:
      case paraTypeTITLE_2:
      case paraTypeTITLE_3:
      case paraTypeTITLE_4:
      case paraTypeTITLE_5:
      case paraTypeTITLE_6:
      case paraTypeTITLE_7:
      case paraTypeTITLE_8:
      case paraTypeTITLE_9:
      case paraTypeTITLE_TOC_1:
      case paraTypeTITLE_TOC_2:
      case paraTypeTITLE_TOC_3:
      case paraTypeTITLE_TOC_4:
      case paraTypeTITLE_TOC_5:
      case paraTypeTITLE_TOC_6:
      case paraTypeTITLE_TOC_7:
      case paraTypeTITLE_TOC_8:
      case paraTypeTITLE_TOC_9:
         switch (para->type)
         {
         case paraTypeTITLE_1:
         case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "\n\n<h1 class=\"zdoc\">", NULL); break;
         case paraTypeTITLE_2:
         case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "\n<h2 class=\"zdoc\">", NULL); break;
         case paraTypeTITLE_3:
         case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "\n<h3 class=\"zdoc\">", NULL); break;
         case paraTypeTITLE_4:
         case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "\n<h4 class=\"zdoc\">", NULL); break;
         case paraTypeTITLE_5:
         case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "\n<h5 class=\"zdoc\">", NULL); break;
         case paraTypeTITLE_6:
         case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "\n<h6 class=\"zdoc\">", NULL); break;
         case paraTypeTITLE_7:
         case paraTypeTITLE_TOC_7: gfileSetA(file, gcTypeU1, "\n<p class=\"zdoch7\">", NULL); break;
         case paraTypeTITLE_8:
         case paraTypeTITLE_TOC_8: gfileSetA(file, gcTypeU1, "\n<p class=\"zdoch8\">", NULL); break;
         case paraTypeTITLE_9:
         case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "\n<p class=\"zdoch9\">", NULL); break;
         }

         // For titles with a table of contents line, add the anchor.
         if (paraTypeTITLE_TOC_1 <= para->type && para->type <= paraTypeTITLE_TOC_9)
         {
            chapter = _ChapterGetLink(para->chapter);
            gfileSetA(file, gcTypeU1, "<a name=\"", NULL);
            gfileSetS(file, gcTypeU1, chapter, NULL);
            gfileSetA(file, gcTypeU1, "\">", NULL);
            gsDestroy(chapter);
         }

         gfileSetS(file, gcTypeU1, para->str, NULL);

         if (paraTypeTITLE_TOC_1 <= para->type && para->type <= paraTypeTITLE_TOC_9)
         {
            gfileSetA(file, gcTypeU1, "</a>", NULL);
         }

         switch (para->type)
         {
         case paraTypeTITLE_1:
         case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "</h1>\n", NULL); break;
         case paraTypeTITLE_2:
         case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "</h2>\n", NULL); break;
         case paraTypeTITLE_3:     
         case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "</h3>\n", NULL); break;
         case paraTypeTITLE_4:     
         case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "</h4>\n", NULL); break;
         case paraTypeTITLE_5:     
         case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "</h5>\n", NULL); break;
         case paraTypeTITLE_6:     
         case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "</h6>\n", NULL); break;

         case paraTypeTITLE_7:
         case paraTypeTITLE_TOC_7: 
         case paraTypeTITLE_8:
         case paraTypeTITLE_TOC_8: 
         case paraTypeTITLE_9:
         case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "</p>\n", NULL); break;
         }
         break;

      case paraTypeLIST_START_BULLET:
         gfileSetA(file, gcTypeU1, "<ul class=\"zdoc\">\n", NULL);
         break;

      case paraTypeLIST_START_NUMBER:
         gfileSetA(file, gcTypeU1, "<ol class=\"zdoc\">\n", NULL);
         break;

      case paraTypeLIST_STOP_BULLET:
         if (isListItemActive)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "</ul>\n", NULL);
         isListItemActive = gbFALSE;
         break;

      case paraTypeLIST_STOP_NUMBER:
         if (isListItemActive)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "</ol>\n", NULL);
         isListItemActive = gbFALSE;
         break;

      case paraTypeLIST_ITEM:
         if (isListItemActive)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "<li class=\"zdoc\">", NULL);
         gfileSetS(file, gcTypeU1, para->str, NULL);
         gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         isListItemActive = gbFALSE;
         break;

      case paraTypeLIST_ITEM_START:
         if (isListItemActive)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "<li class=\"zdoc\">", NULL);
         isListItemActive = gbTRUE;
         break;

      case paraTypeLIST_ITEM_STOP:
         gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         isListItemActive = gbFALSE;
         break;

      case paraTypeTABLE_START:
         gfileSetA(file, gcTypeU1, "<table class=\"zdoc\"><tbody class=\"zdoc\">\n", NULL);
         isTableRowActive  = gbFALSE;
         isTableColActive  = gbFALSE;
         isTableColHActive = gbFALSE;
         break;

      case paraTypeTABLE_STOP:
         if (isTableColActive)
         {
            gfileSetA(file, gcTypeU1, "</td>\n", NULL);
         }
         if (isTableColHActive)
         {
            gfileSetA(file, gcTypeU1, "</th>\n", NULL);
         }
         if (isTableRowActive)
         {
            gfileSetA(file, gcTypeU1, "\n</tr>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "</tbody></table>\n", NULL);
         isTableRowActive = gbFALSE;
         isTableColActive = gbFALSE;
         break;

      case paraTypeTABLE_ROW:
         if (isTableColActive)
         {
            gfileSetA(file, gcTypeU1, "</td>\n", NULL);
         }
         if (isTableColHActive)
         {
            gfileSetA(file, gcTypeU1, "</th>\n", NULL);
         }
         if (isTableRowActive)
         {
            gfileSetA(file, gcTypeU1, "\n</tr>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "<tr class=\"zdoc\">\n", NULL);
         isTableRowActive  = gbTRUE;
         isTableColActive  = gbFALSE;
         isTableColHActive = gbFALSE;
         break;

      case paraTypeTABLE_COL_HEADER:
      case paraTypeTABLE_COL_HEADER_NO_BREAK:
      case paraTypeTABLE_COL_HEADER_FILL:
      case paraTypeTABLE_COL:
      case paraTypeTABLE_COL_NO_BREAK:
      case paraTypeTABLE_COL_FILL:
         if (isTableColHActive)
         {
            gfileSetA(file, gcTypeU1, "</th>\n", NULL);
         }
         if (isTableColActive)
         {
            gfileSetA(file, gcTypeU1, "</td>\n", NULL);
         }


         switch (para->type)
         {
         case paraTypeTABLE_COL_HEADER:            gfileSetA(file, gcTypeU1, "<th class=\"zdoc\">",       NULL); break;
         case paraTypeTABLE_COL_HEADER_NO_BREAK:   gfileSetA(file, gcTypeU1, "<th class=\"zdoc\"><nobr>", NULL); break;
         case paraTypeTABLE_COL_HEADER_FILL:       gfileSetA(file, gcTypeU1, "<th class=\"zdoc_fill\">",  NULL); break;
         case paraTypeTABLE_COL:                   gfileSetA(file, gcTypeU1, "<td class=\"zdoc\">",       NULL); break;
         case paraTypeTABLE_COL_NO_BREAK:          gfileSetA(file, gcTypeU1, "<td class=\"zdoc\"><nobr>", NULL); break;
         case paraTypeTABLE_COL_FILL:              gfileSetA(file, gcTypeU1, "<td class=\"zdoc_fill\">",  NULL); break;
         }
         gfileSetS(file, gcTypeU1, para->str, NULL);

         switch (para->type)
         {
         case paraTypeTABLE_COL_HEADER:            
         case paraTypeTABLE_COL_HEADER_FILL:       gfileSetA(file, gcTypeU1, "</th>\n",        NULL); break;
         case paraTypeTABLE_COL_HEADER_NO_BREAK:   gfileSetA(file, gcTypeU1, "</nobr></th>\n", NULL); break;
         case paraTypeTABLE_COL:            
         case paraTypeTABLE_COL_FILL:              gfileSetA(file, gcTypeU1, "</td>\n",        NULL); break;
         case paraTypeTABLE_COL_NO_BREAK:          gfileSetA(file, gcTypeU1, "</nobr></td>\n", NULL); break;
         }
         
         isTableColHActive = gbFALSE;
         break;

      case paraTypeFORMATED_START:
         gfileSetA(file, gcTypeU1, "<pre class=\"zdoc\">\n", NULL);
         gfileSetS(file, gcTypeU1, para->str, NULL);
         gfileSetA(file, gcTypeU1, "\n</pre>\n", NULL);
         break;

      case paraTypeTABLE_OF_CONTENTS:
         _WriteHTMLTOC(file, paraList);
         break;

      case paraTypePAGE_BREAK:
         break;
      }
   }

   // Write the terminators.
   gfileSetA(
      file,
      gcTypeU1,
      " </body>\n"
      "\n"
      "</html>\n",
      NULL);

   // Clean up.
   gfileClose(file);

   greturn gbTRUE;
}

/******************************************************************************
func: _WriteHTMLTOC
******************************************************************************/
static Gb _WriteHTMLTOC(Gfile * const file, ParaArray const * const paraList)
{
   Gindex     index;
   ParaArray *tocList;
   Para      *para;
   Gs        *chapter;

   genter;

   // Get the table of contents.
   tocList = _ParaListGetToc(paraList);

   // For all titles...
   forCount (index, paraArrayGetCount(tocList))
   {
      // Get the title.
      para = paraArrayGetAt(tocList, index);

      // Get the chapter link.
      chapter = _ChapterGetLink(para->chapter);

      switch (para->type)
      {
      case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc1\">",  NULL); break;
      case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc2\">",  NULL); break;
      case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc3\">",  NULL); break;
      case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc4\">",  NULL); break;
      case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc5\">",  NULL); break;
      case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc6\">",  NULL); break;
      case paraTypeTITLE_TOC_7: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc7\">",  NULL); break;
      case paraTypeTITLE_TOC_8: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc8\">",  NULL); break;
      case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "<p class=\"zdoc_toc9\">",  NULL); break;
      }

      gfileSetA(file, gcTypeU1, "<a href=\"#",              NULL);
      gfileSetS(file, gcTypeU1, chapter,                    NULL);
      gfileSetA(file, gcTypeU1, "\">",                      NULL);
      gfileSetS(file, gcTypeU1, para->str,                  NULL);
      gfileSetA(file, gcTypeU1, "</a></p>\n",               NULL);

      // Clean up
      gsDestroy(chapter);
   }

   // Clean up
   paraArrayDestroy(tocList);

   greturn gbTRUE;
}

/******************************************************************************
func: _WriteHTML
******************************************************************************/
static Gb _WriteMD(Gpath const * const path, ParaArray const * const paraList)
{
   Gpath *pathHtml;
   Gfile *file;
   Gindex index;
   Para  *para;
   Gb     isInsideListBullet,
          isInsideListNumber,
          isTableRowActive,
          isTableColHActive,
          isTableColActive,
          isListItemActive;

   genter;

   isInsideListBullet = gbFALSE;
   isInsideListNumber = gbFALSE;
   isTableRowActive   = gbFALSE;
   isTableColHActive  = gbFALSE;
   isTableColActive   = gbFALSE;
   isListItemActive   = gbFALSE;
      
   // Make the destination file path.
   pathHtml = gpathCreateFrom(path);
   gpathPopExtension(pathHtml);
   gpathPushExtensionA(pathHtml, "md");

   // Create the file.
   file = gfileOpen(pathHtml, gfileOpenModeREAD_WRITE_NEW);

   gpathDestroy(pathHtml);

   greturnFalseIf(!file);

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
         gfileSetA(file, gcTypeU1, "\n", NULL);
         gfileSetS(file, gcTypeU1, para->str, NULL);
         gfileSetA(file, gcTypeU1, "\n", NULL);
         break;

      case paraTypeTITLE_1:
      case paraTypeTITLE_2:
      case paraTypeTITLE_3:
      case paraTypeTITLE_4:
      case paraTypeTITLE_5:
      case paraTypeTITLE_6:
      case paraTypeTITLE_7:
      case paraTypeTITLE_8:
      case paraTypeTITLE_9:
      case paraTypeTITLE_TOC_1:
      case paraTypeTITLE_TOC_2:
      case paraTypeTITLE_TOC_3:
      case paraTypeTITLE_TOC_4:
      case paraTypeTITLE_TOC_5:
      case paraTypeTITLE_TOC_6:
      case paraTypeTITLE_TOC_7:
      case paraTypeTITLE_TOC_8:
      case paraTypeTITLE_TOC_9:
         switch (para->type)
         {
         case paraTypeTITLE_1:
         case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "\n# ", NULL); break;
         case paraTypeTITLE_2:
         case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "\n## ", NULL); break;
         case paraTypeTITLE_3:
         case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "\n### ", NULL); break;
         case paraTypeTITLE_4:
         case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "\n#### ", NULL); break;
         case paraTypeTITLE_5:
         case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "\n##### ", NULL); break;
         case paraTypeTITLE_6:
         case paraTypeTITLE_TOC_6: 
         case paraTypeTITLE_7:
         case paraTypeTITLE_TOC_7: 
         case paraTypeTITLE_8:
         case paraTypeTITLE_TOC_8: 
         case paraTypeTITLE_9:
         case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "\n###### ", NULL); break;
         }

         gfileSetS(file, gcTypeU1, para->str, NULL);

         gfileSetA(file, gcTypeU1, "\n", NULL);
         break;

      case paraTypeLIST_START_BULLET:
         isInsideListNumber = gbFALSE;
         isInsideListBullet = gbTRUE;
         break;

      case paraTypeLIST_START_NUMBER:
         isInsideListBullet = gbFALSE;
         isInsideListNumber = gbTRUE;
         break;

      case paraTypeLIST_STOP_BULLET:
      case paraTypeLIST_STOP_NUMBER:
         gfileSetA(file, gcTypeU1, "\n", NULL);
         isInsideListBullet = gbFALSE;
         isInsideListNumber = gbFALSE;
         break;

      case paraTypeLIST_ITEM:
         if (isInsideListBullet)
         {
            gfileSetA(file, gcTypeU1, "\n* ", NULL);
         }
         else
         {
            gfileSetA(file, gcTypeU1, "1. ", NULL);
         }
         gfileSetS(file, gcTypeU1, para->str, NULL);
         gfileSetA(file, gcTypeU1, "\n", NULL);
         break;

      case paraTypeTABLE_START:
         break;

      case paraTypeTABLE_STOP:
         gfileSetA(file, gcTypeU1, "\n", NULL);
         break;

      case paraTypeTABLE_ROW:
         if (isTableColActive || isTableColHActive)
         {
            gfileSetA(file, gcTypeU1, " |\n", NULL);
         }
         isTableColActive  = gbFALSE;
         isTableColHActive = gbFALSE;
         break;

      case paraTypeTABLE_COL_HEADER:
         if (isTableColHActive)
         {
            gfileSetA(file, gcTypeU1, " | ", NULL);
         }

         gfileSetA(file, gcTypeU1, "| ", NULL);
         isTableColHActive = gbTRUE;
         break;

      case paraTypeTABLE_COL:
         if (isTableColActive)
         {
            gfileSetA(file, gcTypeU1, " | ", NULL);
         }

         gfileSetA(file, gcTypeU1, "| ", NULL);
         isTableColActive = gbTRUE;
         break;

      case paraTypeFORMATED_START:
         gfileSetA(file, gcTypeU1, "\n```\n", NULL);
         gfileSetS(file, gcTypeU1, para->str, NULL);
         gfileSetA(file, gcTypeU1, "\n```\n", NULL);
         break;

      case paraTypeTABLE_OF_CONTENTS:
         _WriteMDTOC(file, paraList);
         break;

      case paraTypePAGE_BREAK:
         gfileSetA(file, gcTypeU1, "\n\n---\n\n", NULL);
         break;
      }
   }

   // Clean up.
   gfileClose(file);

   greturn gbTRUE;
}

/******************************************************************************
func: _WriteMDTOC
******************************************************************************/
static Gb _WriteMDTOC(Gfile * const file, ParaArray const * const paraList)
{
   Gindex     index;
   ParaArray *tocList;
   Para      *para;

   genter;

   // Get the table of contents.
   tocList = _ParaListGetToc(paraList);

   // For all titles...
   forCount (index, paraArrayGetCount(tocList))
   {
      // Get the title.
      para = paraArrayGetAt(tocList, index);

      switch (para->type)
      {
      case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "**",  NULL); break;
      case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, " ",  NULL); break;
      case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "  ",  NULL); break;
      case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "   ",  NULL); break;
      case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "    ",  NULL); break;
      case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "     ",  NULL); break;
      case paraTypeTITLE_TOC_7: gfileSetA(file, gcTypeU1, "      ",  NULL); break;
      case paraTypeTITLE_TOC_8: gfileSetA(file, gcTypeU1, "       ",  NULL); break;
      case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "        ",  NULL); break;
      }

      gfileSetS(file, gcTypeU1, para->str,                  NULL);

      switch (para->type)
      {
      case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "**",  NULL); break;
      }

      gfileSetA(file, gcTypeU1, "\n",  NULL);
   }

   // Clean up
   paraArrayDestroy(tocList);

   greturn gbTRUE;
}

/******************************************************************************
func: _WriteHTML
******************************************************************************/
static Gb _WriteZLYT(Gpath const * const path, ParaArray const * const paraList)
{
   genter;

   greturn gbTRUE;
}
