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
static Gs   *_ChapterGetString(  ParaChapter const chapter);

static Gs   *_ParaListGetTitle(  ParaArray const * const paraList);

static int   _Process(           Gs const * const command, Gpath const * const path);
static Gs   *_ProcessInlineHTML( Gs       * const str, Para const * const para);
static Gs   *_ProcessInlineMD(   Gs       * const str, Para const * const para);

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
                gsIsEqualU2(markup, L"=[")  ||
                gsIsEqualU2(markup, L"]=")  ||
                gsIsEqualU2(markup, L"#[")  ||
                gsIsEqualU2(markup, L"]#")  ||
                gsIsEqualU2(markup, L"-")   ||
                gsIsEqualU2(markup, L"-[")  ||
                gsIsEqualU2(markup, L"]-")  ||
                gsIsEqualU2(markup, L"t[")  ||
                gsIsEqualU2(markup, L"]t")  ||
                gsIsEqualU2(markup, L"t-")  ||
                gsIsEqualU2(markup, L"th")  ||
                gsIsEqualU2(markup, L"thx") ||
                gsIsEqualU2(markup, L"th*") ||
                gsIsEqualU2(markup, L"tc")  ||
                gsIsEqualU2(markup, L"tcx") ||
                gsIsEqualU2(markup, L"tc*") ||
                gsIsEqualU2(markup, L"[")   ||
                gsIsEqualU2(markup, L"[[")  ||
                gsIsEqualU2(markup, L"]]")  ||
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

            if (*gsGetAt(markup, 0) == L'*')
            {
               // Increment the chapter.
               chapter.value[position - 1]++;
   
               // Reset the subchapter counts.
               for (cindex = position; cindex < 10; cindex++)
               {
                  chapter.value[cindex] = 0;
               }
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
         else if (gsIsEqualU2(markup, L"tc"))
         {
            para->type     = paraTypeTABLE_COL;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"tcx"))
         {
            para->type     = paraTypeTABLE_COL_NO_BREAK;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc*"))
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
func: _ProcessInlineHTML

Process inline markup.
******************************************************************************/
static Gs *_ProcessInlineHTML(Gs * const inStr, Para const * const para)
{
   Gs *chapter,
      *str;

   genter;

   str = gsCreateFrom(inStr);

   // Chapter
   chapter = _ChapterGetString(para->chapter);

   gsFindAndReplaceU2(str, L"[chapter]",                   gsGet(chapter),         NULL);

   gsDestroy(chapter);

   // Styling
   gsFindAndReplaceU2(str, L"[img=]",                      L"<img src=\"",         NULL);
   gsFindAndReplaceU2(str, L"[=img]",                      L"\" />",               NULL);
   gsFindAndReplaceU2(str, L"[b=]",                        L"<strong>",            NULL);
   gsFindAndReplaceU2(str, L"[=b]",                        L"</strong>",           NULL);
   gsFindAndReplaceU2(str, L"[i=]",                        L"<em>",                NULL);
   gsFindAndReplaceU2(str, L"[=i]",                        L"</em>",               NULL);
   gsFindAndReplaceU2(str, L"[_=]",                        L"<u>",                 NULL);
   gsFindAndReplaceU2(str, L"[=_]",                        L"</u>",                NULL);
   gsFindAndReplaceU2(str, L"[-=]",                        L"<s>",                 NULL);
   gsFindAndReplaceU2(str, L"[=-]",                        L"</s>",                NULL);
   gsFindAndReplaceU2(str, L"[^=]",                        L"<sup>",               NULL);
   gsFindAndReplaceU2(str, L"[=^]",                        L"</sup>",              NULL);
   gsFindAndReplaceU2(str, L"[v=]",                        L"<sub>",               NULL);
   gsFindAndReplaceU2(str, L"[=v]",                        L"</sub>",              NULL);
   gsFindAndReplaceU2(str, L"[link=]",                     L"<a href=\"",          NULL);
   gsFindAndReplaceU2(str, L"[=link=]",                    L"\">",                 NULL);
   gsFindAndReplaceU2(str, L"[=link]",                     L"</a>",                NULL);
   gsFindAndReplaceU2(str, L"[line]",                      L"<br />",              NULL);
   gsFindAndReplaceU2(str, L"[line s]",                    L"<br /><br />",        NULL);
   gsFindAndReplaceU2(str, L"[line d]",                    L"<br /><br /><br />",  NULL);

   // Legal
   gsFindAndReplaceU2(str, L"[sym copyright]",             L"&copy;",              NULL);
   gsFindAndReplaceU2(str, L"[sym registered tm]",         L"&reg;",               NULL);
   gsFindAndReplaceU2(str, L"[sym tm]",                    L"&trade;",             NULL);

   // Currency
   gsFindAndReplaceU2(str, L"[sym bitcoin]",               L"&#x20BF;",            NULL);
   gsFindAndReplaceU2(str, L"[sym cent]",                  L"&cent;",              NULL);
   gsFindAndReplaceU2(str, L"[sym dollar]",                L"$",                   NULL);
   gsFindAndReplaceU2(str, L"[sym euro]",                  L"&euro;",              NULL);
   gsFindAndReplaceU2(str, L"[sym franc]",                 L"&#x20A3;",            NULL);
   gsFindAndReplaceU2(str, L"[sym lira]",                  L"&#x20A4;",            NULL);
   gsFindAndReplaceU2(str, L"[sym lira turkey]",           L"&#x20BA;",            NULL);
   gsFindAndReplaceU2(str, L"[sym peso]",                  L"&#x20B1;",            NULL);
   gsFindAndReplaceU2(str, L"[sym pound]",                 L"&pound;",             NULL);
   gsFindAndReplaceU2(str, L"[sym ruble]",                 L"&#x20BD;",            NULL);
   gsFindAndReplaceU2(str, L"[sym rupee]",                 L"&#x20A8;",            NULL);
   gsFindAndReplaceU2(str, L"[sym rupee india]",           L"&#x20B9;",            NULL);
   gsFindAndReplaceU2(str, L"[sym won]",                   L"&#x20A9;",            NULL);
   gsFindAndReplaceU2(str, L"[sym yen]",                   L"&yen;",               NULL);

   // Punctuation
   gsFindAndReplaceU2(str, L"[sym ...]",                   L"&hellip;",            NULL);
   gsFindAndReplaceU2(str, L"[sym 1/4]",                   L"&frac14;",            NULL);
   gsFindAndReplaceU2(str, L"[sym 1/2]",                   L"&frac12;",            NULL);
   gsFindAndReplaceU2(str, L"[sym 3/4]",                   L"&frac34;",            NULL);
   gsFindAndReplaceU2(str, L"[sym ampersand]",             L"&amp;",               NULL);
   gsFindAndReplaceU2(str, L"[sym at]",                    L"@",                   NULL);
   gsFindAndReplaceU2(str, L"[sym bullet]",                L"&bull;",              NULL);
   gsFindAndReplaceU2(str, L"[sym dagger s]",              L"&dagger;",            NULL);
   gsFindAndReplaceU2(str, L"[sym dagger d]",              L"&Dagger;",            NULL);
   gsFindAndReplaceU2(str, L"[sym hash]",                  L"#",                   NULL);
   gsFindAndReplaceU2(str, L"[sym inv!]",                  L"&iexcl;",             NULL);
   gsFindAndReplaceU2(str, L"[sym inv?]",                  L"&iquest;",            NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle d l]",       L"&laquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle d r]",       L"&raquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle s l]",       L"&lsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle s r]",       L"&rsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"[sym quote d]",               L"\"",                  NULL);
   gsFindAndReplaceU2(str, L"[sym quote d l]",             L"&ldquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote d r]",             L"&rdquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote d low]",           L"&bdquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote s]",               L"'",                   NULL);
   gsFindAndReplaceU2(str, L"[sym quote s l]",             L"&lsquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote s r]",             L"&rsquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote s low]",           L"&sbquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym para]",                  L"&para;",              NULL);
   gsFindAndReplaceU2(str, L"[sym prime d]",               L"&Prime;",             NULL);
   gsFindAndReplaceU2(str, L"[sym prime s]",               L"&prime;",             NULL);

   // Game
   gsFindAndReplaceU2(str, L"[sym arrow d]",               L"&darr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow l]",               L"&larr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow r]",               L"&rarr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow u]",               L"&uarr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow lr]",              L"&harr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow ud]",              L"&#x2195;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow \\d]",             L"&#x2198;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow /d]",              L"&#x2199;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow \\u]",             L"&#x2196;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow /u]",              L"&#x2197;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow d l]",             L"&lArr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow d lr]",            L"&hArr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow d r]",             L"&rArr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow redo]",            L"&#x21B7;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow undo]",            L"&#x21B6;",            NULL);
   gsFindAndReplaceU2(str, L"[sym card suit c]",           L"&clubs;",             NULL);
   gsFindAndReplaceU2(str, L"[sym card suit d]",           L"&diams;",             NULL);
   gsFindAndReplaceU2(str, L"[sym card suit h]",           L"&hearts;",            NULL);
   gsFindAndReplaceU2(str, L"[sym card suit s]",           L"&spades;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b k]",             L"&#x2654;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b q]",             L"&#x2655;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b r]",             L"&#x2656;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b b]",             L"&#x2657;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b h]",             L"&#x2658;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b p]",             L"&#x2659;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w k]",             L"&#x265A;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w q]",             L"&#x265B;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w r]",             L"&#x265C;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w b]",             L"&#x265D;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w h]",             L"&#x265E;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w p]",             L"&#x265F;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 1]",                 L"&#x2680;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 2]",                 L"&#x2681;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 3]",                 L"&#x2682;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 4]",                 L"&#x2683;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 5]",                 L"&#x2684;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 6]",                 L"&#x2685;",            NULL);
   gsFindAndReplaceU2(str, L"[sym ball baseball]",         L"&#x26BE;",            NULL);
   gsFindAndReplaceU2(str, L"[sym ball soccer]",           L"&#x26BD;",            NULL);

   // Symbols
   gsFindAndReplaceU2(str, L"[sym checkbox off]",          L"&#x2610;",            NULL);
   gsFindAndReplaceU2(str, L"[sym checkbox on]",           L"&#x2611;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 0%]",             L"&#x25CB;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 25%]",            L"&#x25D4;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 50%]",            L"&#x25D1;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 75%]",            L"&#x25D5;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 100%]",           L"&#x25CF;",            NULL);
   gsFindAndReplaceU2(str, L"[sym correct]",               L"&#x2713;",            NULL);
   gsFindAndReplaceU2(str, L"[sym biohazard]",             L"&#x2623;",            NULL);
   gsFindAndReplaceU2(str, L"[sym dot]",                   L"&#x26AA;",            NULL);
   gsFindAndReplaceU2(str, L"[sym dot filled]",            L"&#x26AB;",            NULL);
   gsFindAndReplaceU2(str, L"[sym envelope]",              L"&#x2709;",            NULL);
   gsFindAndReplaceU2(str, L"[sym gender f]",              L"&#x2640;",            NULL);
   gsFindAndReplaceU2(str, L"[sym gender m]",              L"&#x2642;",            NULL);
   gsFindAndReplaceU2(str, L"[sym gender mf]",             L"&#x26A5;",            NULL);
   gsFindAndReplaceU2(str, L"[sym incorrect]",             L"&#x2717;",            NULL);
   gsFindAndReplaceU2(str, L"[sym plane]",                 L"&#x2708;",            NULL);
   gsFindAndReplaceU2(str, L"[sym poison]",                L"&#x2620;",            NULL);
   gsFindAndReplaceU2(str, L"[sym radioactive]",           L"&#x2622;",            NULL);
   gsFindAndReplaceU2(str, L"[sym Rx]",                    L"&#x211E;",            NULL);
   gsFindAndReplaceU2(str, L"[sym recycle]",               L"&#x267B;",            NULL);
   gsFindAndReplaceU2(str, L"[sym scissor]",               L"&#x2702;",            NULL);
   gsFindAndReplaceU2(str, L"[sym snowflake]",             L"&#x2744;",            NULL);
   gsFindAndReplaceU2(str, L"[sym star 5]",                L"&#x2606;",            NULL);
   gsFindAndReplaceU2(str, L"[sym star 5 filled]",         L"&#x2605;",            NULL);
   gsFindAndReplaceU2(str, L"[sym sun]",                   L"&#x2600;",            NULL);

   // Math
   gsFindAndReplaceU2(str, L"[sym +/-]",                   L"&plusmn;",            NULL);
   gsFindAndReplaceU2(str, L"[sym angle]",                 L"&ang;",               NULL);
   gsFindAndReplaceU2(str, L"[sym contains]",              L"&ni;",                NULL);
   gsFindAndReplaceU2(str, L"[sym degree]",                L"&deg;",               NULL);
   gsFindAndReplaceU2(str, L"[sym dot]",                   L"&sdot;",              NULL);
   gsFindAndReplaceU2(str, L"[sym empty set]",             L"&empty;",             NULL);
   gsFindAndReplaceU2(str, L"[sym equal almost]",          L"&asymp;",             NULL);
   gsFindAndReplaceU2(str, L"[sym equal approx ]",         L"&cong;",              NULL);
   gsFindAndReplaceU2(str, L"[sym equal greater than]",    L"&ge;",                NULL);
   gsFindAndReplaceU2(str, L"[sym equal less than]",       L"&le;",                NULL);
   gsFindAndReplaceU2(str, L"[sym equal same]",            L"&equiv;",             NULL);
   gsFindAndReplaceU2(str, L"[sym equal not]",             L"&ne;",                NULL);
   gsFindAndReplaceU2(str, L"[sym for all]",               L"&forall;",            NULL);
   gsFindAndReplaceU2(str, L"[sym infinity]",              L"&infin;",             NULL);
   gsFindAndReplaceU2(str, L"[sym integral]",              L"&int;",               NULL);
   gsFindAndReplaceU2(str, L"[sym intersection]",          L"&cap;",               NULL);
   gsFindAndReplaceU2(str, L"[sym is in]",                 L"&isin;",              NULL);
   gsFindAndReplaceU2(str, L"[sym is not in]",             L"&notin;",             NULL);
   gsFindAndReplaceU2(str, L"[sym math phi]",              L"&straightphi;",       NULL);
   gsFindAndReplaceU2(str, L"[sym math pi]",               L"&piv;",               NULL);
   gsFindAndReplaceU2(str, L"[sym minus]",                 L"&minus;",             NULL);
   gsFindAndReplaceU2(str, L"[sym nabla]",                 L"&nabla;",             NULL);
   gsFindAndReplaceU2(str, L"[sym partial diff]",          L"&part;",              NULL);
   gsFindAndReplaceU2(str, L"[sym product]",               L"&prod;",              NULL);
   gsFindAndReplaceU2(str, L"[sym proportional to]",       L"&prop;",              NULL);
   gsFindAndReplaceU2(str, L"[sym root]",                  L"&radic;",             NULL);
   gsFindAndReplaceU2(str, L"[sym sum]",                   L"&sum;",               NULL);
   gsFindAndReplaceU2(str, L"[sym there exists]",          L"&exist;",             NULL);
   gsFindAndReplaceU2(str, L"[sym therefore]",             L"&there4;",            NULL);
   gsFindAndReplaceU2(str, L"[sym union]",                 L"&cup;",               NULL);

   // Greek
   gsFindAndReplaceU2(str, L"[sym alpha cap]",             L"&Alpha;",             NULL);
   gsFindAndReplaceU2(str, L"[sym beta cap]",              L"&Beta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym gamma cap]",             L"&Gamma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym delta cap]",             L"&Delta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym epsilon cap]",           L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym zeta cap]",              L"&Zeta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym eta cap]",               L"&Eta;",               NULL);
   gsFindAndReplaceU2(str, L"[sym theta cap]",             L"&Theta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym iota cap]",              L"&Iota;",              NULL);
   gsFindAndReplaceU2(str, L"[sym kappa cap]",             L"&Kappa;",             NULL);
   gsFindAndReplaceU2(str, L"[sym lambda cap]",            L"&Lambda;",            NULL);
   gsFindAndReplaceU2(str, L"[sym mu cap]",                L"&Mu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym nu cap]",                L"&Nu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym xi cap]",                L"&Xi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym omicron cap]",           L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"[sym pi cap]",                L"&Pi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym rho cap]",               L"&Rho;",               NULL);
   gsFindAndReplaceU2(str, L"[sym signma cap]",            L"&Sigma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym tau cap]",               L"&Tau;",               NULL);
   gsFindAndReplaceU2(str, L"[sym upsilon cap]",           L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym phi cap]",               L"&Phi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym chi cap]",               L"&Chi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym psi cap]",               L"&Psi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym omega cap]",             L"&Omega;",             NULL);
   gsFindAndReplaceU2(str, L"[sym alpha]",                 L"&alpha;",             NULL);
   gsFindAndReplaceU2(str, L"[sym beta]",                  L"&beta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym gamma]",                 L"&gamma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym delta]",                 L"&delta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym epsilon]",               L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym zeta]",                  L"&zeta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym eta]",                   L"&eta;",               NULL);
   gsFindAndReplaceU2(str, L"[sym theta]",                 L"&theta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym iota]",                  L"&iota;",              NULL);
   gsFindAndReplaceU2(str, L"[sym kappa]",                 L"&kappa;",             NULL);
   gsFindAndReplaceU2(str, L"[sym lambda]",                L"&lambda;",            NULL);
   gsFindAndReplaceU2(str, L"[sym mu]",                    L"&mu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym nu]",                    L"&nu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym xi]",                    L"&xi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym omicron]",               L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"[sym pi]",                    L"&pi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym rho]",                   L"&rho;",               NULL);
   gsFindAndReplaceU2(str, L"[sym signma]",                L"&sigma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym tau]",                   L"&tau;",               NULL);
   gsFindAndReplaceU2(str, L"[sym upsilon]",               L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym phi]",                   L"&phi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym chi]",                   L"&chi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym psi]",                   L"&psi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym omega]",                 L"&omega;",             NULL);

   gsFindAndReplaceU2(str, L"[]]",                         L"]",                   NULL);

   greturn str;
}

/******************************************************************************
func: _ProcessInlineMD

Process inline markup.
******************************************************************************/
static Gs *_ProcessInlineMD(Gs * const inStr, Para const * const para)
{
   Gs *chapter,
      *str;

   genter;

   str = gsCreateFrom(inStr);

   // Chapter
   chapter = _ChapterGetString(para->chapter);

   gsFindAndReplaceU2(str, L"[chapter]",                   gsGet(chapter),         NULL);

   gsDestroy(chapter);

   // Styling
   gsFindAndReplaceU2(str, L"[img=]",                      L"![image](",           NULL);
   gsFindAndReplaceU2(str, L"[=img]",                      L")",                   NULL);
   gsFindAndReplaceU2(str, L"[b=]",                        L"**",                  NULL);
   gsFindAndReplaceU2(str, L"[=b]",                        L"**",                  NULL);
   gsFindAndReplaceU2(str, L"[=i]",                        L"*",                   NULL);
   gsFindAndReplaceU2(str, L"[i=]",                        L"*",                   NULL);
   gsFindAndReplaceU2(str, L"[_=]",                        L"<u>",                 NULL);
   gsFindAndReplaceU2(str, L"[=_]",                        L"</u>",                NULL);
   gsFindAndReplaceU2(str, L"[-=]",                        L"~~",                  NULL);
   gsFindAndReplaceU2(str, L"[=-]",                        L"~~",                  NULL);
   gsFindAndReplaceU2(str, L"[^=]",                        L"<sup>",               NULL);
   gsFindAndReplaceU2(str, L"[=^]",                        L"</sup>",              NULL);
   gsFindAndReplaceU2(str, L"[v=]",                        L"<sub>",               NULL);
   gsFindAndReplaceU2(str, L"[=v]",                        L"</sub>",              NULL);
   gsFindAndReplaceU2(str, L"[link=]",                     L"[LINK](",             NULL);
   gsFindAndReplaceU2(str, L"[=link=]",                    L") ",                  NULL);
   gsFindAndReplaceU2(str, L"[=link]",                     L"",                    NULL);
   gsFindAndReplaceU2(str, L"[line]",                      L" ",                   NULL);
   gsFindAndReplaceU2(str, L"[line s]",                    L" ",                   NULL);
   gsFindAndReplaceU2(str, L"[line d]",                    L" ",                   NULL);

   // Legal
   gsFindAndReplaceU2(str, L"[sym copyright]",             L"&copy;",              NULL);
   gsFindAndReplaceU2(str, L"[sym registered tm]",         L"&reg;",               NULL);
   gsFindAndReplaceU2(str, L"[sym tm]",                    L"&trade;",             NULL);

   // Currency
   gsFindAndReplaceU2(str, L"[sym bitcoin]",               L"&#x20BF;",            NULL);
   gsFindAndReplaceU2(str, L"[sym cent]",                  L"&cent;",              NULL);
   gsFindAndReplaceU2(str, L"[sym dollar]",                L"$",                   NULL);
   gsFindAndReplaceU2(str, L"[sym euro]",                  L"&euro;",              NULL);
   gsFindAndReplaceU2(str, L"[sym franc]",                 L"&#x20A3;",            NULL);
   gsFindAndReplaceU2(str, L"[sym lira]",                  L"&#x20A4;",            NULL);
   gsFindAndReplaceU2(str, L"[sym lira turkey]",           L"&#x20BA;",            NULL);
   gsFindAndReplaceU2(str, L"[sym peso]",                  L"&#x20B1;",            NULL);
   gsFindAndReplaceU2(str, L"[sym pound]",                 L"&pound;",             NULL);
   gsFindAndReplaceU2(str, L"[sym ruble]",                 L"&#x20BD;",            NULL);
   gsFindAndReplaceU2(str, L"[sym rupee]",                 L"&#x20A8;",            NULL);
   gsFindAndReplaceU2(str, L"[sym rupee india]",           L"&#x20B9;",            NULL);
   gsFindAndReplaceU2(str, L"[sym won]",                   L"&#x20A9;",            NULL);
   gsFindAndReplaceU2(str, L"[sym yen]",                   L"&yen;",               NULL);

   // Punctuation
   gsFindAndReplaceU2(str, L"[sym ...]",                   L"&hellip;",            NULL);
   gsFindAndReplaceU2(str, L"[sym 1/4]",                   L"&frac14;",            NULL);
   gsFindAndReplaceU2(str, L"[sym 1/2]",                   L"&frac12;",            NULL);
   gsFindAndReplaceU2(str, L"[sym 3/4]",                   L"&frac34;",            NULL);
   gsFindAndReplaceU2(str, L"[sym ampersand]",             L"&amp;",               NULL);
   gsFindAndReplaceU2(str, L"[sym at]",                    L"@",                   NULL);
   gsFindAndReplaceU2(str, L"[sym bullet]",                L"&bull;",              NULL);
   gsFindAndReplaceU2(str, L"[sym dagger s]",              L"&dagger;",            NULL);
   gsFindAndReplaceU2(str, L"[sym dagger d]",              L"&Dagger;",            NULL);
   gsFindAndReplaceU2(str, L"[sym hash]",                  L"#",                   NULL);
   gsFindAndReplaceU2(str, L"[sym inv!]",                  L"&iexcl;",             NULL);
   gsFindAndReplaceU2(str, L"[sym inv?]",                  L"&iquest;",            NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle d l]",       L"&laquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle d r]",       L"&raquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle s l]",       L"&lsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"[sym quote angle s r]",       L"&rsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"[sym quote d]",               L"\"",                  NULL);
   gsFindAndReplaceU2(str, L"[sym quote d l]",             L"&ldquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote d r]",             L"&rdquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote d low]",           L"&bdquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote s]",               L"'",                   NULL);
   gsFindAndReplaceU2(str, L"[sym quote s l]",             L"&lsquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote s r]",             L"&rsquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym quote s low]",           L"&sbquo;",             NULL);
   gsFindAndReplaceU2(str, L"[sym para]",                  L"&para;",              NULL);
   gsFindAndReplaceU2(str, L"[sym prime d]",               L"&Prime;",             NULL);
   gsFindAndReplaceU2(str, L"[sym prime s]",               L"&prime;",             NULL);

   // Game
   gsFindAndReplaceU2(str, L"[sym arrow d]",               L"&darr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow l]",               L"&larr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow r]",               L"&rarr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow u]",               L"&uarr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow lr]",              L"&harr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow ud]",              L"&#x2195;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow \\d]",             L"&#x2198;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow /d]",              L"&#x2199;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow \\u]",             L"&#x2196;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow /u]",              L"&#x2197;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow d l]",             L"&lArr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow d lr]",            L"&hArr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow d r]",             L"&rArr;",              NULL);
   gsFindAndReplaceU2(str, L"[sym arrow redo]",            L"&#x21B7;",            NULL);
   gsFindAndReplaceU2(str, L"[sym arrow undo]",            L"&#x21B6;",            NULL);
   gsFindAndReplaceU2(str, L"[sym card suit c]",           L"&clubs;",             NULL);
   gsFindAndReplaceU2(str, L"[sym card suit d]",           L"&diams;",             NULL);
   gsFindAndReplaceU2(str, L"[sym card suit h]",           L"&hearts;",            NULL);
   gsFindAndReplaceU2(str, L"[sym card suit s]",           L"&spades;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b k]",             L"&#x2654;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b q]",             L"&#x2655;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b r]",             L"&#x2656;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b b]",             L"&#x2657;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b h]",             L"&#x2658;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess b p]",             L"&#x2659;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w k]",             L"&#x265A;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w q]",             L"&#x265B;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w r]",             L"&#x265C;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w b]",             L"&#x265D;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w h]",             L"&#x265E;",            NULL);
   gsFindAndReplaceU2(str, L"[sym chess w p]",             L"&#x265F;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 1]",                 L"&#x2680;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 2]",                 L"&#x2681;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 3]",                 L"&#x2682;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 4]",                 L"&#x2683;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 5]",                 L"&#x2684;",            NULL);
   gsFindAndReplaceU2(str, L"[sym die 6]",                 L"&#x2685;",            NULL);
   gsFindAndReplaceU2(str, L"[sym ball baseball]",         L"&#x26BE;",            NULL);
   gsFindAndReplaceU2(str, L"[sym ball soccer]",           L"&#x26BD;",            NULL);

   // Symbols
   gsFindAndReplaceU2(str, L"[sym checkbox off]",          L"&#x2610;",            NULL);
   gsFindAndReplaceU2(str, L"[sym checkbox on]",           L"&#x2611;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 0%]",             L"&#x25CB;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 25%]",            L"&#x25D4;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 50%]",            L"&#x25D1;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 75%]",            L"&#x25D5;",            NULL);
   gsFindAndReplaceU2(str, L"[sym circle 100%]",           L"&#x25CF;",            NULL);
   gsFindAndReplaceU2(str, L"[sym correct]",               L"&#x2713;",            NULL);
   gsFindAndReplaceU2(str, L"[sym biohazard]",             L"&#x2623;",            NULL);
   gsFindAndReplaceU2(str, L"[sym dot]",                   L"&#x26AA;",            NULL);
   gsFindAndReplaceU2(str, L"[sym dot filled]",            L"&#x26AB;",            NULL);
   gsFindAndReplaceU2(str, L"[sym envelope]",              L"&#x2709;",            NULL);
   gsFindAndReplaceU2(str, L"[sym gender f]",              L"&#x2640;",            NULL);
   gsFindAndReplaceU2(str, L"[sym gender m]",              L"&#x2642;",            NULL);
   gsFindAndReplaceU2(str, L"[sym gender mf]",             L"&#x26A5;",            NULL);
   gsFindAndReplaceU2(str, L"[sym incorrect]",             L"&#x2717;",            NULL);
   gsFindAndReplaceU2(str, L"[sym plane]",                 L"&#x2708;",            NULL);
   gsFindAndReplaceU2(str, L"[sym poison]",                L"&#x2620;",            NULL);
   gsFindAndReplaceU2(str, L"[sym radioactive]",           L"&#x2622;",            NULL);
   gsFindAndReplaceU2(str, L"[sym Rx]",                    L"&#x211E;",            NULL);
   gsFindAndReplaceU2(str, L"[sym recycle]",               L"&#x267B;",            NULL);
   gsFindAndReplaceU2(str, L"[sym scissor]",               L"&#x2702;",            NULL);
   gsFindAndReplaceU2(str, L"[sym snowflake]",             L"&#x2744;",            NULL);
   gsFindAndReplaceU2(str, L"[sym star 5]",                L"&#x2606;",            NULL);
   gsFindAndReplaceU2(str, L"[sym star 5 filled]",         L"&#x2605;",            NULL);
   gsFindAndReplaceU2(str, L"[sym sun]",                   L"&#x2600;",            NULL);

   // Math
   gsFindAndReplaceU2(str, L"[sym +/-]",                   L"&plusmn;",            NULL);
   gsFindAndReplaceU2(str, L"[sym angle]",                 L"&ang;",               NULL);
   gsFindAndReplaceU2(str, L"[sym contains]",              L"&ni;",                NULL);
   gsFindAndReplaceU2(str, L"[sym degree]",                L"&deg;",               NULL);
   gsFindAndReplaceU2(str, L"[sym dot]",                   L"&sdot;",              NULL);
   gsFindAndReplaceU2(str, L"[sym empty set]",             L"&empty;",             NULL);
   gsFindAndReplaceU2(str, L"[sym equal almost]",          L"&asymp;",             NULL);
   gsFindAndReplaceU2(str, L"[sym equal approx ]",         L"&cong;",              NULL);
   gsFindAndReplaceU2(str, L"[sym equal greater than]",    L"&ge;",                NULL);
   gsFindAndReplaceU2(str, L"[sym equal less than]",       L"&le;",                NULL);
   gsFindAndReplaceU2(str, L"[sym equal same]",            L"&equiv;",             NULL);
   gsFindAndReplaceU2(str, L"[sym equal not]",             L"&ne;",                NULL);
   gsFindAndReplaceU2(str, L"[sym for all]",               L"&forall;",            NULL);
   gsFindAndReplaceU2(str, L"[sym infinity]",              L"&infin;",             NULL);
   gsFindAndReplaceU2(str, L"[sym integral]",              L"&int;",               NULL);
   gsFindAndReplaceU2(str, L"[sym intersection]",          L"&cap;",               NULL);
   gsFindAndReplaceU2(str, L"[sym is in]",                 L"&isin;",              NULL);
   gsFindAndReplaceU2(str, L"[sym is not in]",             L"&notin;",             NULL);
   gsFindAndReplaceU2(str, L"[sym math phi]",              L"&straightphi;",       NULL);
   gsFindAndReplaceU2(str, L"[sym math pi]",               L"&piv;",               NULL);
   gsFindAndReplaceU2(str, L"[sym minus]",                 L"&minus;",             NULL);
   gsFindAndReplaceU2(str, L"[sym nabla]",                 L"&nabla;",             NULL);
   gsFindAndReplaceU2(str, L"[sym partial diff]",          L"&part;",              NULL);
   gsFindAndReplaceU2(str, L"[sym product]",               L"&prod;",              NULL);
   gsFindAndReplaceU2(str, L"[sym proportional to]",       L"&prop;",              NULL);
   gsFindAndReplaceU2(str, L"[sym root]",                  L"&radic;",             NULL);
   gsFindAndReplaceU2(str, L"[sym sum]",                   L"&sum;",               NULL);
   gsFindAndReplaceU2(str, L"[sym there exists]",          L"&exist;",             NULL);
   gsFindAndReplaceU2(str, L"[sym therefore]",             L"&there4;",            NULL);
   gsFindAndReplaceU2(str, L"[sym union]",                 L"&cup;",               NULL);

   // Greek
   gsFindAndReplaceU2(str, L"[sym alpha cap]",             L"&Alpha;",             NULL);
   gsFindAndReplaceU2(str, L"[sym beta cap]",              L"&Beta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym gamma cap]",             L"&Gamma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym delta cap]",             L"&Delta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym epsilon cap]",           L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym zeta cap]",              L"&Zeta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym eta cap]",               L"&Eta;",               NULL);
   gsFindAndReplaceU2(str, L"[sym theta cap]",             L"&Theta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym iota cap]",              L"&Iota;",              NULL);
   gsFindAndReplaceU2(str, L"[sym kappa cap]",             L"&Kappa;",             NULL);
   gsFindAndReplaceU2(str, L"[sym lambda cap]",            L"&Lambda;",            NULL);
   gsFindAndReplaceU2(str, L"[sym mu cap]",                L"&Mu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym nu cap]",                L"&Nu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym xi cap]",                L"&Xi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym omicron cap]",           L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"[sym pi cap]",                L"&Pi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym rho cap]",               L"&Rho;",               NULL);
   gsFindAndReplaceU2(str, L"[sym signma cap]",            L"&Sigma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym tau cap]",               L"&Tau;",               NULL);
   gsFindAndReplaceU2(str, L"[sym upsilon cap]",           L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym phi cap]",               L"&Phi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym chi cap]",               L"&Chi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym psi cap]",               L"&Psi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym omega cap]",             L"&Omega;",             NULL);
   gsFindAndReplaceU2(str, L"[sym alpha]",                 L"&alpha;",             NULL);
   gsFindAndReplaceU2(str, L"[sym beta]",                  L"&beta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym gamma]",                 L"&gamma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym delta]",                 L"&delta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym epsilon]",               L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym zeta]",                  L"&zeta;",              NULL);
   gsFindAndReplaceU2(str, L"[sym eta]",                   L"&eta;",               NULL);
   gsFindAndReplaceU2(str, L"[sym theta]",                 L"&theta;",             NULL);
   gsFindAndReplaceU2(str, L"[sym iota]",                  L"&iota;",              NULL);
   gsFindAndReplaceU2(str, L"[sym kappa]",                 L"&kappa;",             NULL);
   gsFindAndReplaceU2(str, L"[sym lambda]",                L"&lambda;",            NULL);
   gsFindAndReplaceU2(str, L"[sym mu]",                    L"&mu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym nu]",                    L"&nu;",                NULL);
   gsFindAndReplaceU2(str, L"[sym xi]",                    L"&xi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym omicron]",               L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"[sym pi]",                    L"&pi;",                NULL);
   gsFindAndReplaceU2(str, L"[sym rho]",                   L"&rho;",               NULL);
   gsFindAndReplaceU2(str, L"[sym signma]",                L"&sigma;",             NULL);
   gsFindAndReplaceU2(str, L"[sym tau]",                   L"&tau;",               NULL);
   gsFindAndReplaceU2(str, L"[sym upsilon]",               L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"[sym phi]",                   L"&phi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym chi]",                   L"&chi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym psi]",                   L"&psi;",               NULL);
   gsFindAndReplaceU2(str, L"[sym omega]",                 L"&omega;",             NULL);

   gsFindAndReplaceU2(str, L"[]]",                         L"\\]",                 NULL);

   greturn str;
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
   Gs    *title,
         *chapter,
         *stemp;
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
         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetA(file, gcTypeU1, "<p class=\"zdoc\">", NULL);
         gfileSetS(file, gcTypeU1, stemp,                NULL);
         gfileSetA(file, gcTypeU1, "</p>\n",             NULL);
         gsDestroy(stemp);
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
            chapter = _ChapterGetString(para->chapter);
            gfileSetA(file, gcTypeU1, "<a name=\"", NULL);
            gfileSetS(file, gcTypeU1, chapter, NULL);
            gfileSetA(file, gcTypeU1, "\">", NULL);
            gsDestroy(chapter);
         }

         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

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

         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetA(file, gcTypeU1, "<li class=\"zdoc\">", NULL);
         gfileSetS(file, gcTypeU1, stemp,                 NULL);
         gfileSetA(file, gcTypeU1, "</li>\n",             NULL);
         gsDestroy(stemp);
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

         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

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
   Gs        *chapter,
             *stemp;

   genter;

   // Get the table of contents.
   tocList = _ParaListGetToc(paraList);

   // For all titles...
   forCount (index, paraArrayGetCount(tocList))
   {
      // Get the title.
      para = paraArrayGetAt(tocList, index);

      // Get the chapter link.
      chapter = _ChapterGetString(para->chapter);

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

      stemp = _ProcessInlineHTML(para->str, para);
      gfileSetA(file, gcTypeU1, "<a href=\"#", NULL);
      gfileSetS(file, gcTypeU1, chapter,       NULL);
      gfileSetA(file, gcTypeU1, "\">",         NULL);
      gfileSetS(file, gcTypeU1, stemp,         NULL);
      gfileSetA(file, gcTypeU1, "</a></p>\n",  NULL);
      gsDestroy(stemp);

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
   Gindex index,
          hindex;
   Para  *para;
   Gs    *stemp;
   Gcount tableHeaderCount;
   Gb     isInsideListBullet,
          isInsideListNumber,
          isTableRowActive,
          isTableColHActive,
          isTableColActive,
          isTableHeaderLineWritten,
          isListItemActive;

   genter;

   tableHeaderCount         = 0;
   isTableHeaderLineWritten = gbFALSE;
   isInsideListBullet       = gbFALSE;
   isInsideListNumber       = gbFALSE;
   isTableRowActive         = gbFALSE;
   isTableColHActive        = gbFALSE;
   isTableColActive         = gbFALSE;
   isListItemActive         = gbFALSE;
      
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
         stemp = _ProcessInlineMD(para->str, para);
         gfileSetA(file, gcTypeU1, "\n",  NULL);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gfileSetA(file, gcTypeU1, "\n",  NULL);
         gsDestroy(stemp);
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

         stemp = _ProcessInlineMD(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

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
         stemp = _ProcessInlineMD(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gfileSetA(file, gcTypeU1, "\n",  NULL);
         gsDestroy(stemp);
         break;

      case paraTypeTABLE_START:
         gfileSetA(file, gcTypeU1, "\n", NULL);
         tableHeaderCount         = 0;
         isTableHeaderLineWritten = gbFALSE;
         break;

      case paraTypeTABLE_STOP:
         if (isTableColActive || isTableColHActive)
         {
            gfileSetA(file, gcTypeU1, " |\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "\n", NULL);
         isTableColActive  = gbFALSE;
         isTableColHActive = gbFALSE;
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
      case paraTypeTABLE_COL_HEADER_NO_BREAK:
      case paraTypeTABLE_COL_HEADER_FILL:
         if (isTableColHActive)
         {
            gfileSetA(file, gcTypeU1, " | ", NULL);
         }
         else 
         {
            gfileSetA(file, gcTypeU1, "| ", NULL);
         }
         stemp = _ProcessInlineMD(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

         isTableColHActive = gbTRUE;

         tableHeaderCount++;
         break;

      case paraTypeTABLE_COL:
      case paraTypeTABLE_COL_NO_BREAK:
      case paraTypeTABLE_COL_FILL:
         if (!isTableHeaderLineWritten)
         {
            forCount (hindex, tableHeaderCount)
            {
               gfileSetA(file, gcTypeU1, "| --- ", NULL);
            }
            gfileSetA(file, gcTypeU1, "|\n", NULL);
         }
         isTableHeaderLineWritten = gbTRUE;

         if (isTableColActive)
         {
            gfileSetA(file, gcTypeU1, " | ", NULL);
         }
         else 
         {
            gfileSetA(file, gcTypeU1, "| ", NULL);
         }
         stemp = _ProcessInlineMD(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);
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
   Gs        *stemp;

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

      stemp = _ProcessInlineMD(para->str, para);
      gfileSetS(file, gcTypeU1, stemp, NULL);
      gsDestroy(stemp);

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
