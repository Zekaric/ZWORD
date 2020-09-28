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
   listTypeBULLET,
   listTypeNUMBER
} ListType;

typedef enum
{
   listActiveNO,
   listActiveYES_SINGLE_LINE,
   listActiveYES
} ListActive;

typedef enum
{
   paraTypeNONE,

   paraTypeREGULAR,

   paraTypeKEY_VALUE,

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
   
   paraTypeLIST_ITEM_BULLET_1,
   paraTypeLIST_ITEM_BULLET_2,
   paraTypeLIST_ITEM_BULLET_3,
   paraTypeLIST_ITEM_BULLET_4,
   paraTypeLIST_ITEM_BULLET_5,
   paraTypeLIST_ITEM_BULLET_6,
   paraTypeLIST_ITEM_BULLET_7,
   paraTypeLIST_ITEM_BULLET_8,
   paraTypeLIST_ITEM_BULLET_9,
   paraTypeLIST_ITEM_BULLET_START_1,
   paraTypeLIST_ITEM_BULLET_START_2,
   paraTypeLIST_ITEM_BULLET_START_3,
   paraTypeLIST_ITEM_BULLET_START_4,
   paraTypeLIST_ITEM_BULLET_START_5,
   paraTypeLIST_ITEM_BULLET_START_6,
   paraTypeLIST_ITEM_BULLET_START_7,
   paraTypeLIST_ITEM_BULLET_START_8,
   paraTypeLIST_ITEM_BULLET_START_9,
   paraTypeLIST_ITEM_BULLET_STOP_1,
   paraTypeLIST_ITEM_BULLET_STOP_2,
   paraTypeLIST_ITEM_BULLET_STOP_3,
   paraTypeLIST_ITEM_BULLET_STOP_4,
   paraTypeLIST_ITEM_BULLET_STOP_5,
   paraTypeLIST_ITEM_BULLET_STOP_6,
   paraTypeLIST_ITEM_BULLET_STOP_7,
   paraTypeLIST_ITEM_BULLET_STOP_8,
   paraTypeLIST_ITEM_BULLET_STOP_9,
   paraTypeLIST_ITEM_NUMBER_1,
   paraTypeLIST_ITEM_NUMBER_2,
   paraTypeLIST_ITEM_NUMBER_3,
   paraTypeLIST_ITEM_NUMBER_4,
   paraTypeLIST_ITEM_NUMBER_5,
   paraTypeLIST_ITEM_NUMBER_6,
   paraTypeLIST_ITEM_NUMBER_7,
   paraTypeLIST_ITEM_NUMBER_8,
   paraTypeLIST_ITEM_NUMBER_9,
   paraTypeLIST_ITEM_NUMBER_START_1,
   paraTypeLIST_ITEM_NUMBER_START_2,
   paraTypeLIST_ITEM_NUMBER_START_3,
   paraTypeLIST_ITEM_NUMBER_START_4,
   paraTypeLIST_ITEM_NUMBER_START_5,
   paraTypeLIST_ITEM_NUMBER_START_6,
   paraTypeLIST_ITEM_NUMBER_START_7,
   paraTypeLIST_ITEM_NUMBER_START_8,
   paraTypeLIST_ITEM_NUMBER_START_9,
   paraTypeLIST_ITEM_NUMBER_STOP_1,
   paraTypeLIST_ITEM_NUMBER_STOP_2,
   paraTypeLIST_ITEM_NUMBER_STOP_3,
   paraTypeLIST_ITEM_NUMBER_STOP_4,
   paraTypeLIST_ITEM_NUMBER_STOP_5,
   paraTypeLIST_ITEM_NUMBER_STOP_6,
   paraTypeLIST_ITEM_NUMBER_STOP_7,
   paraTypeLIST_ITEM_NUMBER_STOP_8,
   paraTypeLIST_ITEM_NUMBER_STOP_9,

   paraTypeTABLE_START,
   paraTypeTABLE_STOP,
   paraTypeTABLE_ROW,
   paraTypeTABLE_COL_HEADER,
   paraTypeTABLE_COL_HEADER_NO_BREAK,
   paraTypeTABLE_COL_HEADER_FILL,
   paraTypeTABLE_COL,
   paraTypeTABLE_COL_NO_BREAK,
   paraTypeTABLE_COL_FILL,
   paraTypeTABLE_COL_NUMBER,
   paraTypeTABLE_COL_NUMBER_FILL,

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
static Gs   *_ChapterGetString(        ParaChapter const chapter);

static Gs   *_ParaListGetTitle(        ParaArray const * const paraList);

static int   _Process(                 Gs const * const command, Gpath const * const path);
static Gs   *_ProcessInlineHTML(       Gs       * const str, Para const * const para);
static Gs   *_ProcessInlineMD(         Gs       * const str, Para const * const para);
static Gs   *_ProcessInlineRTF(        Gs       * const str, Para const * const para);

static Gb    _WriteHTML(               Gpath const * const path, ParaArray const * const paraList);
static void  _WriteHTMLPopList(        Gfile       * const file, Gb const isForced, Gindex const listLevel, ListType * const listType, ListActive * const isListActive);
static Gb    _WriteHTMLTOC(            Gfile       * const file, ParaArray const * const paraList);
static Gb    _WriteMD(                 Gpath const * const path, ParaArray const * const paraList);
static Gb    _WriteMDTOC(              Gfile       * const file, ParaArray const * const paraList);
static Gb    _WriteRTF(                Gpath const * const path, ParaArray const * const paraList);
static Gb    _WriteRTFTOC(             Gfile       * const file, ParaArray const * const paraList);
static Gb    _WriteZLYT(               Gpath const * const path, ParaArray const * const paraList);

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
      "Z:WORD\n"
      "========\n"
      "\n");

   // Print the help.
   if (acount <= 1)
   {
      gconSetA(
         "zword [command] [file]\n"
         "\n"
         "[command] = \n"
         "   \"all\"        convert file to all the formats.\n"
         "   \"confluence\" convert file to HTML format.\n"
         "   \"html\"       convert file to HTML format.\n"
         "   \"md\"         convert file to MD markup.\n"
         // future "   \"PDF\"  convert file to PDF format.\n"
         "   \"rtf\"        convert file to RTF format.\n"
         "   \"zlyt\"       convert file to Z:LYT markup.\n"
         "\n"
         "[file]    =\n"
         "   Fully qualified file path.  Including .zword extension.\n"
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
            if (gsIsEqualU2(markup, L"h")          ||
                gsIsEqualU2(markup, L"hh")         ||
                gsIsEqualU2(markup, L"hhh")        ||
                gsIsEqualU2(markup, L"hhhh")       ||
                gsIsEqualU2(markup, L"hhhhh")      ||
                gsIsEqualU2(markup, L"hhhhhh")     ||
                gsIsEqualU2(markup, L"hhhhhhh")    ||
                gsIsEqualU2(markup, L"hhhhhhhh")   ||
                gsIsEqualU2(markup, L"hhhhhhhhh")  ||
                gsIsEqualU2(markup, L"H")          ||
                gsIsEqualU2(markup, L"HH")         ||
                gsIsEqualU2(markup, L"HHH")        ||
                gsIsEqualU2(markup, L"HHHH")       ||
                gsIsEqualU2(markup, L"HHHHH")      ||
                gsIsEqualU2(markup, L"HHHHHH")     ||
                gsIsEqualU2(markup, L"HHHHHHH")    ||
                gsIsEqualU2(markup, L"HHHHHHHH")   ||
                gsIsEqualU2(markup, L"HHHHHHHHH")  ||
                gsIsEqualU2(markup, L"-")          ||
                gsIsEqualU2(markup, L".-")         ||
                gsIsEqualU2(markup, L"..-")        ||
                gsIsEqualU2(markup, L"...-")       ||
                gsIsEqualU2(markup, L"....-")      ||
                gsIsEqualU2(markup, L".....-")     ||
                gsIsEqualU2(markup, L"......-")    ||
                gsIsEqualU2(markup, L".......-")   ||
                gsIsEqualU2(markup, L"........-")  ||
                gsIsEqualU2(markup, L"-[")         ||
                gsIsEqualU2(markup, L".-[")        ||
                gsIsEqualU2(markup, L"..-[")       ||
                gsIsEqualU2(markup, L"...-[")      ||
                gsIsEqualU2(markup, L"....-[")     ||
                gsIsEqualU2(markup, L".....-[")    ||
                gsIsEqualU2(markup, L"......-[")   ||
                gsIsEqualU2(markup, L".......-[")  ||
                gsIsEqualU2(markup, L"........-[") ||
                gsIsEqualU2(markup, L"-]")         ||
                gsIsEqualU2(markup, L".-]")        ||
                gsIsEqualU2(markup, L"..-]")       ||
                gsIsEqualU2(markup, L"...-]")      ||
                gsIsEqualU2(markup, L"....-]")     ||
                gsIsEqualU2(markup, L".....-]")    ||
                gsIsEqualU2(markup, L"......-]")   ||
                gsIsEqualU2(markup, L".......-]")  ||
                gsIsEqualU2(markup, L"........-]") ||
                gsIsEqualU2(markup, L"1")          ||
                gsIsEqualU2(markup, L".1")         ||
                gsIsEqualU2(markup, L"..1")        ||
                gsIsEqualU2(markup, L"...1")       ||
                gsIsEqualU2(markup, L"....1")      ||
                gsIsEqualU2(markup, L".....1")     ||
                gsIsEqualU2(markup, L"......1")    ||
                gsIsEqualU2(markup, L".......1")   ||
                gsIsEqualU2(markup, L"........1")  ||
                gsIsEqualU2(markup, L"1[")         ||
                gsIsEqualU2(markup, L".1[")        ||
                gsIsEqualU2(markup, L"..1[")       ||
                gsIsEqualU2(markup, L"...1[")      ||
                gsIsEqualU2(markup, L"....1[")     ||
                gsIsEqualU2(markup, L".....1[")    ||
                gsIsEqualU2(markup, L"......1[")   ||
                gsIsEqualU2(markup, L".......1[")  ||
                gsIsEqualU2(markup, L"........1[") ||
                gsIsEqualU2(markup, L"1]")         ||
                gsIsEqualU2(markup, L".1]")        ||
                gsIsEqualU2(markup, L"..1]")       ||
                gsIsEqualU2(markup, L"...1]")      ||
                gsIsEqualU2(markup, L"....1]")     ||
                gsIsEqualU2(markup, L".....1]")    ||
                gsIsEqualU2(markup, L"......1]")   ||
                gsIsEqualU2(markup, L".......1]")  ||
                gsIsEqualU2(markup, L"........1]") ||
                gsIsEqualU2(markup, L"kv")         ||
                gsIsEqualU2(markup, L"t[")         ||
                gsIsEqualU2(markup, L"t]")         ||
                gsIsEqualU2(markup, L"t-")         ||
                gsIsEqualU2(markup, L"th")         ||
                gsIsEqualU2(markup, L"thx")        ||
                gsIsEqualU2(markup, L"th*")        ||
                gsIsEqualU2(markup, L"tc")         ||
                gsIsEqualU2(markup, L"tcx")        ||
                gsIsEqualU2(markup, L"tc*")        ||
                gsIsEqualU2(markup, L"tc1")        ||
                gsIsEqualU2(markup, L"tc1*")       ||
                gsIsEqualU2(markup, L"...")        ||
                gsIsEqualU2(markup, L"...[")       ||
                gsIsEqualU2(markup, L"...]")       ||
                gsIsEqualU2(markup, L"toc")        ||
                gsIsEqualU2(markup, L"==="))
            {
               gsDestroy(markup);
               index--;
               break;
            }
            gsDestroy(markup);

            // Just a continuation of the paragraph.
            gsAppendA(para->str, " ");
            gsAppend(para->str, gsTrimU2(stemp, WHITESPACE_U2));
         }
      }
      else
      {
         // We have a special paragraph
         markup = gsCreateFromSub(stemp, 0, position);
         gsEraseSub(stemp, 0, position);
         gsTrimU2(stemp, WHITESPACE_U2);

         // Title Lines.
         if      (gsIsEqualU2(markup, L"h")         ||
                  gsIsEqualU2(markup, L"hh")        ||
                  gsIsEqualU2(markup, L"hhh")       ||
                  gsIsEqualU2(markup, L"hhhh")      ||
                  gsIsEqualU2(markup, L"hhhhh")     ||
                  gsIsEqualU2(markup, L"hhhhhh")    ||
                  gsIsEqualU2(markup, L"hhhhhhh")   ||
                  gsIsEqualU2(markup, L"hhhhhhhh")  ||
                  gsIsEqualU2(markup, L"hhhhhhhhh") ||
                  gsIsEqualU2(markup, L"H")         ||
                  gsIsEqualU2(markup, L"HH")        ||
                  gsIsEqualU2(markup, L"HHH")       ||
                  gsIsEqualU2(markup, L"HHHH")      ||
                  gsIsEqualU2(markup, L"HHHHH")     ||
                  gsIsEqualU2(markup, L"HHHHHH")    ||
                  gsIsEqualU2(markup, L"HHHHHHH")   ||
                  gsIsEqualU2(markup, L"HHHHHHHH")  ||
                  gsIsEqualU2(markup, L"HHHHHHHHH"))
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

            para->str     = stemp;
         }
         // List Lines.
         else if (gsIsEqualU2(markup, L"-")         ||
                  gsIsEqualU2(markup, L".-")        ||
                  gsIsEqualU2(markup, L"..-")       ||
                  gsIsEqualU2(markup, L"...-")      ||
                  gsIsEqualU2(markup, L"....-")     ||
                  gsIsEqualU2(markup, L".....-")    ||
                  gsIsEqualU2(markup, L"......-")   ||
                  gsIsEqualU2(markup, L".......-")  ||
                  gsIsEqualU2(markup, L"........-"))
         {
            para->type = paraTypeLIST_ITEM_BULLET_1 + gsGetCount(markup) - 1;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"1")        ||
                  gsIsEqualU2(markup, L".1")       ||
                  gsIsEqualU2(markup, L"..1")      ||
                  gsIsEqualU2(markup, L"...1")     ||
                  gsIsEqualU2(markup, L"....1")    ||
                  gsIsEqualU2(markup, L".....1")   ||
                  gsIsEqualU2(markup, L"......1")  ||
                  gsIsEqualU2(markup, L".......1") ||
                  gsIsEqualU2(markup, L"........1"))
         {
            para->type = paraTypeLIST_ITEM_NUMBER_1 + gsGetCount(markup) - 1;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"-[")         ||
                  gsIsEqualU2(markup, L".-[")        ||
                  gsIsEqualU2(markup, L"..-[")       ||
                  gsIsEqualU2(markup, L"...-[")      ||
                  gsIsEqualU2(markup, L"....-[")     ||
                  gsIsEqualU2(markup, L".....-[")    ||
                  gsIsEqualU2(markup, L"......-[")   ||
                  gsIsEqualU2(markup, L".......-[")  ||
                  gsIsEqualU2(markup, L"........-["))
         {
            para->type = paraTypeLIST_ITEM_BULLET_START_1 + gsGetCount(markup) - 1;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"1[")         ||
                  gsIsEqualU2(markup, L".1[")        ||
                  gsIsEqualU2(markup, L"..1[")       ||
                  gsIsEqualU2(markup, L"...1[")      ||
                  gsIsEqualU2(markup, L"....1[")     ||
                  gsIsEqualU2(markup, L".....1[")    ||
                  gsIsEqualU2(markup, L"......1[")   ||
                  gsIsEqualU2(markup, L".......1[")  ||
                  gsIsEqualU2(markup, L"........1["))
         {
            para->type = paraTypeLIST_ITEM_BULLET_START_1 + gsGetCount(markup) - 1;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"-]")         ||
                  gsIsEqualU2(markup, L".-]")        ||
                  gsIsEqualU2(markup, L"..-]")       ||
                  gsIsEqualU2(markup, L"...-]")      ||
                  gsIsEqualU2(markup, L"....-]")     ||
                  gsIsEqualU2(markup, L".....-]")    ||
                  gsIsEqualU2(markup, L"......-]")   ||
                  gsIsEqualU2(markup, L".......-]")  ||
                  gsIsEqualU2(markup, L"........-]"))
         {
            para->type = paraTypeLIST_ITEM_BULLET_STOP_1 + gsGetCount(markup) - 1;
            para->str  = stemp;
         }
         else if (gsIsEqualU2(markup, L"1]")         ||
                  gsIsEqualU2(markup, L".1]")        ||
                  gsIsEqualU2(markup, L"..1]")       ||
                  gsIsEqualU2(markup, L"...1]")      ||
                  gsIsEqualU2(markup, L"....1]")     ||
                  gsIsEqualU2(markup, L".....1]")    ||
                  gsIsEqualU2(markup, L"......1]")   ||
                  gsIsEqualU2(markup, L".......1]")  ||
                  gsIsEqualU2(markup, L"........1]"))
         {
            para->type = paraTypeLIST_ITEM_BULLET_STOP_1 + gsGetCount(markup) - 1;
            para->str  = stemp;
         }
         // Key Value line.
         else if (gsIsEqualU2(markup, L"kv"))
         {
            para->type     = paraTypeKEY_VALUE;
            para->str      = stemp;
         }
         // Table Lines.
         else if (gsIsEqualU2(markup, L"t["))
         {
            para->type     = paraTypeTABLE_START;
         }
         else if (gsIsEqualU2(markup, L"t]"))
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
         else if (gsIsEqualU2(markup, L"tc1"))
         {
            para->type     = paraTypeTABLE_COL_NUMBER;
            para->str      = stemp;
         }
         else if (gsIsEqualU2(markup, L"tc1*"))
         {
            para->type     = paraTypeTABLE_COL_NUMBER_FILL;
            para->str      = stemp;
         }
         // Formated Lines.
         else if (gsIsEqualU2(markup, L"..."))
         {
            para->type     = paraTypeFORMATED_START;
            para->str      = gsTrimU2(stemp, WHITESPACE_U2);
         }
         else if (gsIsEqualU2(markup, L"...["))
         {
            para->type     = paraTypeFORMATED_START;

            // Special case.  No processing of the lines inside.
            // Keep appending strings until a terminator.
            for (index++; index < strListCount; index++)
            {
               stemp = gsArrayGetAt(strList, index);

               // Break; on terminator.
               breakIf(
                  gsGetCount(stemp)  >= 4    &&
                  *gsGetAt(stemp, 0) == L'.' &&
                  *gsGetAt(stemp, 1) == L'.' &&
                  *gsGetAt(stemp, 2) == L'.' &&
                  *gsGetAt(stemp, 3) == L']');

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
         else if (gsIsEqualU2(markup, L"...]"))
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
       gsIsEqualU2(command, L"rtf"))
   {
      _WriteRTF(path, paraList);
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

   gsFindAndReplaceU2(str, L"|chapter|",                   gsGet(chapter),         NULL);

   gsDestroy(chapter);

   // Styling
   gsFindAndReplaceU2(str, L"|img |",                      L"<img src=\"",         NULL);
   gsFindAndReplaceU2(str, L"| img|",                      L"\" />",               NULL);
   gsFindAndReplaceU2(str, L"|link |",                     L"<a href=\"",          NULL);
   gsFindAndReplaceU2(str, L"| link |",                    L"\">",                 NULL);
   gsFindAndReplaceU2(str, L"| link|",                     L"</a>",                NULL);
   gsFindAndReplaceU2(str, L"|line|",                      L"<br />",              NULL);
   gsFindAndReplaceU2(str, L"|line s|",                    L"<br /><br />",        NULL);
   gsFindAndReplaceU2(str, L"|line d|",                    L"<br /><br /><br />",  NULL);

   // Legal
   gsFindAndReplaceU2(str, L"|sym copyright|",             L"&copy;",              NULL);
   gsFindAndReplaceU2(str, L"|sym registered tm|",         L"&reg;",               NULL);
   gsFindAndReplaceU2(str, L"|sym tm|",                    L"&trade;",             NULL);

   // Currency
   gsFindAndReplaceU2(str, L"|sym bitcoin|",               L"&#x20BF;",            NULL);
   gsFindAndReplaceU2(str, L"|sym cent|",                  L"&cent;",              NULL);
   gsFindAndReplaceU2(str, L"|sym dollar|",                L"$",                   NULL);
   gsFindAndReplaceU2(str, L"|sym euro|",                  L"&euro;",              NULL);
   gsFindAndReplaceU2(str, L"|sym franc|",                 L"&#x20A3;",            NULL);
   gsFindAndReplaceU2(str, L"|sym lira|",                  L"&#x20A4;",            NULL);
   gsFindAndReplaceU2(str, L"|sym lira turkey|",           L"&#x20BA;",            NULL);
   gsFindAndReplaceU2(str, L"|sym peso|",                  L"&#x20B1;",            NULL);
   gsFindAndReplaceU2(str, L"|sym pound|",                 L"&pound;",             NULL);
   gsFindAndReplaceU2(str, L"|sym ruble|",                 L"&#x20BD;",            NULL);
   gsFindAndReplaceU2(str, L"|sym rupee|",                 L"&#x20A8;",            NULL);
   gsFindAndReplaceU2(str, L"|sym rupee india|",           L"&#x20B9;",            NULL);
   gsFindAndReplaceU2(str, L"|sym won|",                   L"&#x20A9;",            NULL);
   gsFindAndReplaceU2(str, L"|sym yen|",                   L"&yen;",               NULL);

   // Punctuation
   gsFindAndReplaceU2(str, L"|sym ...|",                   L"&hellip;",            NULL);
   gsFindAndReplaceU2(str, L"|sym 1/4|",                   L"&frac14;",            NULL);
   gsFindAndReplaceU2(str, L"|sym 1/2|",                   L"&frac12;",            NULL);
   gsFindAndReplaceU2(str, L"|sym 3/4|",                   L"&frac34;",            NULL);
   gsFindAndReplaceU2(str, L"|sym ampersand|",             L"&amp;",               NULL);
   gsFindAndReplaceU2(str, L"|sym at|",                    L"@",                   NULL);
   gsFindAndReplaceU2(str, L"|sym bullet|",                L"&bull;",              NULL);
   gsFindAndReplaceU2(str, L"|sym dagger s|",              L"&dagger;",            NULL);
   gsFindAndReplaceU2(str, L"|sym dagger d|",              L"&Dagger;",            NULL);
   gsFindAndReplaceU2(str, L"|sym hash|",                  L"#",                   NULL);
   gsFindAndReplaceU2(str, L"|sym inv!|",                  L"&iexcl;",             NULL);
   gsFindAndReplaceU2(str, L"|sym inv?|",                  L"&iquest;",            NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle d l|",       L"&laquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle d r|",       L"&raquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle s l|",       L"&lsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle s r|",       L"&rsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"|sym quote d|",               L"\"",                  NULL);
   gsFindAndReplaceU2(str, L"|sym quote d l|",             L"&ldquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote d r|",             L"&rdquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote d low|",           L"&bdquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s|",               L"'",                   NULL);
   gsFindAndReplaceU2(str, L"|sym quote s l|",             L"&lsquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s r|",             L"&rsquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s low|",           L"&sbquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym para|",                  L"&para;",              NULL);
   gsFindAndReplaceU2(str, L"|sym prime d|",               L"&Prime;",             NULL);
   gsFindAndReplaceU2(str, L"|sym prime s|",               L"&prime;",             NULL);

   // Game
   gsFindAndReplaceU2(str, L"|sym arrow d|",               L"&darr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow l|",               L"&larr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow r|",               L"&rarr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow u|",               L"&uarr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow lr|",              L"&harr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow ud|",              L"&#x2195;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow \\d|",             L"&#x2198;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow /d|",              L"&#x2199;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow \\u|",             L"&#x2196;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow /u|",              L"&#x2197;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d l|",             L"&lArr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d lr|",            L"&hArr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d r|",             L"&rArr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow redo|",            L"&#x21B7;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow undo|",            L"&#x21B6;",            NULL);
   gsFindAndReplaceU2(str, L"|sym card suit c|",           L"&clubs;",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit d|",           L"&diams;",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit h|",           L"&hearts;",            NULL);
   gsFindAndReplaceU2(str, L"|sym card suit s|",           L"&spades;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b k|",             L"&#x2654;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b q|",             L"&#x2655;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b r|",             L"&#x2656;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b b|",             L"&#x2657;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b h|",             L"&#x2658;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b p|",             L"&#x2659;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w k|",             L"&#x265A;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w q|",             L"&#x265B;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w r|",             L"&#x265C;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w b|",             L"&#x265D;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w h|",             L"&#x265E;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w p|",             L"&#x265F;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 1|",                 L"&#x2680;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 2|",                 L"&#x2681;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 3|",                 L"&#x2682;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 4|",                 L"&#x2683;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 5|",                 L"&#x2684;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 6|",                 L"&#x2685;",            NULL);
   gsFindAndReplaceU2(str, L"|sym ball baseball|",         L"&#x26BE;",            NULL);
   gsFindAndReplaceU2(str, L"|sym ball soccer|",           L"&#x26BD;",            NULL);

   // Symbols
   gsFindAndReplaceU2(str, L"|sym checkbox off|",          L"&#x2610;",            NULL);
   gsFindAndReplaceU2(str, L"|sym checkbox on|",           L"&#x2611;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 0%|",             L"&#x25CB;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 25%|",            L"&#x25D4;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 50%|",            L"&#x25D1;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 75%|",            L"&#x25D5;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 100%|",           L"&#x25CF;",            NULL);
   gsFindAndReplaceU2(str, L"|sym correct|",               L"&#x2713;",            NULL);
   gsFindAndReplaceU2(str, L"|sym biohazard|",             L"&#x2623;",            NULL);
   gsFindAndReplaceU2(str, L"|sym dot|",                   L"&#x26AA;",            NULL);
   gsFindAndReplaceU2(str, L"|sym dot filled|",            L"&#x26AB;",            NULL);
   gsFindAndReplaceU2(str, L"|sym envelope|",              L"&#x2709;",            NULL);
   gsFindAndReplaceU2(str, L"|sym gender f|",              L"&#x2640;",            NULL);
   gsFindAndReplaceU2(str, L"|sym gender m|",              L"&#x2642;",            NULL);
   gsFindAndReplaceU2(str, L"|sym gender mf|",             L"&#x26A5;",            NULL);
   gsFindAndReplaceU2(str, L"|sym incorrect|",             L"&#x2717;",            NULL);
   gsFindAndReplaceU2(str, L"|sym plane|",                 L"&#x2708;",            NULL);
   gsFindAndReplaceU2(str, L"|sym poison|",                L"&#x2620;",            NULL);
   gsFindAndReplaceU2(str, L"|sym radioactive|",           L"&#x2622;",            NULL);
   gsFindAndReplaceU2(str, L"|sym Rx|",                    L"&#x211E;",            NULL);
   gsFindAndReplaceU2(str, L"|sym recycle|",               L"&#x267B;",            NULL);
   gsFindAndReplaceU2(str, L"|sym scissor|",               L"&#x2702;",            NULL);
   gsFindAndReplaceU2(str, L"|sym snowflake|",             L"&#x2744;",            NULL);
   gsFindAndReplaceU2(str, L"|sym star 5|",                L"&#x2606;",            NULL);
   gsFindAndReplaceU2(str, L"|sym star 5 filled|",         L"&#x2605;",            NULL);
   gsFindAndReplaceU2(str, L"|sym sun|",                   L"&#x2600;",            NULL);

   // Math
   gsFindAndReplaceU2(str, L"|sym +/-|",                   L"&plusmn;",            NULL);
   gsFindAndReplaceU2(str, L"|sym angle|",                 L"&ang;",               NULL);
   gsFindAndReplaceU2(str, L"|sym contains|",              L"&ni;",                NULL);
   gsFindAndReplaceU2(str, L"|sym degree|",                L"&deg;",               NULL);
   gsFindAndReplaceU2(str, L"|sym dot|",                   L"&sdot;",              NULL);
   gsFindAndReplaceU2(str, L"|sym empty set|",             L"&empty;",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal almost|",          L"&asymp;",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal approx |",         L"&cong;",              NULL);
   gsFindAndReplaceU2(str, L"|sym equal greater than|",    L"&ge;",                NULL);
   gsFindAndReplaceU2(str, L"|sym equal less than|",       L"&le;",                NULL);
   gsFindAndReplaceU2(str, L"|sym equal same|",            L"&equiv;",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal not|",             L"&ne;",                NULL);
   gsFindAndReplaceU2(str, L"|sym for all|",               L"&forall;",            NULL);
   gsFindAndReplaceU2(str, L"|sym infinity|",              L"&infin;",             NULL);
   gsFindAndReplaceU2(str, L"|sym integral|",              L"&int;",               NULL);
   gsFindAndReplaceU2(str, L"|sym intersection|",          L"&cap;",               NULL);
   gsFindAndReplaceU2(str, L"|sym is in|",                 L"&isin;",              NULL);
   gsFindAndReplaceU2(str, L"|sym is not in|",             L"&notin;",             NULL);
   gsFindAndReplaceU2(str, L"|sym math phi|",              L"&straightphi;",       NULL);
   gsFindAndReplaceU2(str, L"|sym math pi|",               L"&piv;",               NULL);
   gsFindAndReplaceU2(str, L"|sym minus|",                 L"&minus;",             NULL);
   gsFindAndReplaceU2(str, L"|sym nabla|",                 L"&nabla;",             NULL);
   gsFindAndReplaceU2(str, L"|sym partial diff|",          L"&part;",              NULL);
   gsFindAndReplaceU2(str, L"|sym product|",               L"&prod;",              NULL);
   gsFindAndReplaceU2(str, L"|sym proportional to|",       L"&prop;",              NULL);
   gsFindAndReplaceU2(str, L"|sym root|",                  L"&radic;",             NULL);
   gsFindAndReplaceU2(str, L"|sym sum|",                   L"&sum;",               NULL);
   gsFindAndReplaceU2(str, L"|sym there exists|",          L"&exist;",             NULL);
   gsFindAndReplaceU2(str, L"|sym therefore|",             L"&there4;",            NULL);
   gsFindAndReplaceU2(str, L"|sym union|",                 L"&cup;",               NULL);

   // Greek
   gsFindAndReplaceU2(str, L"|sym alpha cap|",             L"&Alpha;",             NULL);
   gsFindAndReplaceU2(str, L"|sym beta cap|",              L"&Beta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym gamma cap|",             L"&Gamma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym delta cap|",             L"&Delta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym epsilon cap|",           L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym zeta cap|",              L"&Zeta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym eta cap|",               L"&Eta;",               NULL);
   gsFindAndReplaceU2(str, L"|sym theta cap|",             L"&Theta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym iota cap|",              L"&Iota;",              NULL);
   gsFindAndReplaceU2(str, L"|sym kappa cap|",             L"&Kappa;",             NULL);
   gsFindAndReplaceU2(str, L"|sym lambda cap|",            L"&Lambda;",            NULL);
   gsFindAndReplaceU2(str, L"|sym mu cap|",                L"&Mu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym nu cap|",                L"&Nu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym xi cap|",                L"&Xi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym omicron cap|",           L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"|sym pi cap|",                L"&Pi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym rho cap|",               L"&Rho;",               NULL);
   gsFindAndReplaceU2(str, L"|sym signma cap|",            L"&Sigma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym tau cap|",               L"&Tau;",               NULL);
   gsFindAndReplaceU2(str, L"|sym upsilon cap|",           L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym phi cap|",               L"&Phi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym chi cap|",               L"&Chi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym psi cap|",               L"&Psi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym omega cap|",             L"&Omega;",             NULL);
   gsFindAndReplaceU2(str, L"|sym alpha|",                 L"&alpha;",             NULL);
   gsFindAndReplaceU2(str, L"|sym beta|",                  L"&beta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym gamma|",                 L"&gamma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym delta|",                 L"&delta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym epsilon|",               L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym zeta|",                  L"&zeta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym eta|",                   L"&eta;",               NULL);
   gsFindAndReplaceU2(str, L"|sym theta|",                 L"&theta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym iota|",                  L"&iota;",              NULL);
   gsFindAndReplaceU2(str, L"|sym kappa|",                 L"&kappa;",             NULL);
   gsFindAndReplaceU2(str, L"|sym lambda|",                L"&lambda;",            NULL);
   gsFindAndReplaceU2(str, L"|sym mu|",                    L"&mu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym nu|",                    L"&nu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym xi|",                    L"&xi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym omicron|",               L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"|sym pi|",                    L"&pi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym rho|",                   L"&rho;",               NULL);
   gsFindAndReplaceU2(str, L"|sym signma|",                L"&sigma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym tau|",                   L"&tau;",               NULL);
   gsFindAndReplaceU2(str, L"|sym upsilon|",               L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym phi|",                   L"&phi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym chi|",                   L"&chi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym psi|",                   L"&psi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym omega|",                 L"&omega;",             NULL);

   gsFindAndReplaceU2(str, L"|sup",                        L"<sup>",               NULL);
   gsFindAndReplaceU2(str, L"sup|",                        L"</sup>",              NULL);
   gsFindAndReplaceU2(str, L"|sub",                        L"<sub>",               NULL);
   gsFindAndReplaceU2(str, L"sub|",                        L"</sub>",              NULL);
   gsFindAndReplaceU2(str, L"|c",                          L"<code>",              NULL);
   gsFindAndReplaceU2(str, L"c|",                          L"</code>",             NULL);
   gsFindAndReplaceU2(str, L"|b",                          L"<strong>",            NULL);
   gsFindAndReplaceU2(str, L"b|",                          L"</strong>",           NULL);
   gsFindAndReplaceU2(str, L"|i",                          L"<em>",                NULL);
   gsFindAndReplaceU2(str, L"i|",                          L"</em>",               NULL);
   gsFindAndReplaceU2(str, L"|u",                          L"<u>",                 NULL);
   gsFindAndReplaceU2(str, L"u|",                          L"</u>",                NULL);
   gsFindAndReplaceU2(str, L"|s",                          L"<s>",                 NULL);
   gsFindAndReplaceU2(str, L"s|",                          L"</s>",                NULL);

   gsFindAndReplaceU2(str, L"\\|",                         L"|",                   NULL);
   gsFindAndReplaceU2(str, L"|\\",                         L"|",                   NULL);

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

   gsFindAndReplaceU2(str, L"`",                           L"&#96;",               NULL);
   gsFindAndReplaceU2(str, L"*",                           L"&#42;",               NULL);
   gsFindAndReplaceU2(str, L"~",                           L"&#126;",              NULL);
   gsFindAndReplaceU2(str, L"<",                           L"&#60;",               NULL);
   gsFindAndReplaceU2(str, L">",                           L"&#62;",               NULL);

   gsFindAndReplaceU2(str, L"|chapter|",                   gsGet(chapter),         NULL);

   gsDestroy(chapter);

   // Styling
   gsFindAndReplaceU2(str, L"|img |",                      L"![image](",           NULL);
   gsFindAndReplaceU2(str, L"| img|",                      L")",                   NULL);
   gsFindAndReplaceU2(str, L"|link |",                     L"[LINK](",             NULL);
   gsFindAndReplaceU2(str, L"| link |",                    L") ",                  NULL);
   gsFindAndReplaceU2(str, L"| link|",                     L"",                    NULL);
   gsFindAndReplaceU2(str, L"|line|",                      L"<br />",              NULL);
   gsFindAndReplaceU2(str, L"|line s|",                    L"<br /><br />",        NULL);
   gsFindAndReplaceU2(str, L"|line d|",                    L"<br /><br /><br />",  NULL);

   // Legal
   gsFindAndReplaceU2(str, L"|sym copyright|",             L"&copy;",              NULL);
   gsFindAndReplaceU2(str, L"|sym registered tm|",         L"&reg;",               NULL);
   gsFindAndReplaceU2(str, L"|sym tm|",                    L"&trade;",             NULL);

   // Currency
   gsFindAndReplaceU2(str, L"|sym bitcoin|",               L"&#x20BF;",            NULL);
   gsFindAndReplaceU2(str, L"|sym cent|",                  L"&cent;",              NULL);
   gsFindAndReplaceU2(str, L"|sym dollar|",                L"$",                   NULL);
   gsFindAndReplaceU2(str, L"|sym euro|",                  L"&euro;",              NULL);
   gsFindAndReplaceU2(str, L"|sym franc|",                 L"&#x20A3;",            NULL);
   gsFindAndReplaceU2(str, L"|sym lira|",                  L"&#x20A4;",            NULL);
   gsFindAndReplaceU2(str, L"|sym lira turkey|",           L"&#x20BA;",            NULL);
   gsFindAndReplaceU2(str, L"|sym peso|",                  L"&#x20B1;",            NULL);
   gsFindAndReplaceU2(str, L"|sym pound|",                 L"&pound;",             NULL);
   gsFindAndReplaceU2(str, L"|sym ruble|",                 L"&#x20BD;",            NULL);
   gsFindAndReplaceU2(str, L"|sym rupee|",                 L"&#x20A8;",            NULL);
   gsFindAndReplaceU2(str, L"|sym rupee india|",           L"&#x20B9;",            NULL);
   gsFindAndReplaceU2(str, L"|sym won|",                   L"&#x20A9;",            NULL);
   gsFindAndReplaceU2(str, L"|sym yen|",                   L"&yen;",               NULL);

   // Punctuation
   gsFindAndReplaceU2(str, L"|sym ...|",                   L"&hellip;",            NULL);
   gsFindAndReplaceU2(str, L"|sym 1/4|",                   L"&frac14;",            NULL);
   gsFindAndReplaceU2(str, L"|sym 1/2|",                   L"&frac12;",            NULL);
   gsFindAndReplaceU2(str, L"|sym 3/4|",                   L"&frac34;",            NULL);
   gsFindAndReplaceU2(str, L"|sym ampersand|",             L"&amp;",               NULL);
   gsFindAndReplaceU2(str, L"|sym at|",                    L"@",                   NULL);
   gsFindAndReplaceU2(str, L"|sym bullet|",                L"&bull;",              NULL);
   gsFindAndReplaceU2(str, L"|sym dagger s|",              L"&dagger;",            NULL);
   gsFindAndReplaceU2(str, L"|sym dagger d|",              L"&Dagger;",            NULL);
   gsFindAndReplaceU2(str, L"|sym hash|",                  L"#",                   NULL);
   gsFindAndReplaceU2(str, L"|sym inv!|",                  L"&iexcl;",             NULL);
   gsFindAndReplaceU2(str, L"|sym inv?|",                  L"&iquest;",            NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle d l|",       L"&laquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle d r|",       L"&raquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle s l|",       L"&lsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle s r|",       L"&rsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"|sym quote d|",               L"\"",                  NULL);
   gsFindAndReplaceU2(str, L"|sym quote d l|",             L"&ldquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote d r|",             L"&rdquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote d low|",           L"&bdquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s|",               L"'",                   NULL);
   gsFindAndReplaceU2(str, L"|sym quote s l|",             L"&lsquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s r|",             L"&rsquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s low|",           L"&sbquo;",             NULL);
   gsFindAndReplaceU2(str, L"|sym para|",                  L"&para;",              NULL);
   gsFindAndReplaceU2(str, L"|sym prime d|",               L"&Prime;",             NULL);
   gsFindAndReplaceU2(str, L"|sym prime s|",               L"&prime;",             NULL);

   // Game
   gsFindAndReplaceU2(str, L"|sym arrow d|",               L"&darr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow l|",               L"&larr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow r|",               L"&rarr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow u|",               L"&uarr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow lr|",              L"&harr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow ud|",              L"&#x2195;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow \\d|",             L"&#x2198;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow /d|",              L"&#x2199;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow \\u|",             L"&#x2196;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow /u|",              L"&#x2197;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d l|",             L"&lArr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d lr|",            L"&hArr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d r|",             L"&rArr;",              NULL);
   gsFindAndReplaceU2(str, L"|sym arrow redo|",            L"&#x21B7;",            NULL);
   gsFindAndReplaceU2(str, L"|sym arrow undo|",            L"&#x21B6;",            NULL);
   gsFindAndReplaceU2(str, L"|sym card suit c|",           L"&clubs;",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit d|",           L"&diams;",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit h|",           L"&hearts;",            NULL);
   gsFindAndReplaceU2(str, L"|sym card suit s|",           L"&spades;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b k|",             L"&#x2654;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b q|",             L"&#x2655;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b r|",             L"&#x2656;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b b|",             L"&#x2657;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b h|",             L"&#x2658;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess b p|",             L"&#x2659;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w k|",             L"&#x265A;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w q|",             L"&#x265B;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w r|",             L"&#x265C;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w b|",             L"&#x265D;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w h|",             L"&#x265E;",            NULL);
   gsFindAndReplaceU2(str, L"|sym chess w p|",             L"&#x265F;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 1|",                 L"&#x2680;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 2|",                 L"&#x2681;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 3|",                 L"&#x2682;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 4|",                 L"&#x2683;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 5|",                 L"&#x2684;",            NULL);
   gsFindAndReplaceU2(str, L"|sym die 6|",                 L"&#x2685;",            NULL);
   gsFindAndReplaceU2(str, L"|sym ball baseball|",         L"&#x26BE;",            NULL);
   gsFindAndReplaceU2(str, L"|sym ball soccer|",           L"&#x26BD;",            NULL);

   // Symbols
   gsFindAndReplaceU2(str, L"|sym checkbox off|",          L"&#x2610;",            NULL);
   gsFindAndReplaceU2(str, L"|sym checkbox on|",           L"&#x2611;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 0%|",             L"&#x25CB;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 25%|",            L"&#x25D4;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 50%|",            L"&#x25D1;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 75%|",            L"&#x25D5;",            NULL);
   gsFindAndReplaceU2(str, L"|sym circle 100%|",           L"&#x25CF;",            NULL);
   gsFindAndReplaceU2(str, L"|sym correct|",               L"&#x2713;",            NULL);
   gsFindAndReplaceU2(str, L"|sym biohazard|",             L"&#x2623;",            NULL);
   gsFindAndReplaceU2(str, L"|sym dot|",                   L"&#x26AA;",            NULL);
   gsFindAndReplaceU2(str, L"|sym dot filled|",            L"&#x26AB;",            NULL);
   gsFindAndReplaceU2(str, L"|sym envelope|",              L"&#x2709;",            NULL);
   gsFindAndReplaceU2(str, L"|sym gender f|",              L"&#x2640;",            NULL);
   gsFindAndReplaceU2(str, L"|sym gender m|",              L"&#x2642;",            NULL);
   gsFindAndReplaceU2(str, L"|sym gender mf|",             L"&#x26A5;",            NULL);
   gsFindAndReplaceU2(str, L"|sym incorrect|",             L"&#x2717;",            NULL);
   gsFindAndReplaceU2(str, L"|sym plane|",                 L"&#x2708;",            NULL);
   gsFindAndReplaceU2(str, L"|sym poison|",                L"&#x2620;",            NULL);
   gsFindAndReplaceU2(str, L"|sym radioactive|",           L"&#x2622;",            NULL);
   gsFindAndReplaceU2(str, L"|sym Rx|",                    L"&#x211E;",            NULL);
   gsFindAndReplaceU2(str, L"|sym recycle|",               L"&#x267B;",            NULL);
   gsFindAndReplaceU2(str, L"|sym scissor|",               L"&#x2702;",            NULL);
   gsFindAndReplaceU2(str, L"|sym snowflake|",             L"&#x2744;",            NULL);
   gsFindAndReplaceU2(str, L"|sym star 5|",                L"&#x2606;",            NULL);
   gsFindAndReplaceU2(str, L"|sym star 5 filled|",         L"&#x2605;",            NULL);
   gsFindAndReplaceU2(str, L"|sym sun|",                   L"&#x2600;",            NULL);

   // Math
   gsFindAndReplaceU2(str, L"|sym +/-|",                   L"&plusmn;",            NULL);
   gsFindAndReplaceU2(str, L"|sym angle|",                 L"&ang;",               NULL);
   gsFindAndReplaceU2(str, L"|sym contains|",              L"&ni;",                NULL);
   gsFindAndReplaceU2(str, L"|sym degree|",                L"&deg;",               NULL);
   gsFindAndReplaceU2(str, L"|sym dot|",                   L"&sdot;",              NULL);
   gsFindAndReplaceU2(str, L"|sym empty set|",             L"&empty;",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal almost|",          L"&asymp;",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal approx |",         L"&cong;",              NULL);
   gsFindAndReplaceU2(str, L"|sym equal greater than|",    L"&ge;",                NULL);
   gsFindAndReplaceU2(str, L"|sym equal less than|",       L"&le;",                NULL);
   gsFindAndReplaceU2(str, L"|sym equal same|",            L"&equiv;",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal not|",             L"&ne;",                NULL);
   gsFindAndReplaceU2(str, L"|sym for all|",               L"&forall;",            NULL);
   gsFindAndReplaceU2(str, L"|sym infinity|",              L"&infin;",             NULL);
   gsFindAndReplaceU2(str, L"|sym integral|",              L"&int;",               NULL);
   gsFindAndReplaceU2(str, L"|sym intersection|",          L"&cap;",               NULL);
   gsFindAndReplaceU2(str, L"|sym is in|",                 L"&isin;",              NULL);
   gsFindAndReplaceU2(str, L"|sym is not in|",             L"&notin;",             NULL);
   gsFindAndReplaceU2(str, L"|sym math phi|",              L"&straightphi;",       NULL);
   gsFindAndReplaceU2(str, L"|sym math pi|",               L"&piv;",               NULL);
   gsFindAndReplaceU2(str, L"|sym minus|",                 L"&minus;",             NULL);
   gsFindAndReplaceU2(str, L"|sym nabla|",                 L"&nabla;",             NULL);
   gsFindAndReplaceU2(str, L"|sym partial diff|",          L"&part;",              NULL);
   gsFindAndReplaceU2(str, L"|sym product|",               L"&prod;",              NULL);
   gsFindAndReplaceU2(str, L"|sym proportional to|",       L"&prop;",              NULL);
   gsFindAndReplaceU2(str, L"|sym root|",                  L"&radic;",             NULL);
   gsFindAndReplaceU2(str, L"|sym sum|",                   L"&sum;",               NULL);
   gsFindAndReplaceU2(str, L"|sym there exists|",          L"&exist;",             NULL);
   gsFindAndReplaceU2(str, L"|sym therefore|",             L"&there4;",            NULL);
   gsFindAndReplaceU2(str, L"|sym union|",                 L"&cup;",               NULL);

   // Greek
   gsFindAndReplaceU2(str, L"|sym alpha cap|",             L"&Alpha;",             NULL);
   gsFindAndReplaceU2(str, L"|sym beta cap|",              L"&Beta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym gamma cap|",             L"&Gamma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym delta cap|",             L"&Delta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym epsilon cap|",           L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym zeta cap|",              L"&Zeta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym eta cap|",               L"&Eta;",               NULL);
   gsFindAndReplaceU2(str, L"|sym theta cap|",             L"&Theta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym iota cap|",              L"&Iota;",              NULL);
   gsFindAndReplaceU2(str, L"|sym kappa cap|",             L"&Kappa;",             NULL);
   gsFindAndReplaceU2(str, L"|sym lambda cap|",            L"&Lambda;",            NULL);
   gsFindAndReplaceU2(str, L"|sym mu cap|",                L"&Mu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym nu cap|",                L"&Nu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym xi cap|",                L"&Xi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym omicron cap|",           L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"|sym pi cap|",                L"&Pi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym rho cap|",               L"&Rho;",               NULL);
   gsFindAndReplaceU2(str, L"|sym signma cap|",            L"&Sigma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym tau cap|",               L"&Tau;",               NULL);
   gsFindAndReplaceU2(str, L"|sym upsilon cap|",           L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym phi cap|",               L"&Phi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym chi cap|",               L"&Chi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym psi cap|",               L"&Psi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym omega cap|",             L"&Omega;",             NULL);
   gsFindAndReplaceU2(str, L"|sym alpha|",                 L"&alpha;",             NULL);
   gsFindAndReplaceU2(str, L"|sym beta|",                  L"&beta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym gamma|",                 L"&gamma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym delta|",                 L"&delta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym epsilon|",               L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym zeta|",                  L"&zeta;",              NULL);
   gsFindAndReplaceU2(str, L"|sym eta|",                   L"&eta;",               NULL);
   gsFindAndReplaceU2(str, L"|sym theta|",                 L"&theta;",             NULL);
   gsFindAndReplaceU2(str, L"|sym iota|",                  L"&iota;",              NULL);
   gsFindAndReplaceU2(str, L"|sym kappa|",                 L"&kappa;",             NULL);
   gsFindAndReplaceU2(str, L"|sym lambda|",                L"&lambda;",            NULL);
   gsFindAndReplaceU2(str, L"|sym mu|",                    L"&mu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym nu|",                    L"&nu;",                NULL);
   gsFindAndReplaceU2(str, L"|sym xi|",                    L"&xi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym omicron|",               L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"|sym pi|",                    L"&pi;",                NULL);
   gsFindAndReplaceU2(str, L"|sym rho|",                   L"&rho;",               NULL);
   gsFindAndReplaceU2(str, L"|sym signma|",                L"&sigma;",             NULL);
   gsFindAndReplaceU2(str, L"|sym tau|",                   L"&tau;",               NULL);
   gsFindAndReplaceU2(str, L"|sym upsilon|",               L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|sym phi|",                   L"&phi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym chi|",                   L"&chi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym psi|",                   L"&psi;",               NULL);
   gsFindAndReplaceU2(str, L"|sym omega|",                 L"&omega;",             NULL);

   gsFindAndReplaceU2(str, L"|sup",                        L"<sup>",               NULL);
   gsFindAndReplaceU2(str, L"sup|",                        L"</sup>",              NULL);
   gsFindAndReplaceU2(str, L"|sub",                        L"<sub>",               NULL);
   gsFindAndReplaceU2(str, L"sub|",                        L"</sub>",              NULL);
   gsFindAndReplaceU2(str, L"|c",                          L"`",                   NULL);
   gsFindAndReplaceU2(str, L"c|",                          L"`",                   NULL);
   gsFindAndReplaceU2(str, L"|b",                          L"**",                  NULL);
   gsFindAndReplaceU2(str, L"b|",                          L"**",                  NULL);
   gsFindAndReplaceU2(str, L"|i",                          L"*",                   NULL);
   gsFindAndReplaceU2(str, L"i|",                          L"*",                   NULL);
   gsFindAndReplaceU2(str, L"|u",                          L"<u>",                 NULL);
   gsFindAndReplaceU2(str, L"u|",                          L"</u>",                NULL);
   gsFindAndReplaceU2(str, L"|s",                          L"~~",                  NULL);
   gsFindAndReplaceU2(str, L"s|",                          L"~~",                  NULL);

   gsFindAndReplaceU2(str, L"\\|",                         L"&#124;",              NULL);
   gsFindAndReplaceU2(str, L"|\\",                         L"&#124;",              NULL);

   greturn str;
}

/******************************************************************************
func: _ProcessInlineHTML

Process inline markup.
******************************************************************************/
static Gs *_ProcessInlineRTF(Gs * const inStr, Para const * const para)
{
   Gs *chapter,
      *str;

   genter;

   str = gsCreateFrom(inStr);

   // Chapter
   chapter = _ChapterGetString(para->chapter);

   gsFindAndReplaceU2(str, L"`",                           L"\\lquote ",            NULL);
   gsFindAndReplaceU2(str, L"'",                           L"\\rquote ",            NULL);
   gsFindAndReplaceU2(str, L"*",                           L"\\u42s",               NULL);
   gsFindAndReplaceU2(str, L"{",                           L"\\{",                  NULL);
   gsFindAndReplaceU2(str, L"}",                           L"\\}",                  NULL);

   gsFindAndReplaceU2(str, L"|chapter|",                   gsGet(chapter),          NULL);

   gsDestroy(chapter);

   // Styling
   gsFindAndReplaceU2(str, L"|img |",                      L"(",                    NULL);
   gsFindAndReplaceU2(str, L"| img|",                      L")",                    NULL);
   gsFindAndReplaceU2(str, L"|link |",                     L"[",                    NULL);
   gsFindAndReplaceU2(str, L"| link |",                    L"|",                    NULL);
   gsFindAndReplaceU2(str, L"| link|",                     L"]",                    NULL);
   gsFindAndReplaceU2(str, L"|line|",                      L"\\line ",              NULL);
   gsFindAndReplaceU2(str, L"|line s|",                    L"\\line\\line ",        NULL);
   gsFindAndReplaceU2(str, L"|line d|",                    L"\\line\\line\\line ",  NULL);

   // Legal
   gsFindAndReplaceU2(str, L"|sym copyright|",             L"\\u169X",              NULL);
   gsFindAndReplaceU2(str, L"|sym registered tm|",         L"\\u174X",              NULL);
   gsFindAndReplaceU2(str, L"|sym tm|",                    L"\\u8482X",             NULL);

   // Currency
   gsFindAndReplaceU2(str, L"|sym bitcoin|",               L"\\u8383B",             NULL);
   gsFindAndReplaceU2(str, L"|sym cent|",                  L"\\u162c",              NULL);
   gsFindAndReplaceU2(str, L"|sym dollar|",                L"$",                    NULL);
   gsFindAndReplaceU2(str, L"|sym euro|",                  L"\\u8364E",             NULL);
   gsFindAndReplaceU2(str, L"|sym franc|",                 L"\\u8355F",             NULL);
   gsFindAndReplaceU2(str, L"|sym lira|",                  L"\\u8356L",             NULL);
   gsFindAndReplaceU2(str, L"|sym lira turkey|",           L"\\u8378L",             NULL);
   gsFindAndReplaceU2(str, L"|sym peso|",                  L"\\u8369P",             NULL);
   gsFindAndReplaceU2(str, L"|sym pound|",                 L"\\u163L",              NULL);
   gsFindAndReplaceU2(str, L"|sym ruble|",                 L"\\u8381R",             NULL);
   gsFindAndReplaceU2(str, L"|sym rupee|",                 L"\\u8360R",             NULL);
   gsFindAndReplaceU2(str, L"|sym rupee india|",           L"\\u8377R",             NULL);
   gsFindAndReplaceU2(str, L"|sym won|",                   L"\\u8361W",             NULL);
   gsFindAndReplaceU2(str, L"|sym yen|",                   L"\\u165Y",              NULL);

   // Punctuation
   gsFindAndReplaceU2(str, L"|sym ...|",                   L"\\u8230W",             NULL);
   gsFindAndReplaceU2(str, L"|sym 1/4|",                   L"\\u188X",              NULL);
   gsFindAndReplaceU2(str, L"|sym 1/2|",                   L"\\u189X",              NULL);
   gsFindAndReplaceU2(str, L"|sym 3/4|",                   L"\\u190X",              NULL);
   gsFindAndReplaceU2(str, L"|sym ampersand|",             L"&",                    NULL);
   gsFindAndReplaceU2(str, L"|sym at|",                    L"@",                    NULL);
   gsFindAndReplaceU2(str, L"|sym bullet|",                L"\\bullet ",            NULL);
   gsFindAndReplaceU2(str, L"|sym dagger s|",              L"\\u8224X",             NULL);
   gsFindAndReplaceU2(str, L"|sym dagger d|",              L"\\u8225X",             NULL);
   gsFindAndReplaceU2(str, L"|sym hash|",                  L"#",                    NULL);
   gsFindAndReplaceU2(str, L"|sym inv!|",                  L"\\u161!",              NULL);
   gsFindAndReplaceU2(str, L"|sym inv?|",                  L"\\u191?",              NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle d l|",       L"\\u171X",              NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle d r|",       L"\\u187X",              NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle s l|",       L"\\u8249X",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote angle s r|",       L"\\u8250X",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote d|",               L"\"",                   NULL);
   gsFindAndReplaceU2(str, L"|sym quote d l|",             L"\\u8220X",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote d r|",             L"\\u8221X",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote d low|",           L"\\u8222X",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s|",               L"'",                    NULL);
   gsFindAndReplaceU2(str, L"|sym quote s l|",             L"\\u8216X",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s r|",             L"\\u8217X",             NULL);
   gsFindAndReplaceU2(str, L"|sym quote s low|",           L"\\u8218X",             NULL);
   gsFindAndReplaceU2(str, L"|sym para|",                  L"\\u182X",              NULL);
   gsFindAndReplaceU2(str, L"|sym prime d|",               L"\\u8242X",             NULL);
   gsFindAndReplaceU2(str, L"|sym prime s|",               L"\\u8243X",             NULL);

   // Game
   gsFindAndReplaceU2(str, L"|sym arrow d|",               L"\\u8595X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow l|",               L"\\u8592X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow r|",               L"\\u8594X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow u|",               L"\\u8593X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow lr|",              L"\\u8596X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow ud|",              L"\\u8597X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow \\d|",             L"\\u8600X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow /d|",              L"\\u8601X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow \\u|",             L"\\u8598X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow /u|",              L"\\u8599X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d l|",             L"\\u8656X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d lr|",            L"\\u8660X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow d r|",             L"\\u8658X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow redo|",            L"\\u8631X",             NULL);
   gsFindAndReplaceU2(str, L"|sym arrow undo|",            L"\\u8630X",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit c|",           L"\\u9827X",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit d|",           L"\\u9830X",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit h|",           L"\\u9829X",             NULL);
   gsFindAndReplaceU2(str, L"|sym card suit s|",           L"\\u9828X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess b k|",             L"\\u9812X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess b q|",             L"\\u9813X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess b r|",             L"\\u9814X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess b b|",             L"\\u9815X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess b h|",             L"\\u9816X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess b p|",             L"\\u9817X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess w k|",             L"\\u9818X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess w q|",             L"\\u9819X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess w r|",             L"\\u9820X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess w b|",             L"\\u9821X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess w h|",             L"\\u9822X",             NULL);
   gsFindAndReplaceU2(str, L"|sym chess w p|",             L"\\u9823X",             NULL);
   gsFindAndReplaceU2(str, L"|sym die 1|",                 L"\\u9856X",             NULL);
   gsFindAndReplaceU2(str, L"|sym die 2|",                 L"\\u9857X",             NULL);
   gsFindAndReplaceU2(str, L"|sym die 3|",                 L"\\u9858X",             NULL);
   gsFindAndReplaceU2(str, L"|sym die 4|",                 L"\\u9859X",             NULL);
   gsFindAndReplaceU2(str, L"|sym die 5|",                 L"\\u9860X",             NULL);
   gsFindAndReplaceU2(str, L"|sym die 6|",                 L"\\u9861X",             NULL);
   gsFindAndReplaceU2(str, L"|sym ball baseball|",         L"\\u9918X",             NULL);
   gsFindAndReplaceU2(str, L"|sym ball soccer|",           L"\\u9917X",             NULL);

   // Symbols
   gsFindAndReplaceU2(str, L"|sym checkbox off|",          L"\\u9744X",             NULL);
   gsFindAndReplaceU2(str, L"|sym checkbox on|",           L"\\u9745X",             NULL);
   gsFindAndReplaceU2(str, L"|sym circle 0%|",             L"\\u9675X",             NULL);
   gsFindAndReplaceU2(str, L"|sym circle 25%|",            L"\\u9684X",             NULL);
   gsFindAndReplaceU2(str, L"|sym circle 50%|",            L"\\u9681X",             NULL);
   gsFindAndReplaceU2(str, L"|sym circle 75%|",            L"\\u9685X",             NULL);
   gsFindAndReplaceU2(str, L"|sym circle 100%|",           L"\\u9679X",             NULL);
   gsFindAndReplaceU2(str, L"|sym correct|",               L"\\u10003X",            NULL);
   gsFindAndReplaceU2(str, L"|sym biohazard|",             L"\\u9763X",             NULL);
   gsFindAndReplaceU2(str, L"|sym dot|",                   L"\\u9898X",             NULL);
   gsFindAndReplaceU2(str, L"|sym dot filled|",            L"\\u9899X",             NULL);
   gsFindAndReplaceU2(str, L"|sym envelope|",              L"\\u9993X",             NULL);
   gsFindAndReplaceU2(str, L"|sym gender f|",              L"\\u9792X",             NULL);
   gsFindAndReplaceU2(str, L"|sym gender m|",              L"\\u9794X",             NULL);
   gsFindAndReplaceU2(str, L"|sym gender mf|",             L"\\u9893X",             NULL);
   gsFindAndReplaceU2(str, L"|sym incorrect|",             L"\\u10007X",            NULL);
   gsFindAndReplaceU2(str, L"|sym plane|",                 L"\\u9992X",             NULL);
   gsFindAndReplaceU2(str, L"|sym poison|",                L"\\u9760X",             NULL);
   gsFindAndReplaceU2(str, L"|sym radioactive|",           L"\\u9762X",             NULL);
   gsFindAndReplaceU2(str, L"|sym Rx|",                    L"\\u8478X",             NULL);
   gsFindAndReplaceU2(str, L"|sym recycle|",               L"\\u9851X",             NULL);
   gsFindAndReplaceU2(str, L"|sym scissor|",               L"\\u9986X",             NULL);
   gsFindAndReplaceU2(str, L"|sym snowflake|",             L"\\u10052X",            NULL);
   gsFindAndReplaceU2(str, L"|sym star 5|",                L"\\u9734X",             NULL);
   gsFindAndReplaceU2(str, L"|sym star 5 filled|",         L"\\u9733X",             NULL);
   gsFindAndReplaceU2(str, L"|sym sun|",                   L"\\u9728X",             NULL);

   // Math
   gsFindAndReplaceU2(str, L"|sym +/-|",                   L"\\u177X",              NULL);
   gsFindAndReplaceU2(str, L"|sym angle|",                 L"\\u8736X",             NULL);
   gsFindAndReplaceU2(str, L"|sym contains|",              L"\\u8715X",             NULL);
   gsFindAndReplaceU2(str, L"|sym degree|",                L"\\u176X",              NULL);
   gsFindAndReplaceU2(str, L"|sym dot|",                   L"\\u8901X",             NULL);
   gsFindAndReplaceU2(str, L"|sym empty set|",             L"\\u8709X",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal almost|",          L"\\u8776X",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal approx |",         L"\\u8773X",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal greater than|",    L"\\u8805X",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal less than|",       L"\\u8804X",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal same|",            L"\\u8801X",             NULL);
   gsFindAndReplaceU2(str, L"|sym equal not|",             L"\\u8800X",             NULL);
   gsFindAndReplaceU2(str, L"|sym for all|",               L"\\u8704X",             NULL);
   gsFindAndReplaceU2(str, L"|sym infinity|",              L"\\u8734X",             NULL);
   gsFindAndReplaceU2(str, L"|sym integral|",              L"\\u8747X",             NULL);
   gsFindAndReplaceU2(str, L"|sym intersection|",          L"\\u8745X",             NULL);
   gsFindAndReplaceU2(str, L"|sym is in|",                 L"\\u8712X",             NULL);
   gsFindAndReplaceU2(str, L"|sym is not in|",             L"\\u8713X",             NULL);
   gsFindAndReplaceU2(str, L"|sym math phi|",              L"\\u981PX",             NULL);
   gsFindAndReplaceU2(str, L"|sym math pi|",               L"\\u982PX",             NULL);
   gsFindAndReplaceU2(str, L"|sym minus|",                 L"\\u8722X",             NULL);
   gsFindAndReplaceU2(str, L"|sym nabla|",                 L"\\u8711X",             NULL);
   gsFindAndReplaceU2(str, L"|sym partial diff|",          L"\\u8706X",             NULL);
   gsFindAndReplaceU2(str, L"|sym product|",               L"\\u8719X",             NULL);
   gsFindAndReplaceU2(str, L"|sym proportional to|",       L"\\u8733X",             NULL);
   gsFindAndReplaceU2(str, L"|sym root|",                  L"\\u8730X",             NULL);
   gsFindAndReplaceU2(str, L"|sym sum|",                   L"\\u8721X",             NULL);
   gsFindAndReplaceU2(str, L"|sym there exists|",          L"\\u8707X",             NULL);
   gsFindAndReplaceU2(str, L"|sym therefore|",             L"\\u8756X",             NULL);
   gsFindAndReplaceU2(str, L"|sym union|",                 L"\\u8746X",             NULL);

   // Greek
   gsFindAndReplaceU2(str, L"|sym alpha cap|",             L"\\u913X",              NULL);
   gsFindAndReplaceU2(str, L"|sym beta cap|",              L"\\u914X",              NULL);
   gsFindAndReplaceU2(str, L"|sym gamma cap|",             L"\\u915X",              NULL);
   gsFindAndReplaceU2(str, L"|sym delta cap|",             L"\\u916X",              NULL);
   gsFindAndReplaceU2(str, L"|sym epsilon cap|",           L"\\u917X",              NULL);
   gsFindAndReplaceU2(str, L"|sym zeta cap|",              L"\\u918X",              NULL);
   gsFindAndReplaceU2(str, L"|sym eta cap|",               L"\\u919X",              NULL);
   gsFindAndReplaceU2(str, L"|sym theta cap|",             L"\\u920X",              NULL);
   gsFindAndReplaceU2(str, L"|sym iota cap|",              L"\\u921X",              NULL);
   gsFindAndReplaceU2(str, L"|sym kappa cap|",             L"\\u922X",              NULL);
   gsFindAndReplaceU2(str, L"|sym lambda cap|",            L"\\u923X",              NULL);
   gsFindAndReplaceU2(str, L"|sym mu cap|",                L"\\u924X",              NULL);
   gsFindAndReplaceU2(str, L"|sym nu cap|",                L"\\u925X",              NULL);
   gsFindAndReplaceU2(str, L"|sym xi cap|",                L"\\u926X",              NULL);
   gsFindAndReplaceU2(str, L"|sym omicron cap|",           L"\\u927X",              NULL);
   gsFindAndReplaceU2(str, L"|sym pi cap|",                L"\\u928X",              NULL);
   gsFindAndReplaceU2(str, L"|sym rho cap|",               L"\\u929X",              NULL);
   gsFindAndReplaceU2(str, L"|sym signma cap|",            L"\\u931X",              NULL);
   gsFindAndReplaceU2(str, L"|sym tau cap|",               L"\\u932X",              NULL);
   gsFindAndReplaceU2(str, L"|sym upsilon cap|",           L"\\u933X",              NULL);
   gsFindAndReplaceU2(str, L"|sym phi cap|",               L"\\u934X",              NULL);
   gsFindAndReplaceU2(str, L"|sym chi cap|",               L"\\u935X",              NULL);
   gsFindAndReplaceU2(str, L"|sym psi cap|",               L"\\u936X",              NULL);
   gsFindAndReplaceU2(str, L"|sym omega cap|",             L"\\u937X",              NULL);
   gsFindAndReplaceU2(str, L"|sym alpha|",                 L"\\u945X",              NULL);
   gsFindAndReplaceU2(str, L"|sym beta|",                  L"\\u946X",              NULL);
   gsFindAndReplaceU2(str, L"|sym gamma|",                 L"\\u947X",              NULL);
   gsFindAndReplaceU2(str, L"|sym delta|",                 L"\\u948X",              NULL);
   gsFindAndReplaceU2(str, L"|sym epsilon|",               L"\\u949X",              NULL);
   gsFindAndReplaceU2(str, L"|sym zeta|",                  L"\\u950X",              NULL);
   gsFindAndReplaceU2(str, L"|sym eta|",                   L"\\u951X",              NULL);
   gsFindAndReplaceU2(str, L"|sym theta|",                 L"\\u952X",              NULL);
   gsFindAndReplaceU2(str, L"|sym iota|",                  L"\\u953X",              NULL);
   gsFindAndReplaceU2(str, L"|sym kappa|",                 L"\\u954X",              NULL);
   gsFindAndReplaceU2(str, L"|sym lambda|",                L"\\u955X",              NULL);
   gsFindAndReplaceU2(str, L"|sym mu|",                    L"\\u956X",              NULL);
   gsFindAndReplaceU2(str, L"|sym nu|",                    L"\\u957X",              NULL);
   gsFindAndReplaceU2(str, L"|sym xi|",                    L"\\u958X",              NULL);
   gsFindAndReplaceU2(str, L"|sym omicron|",               L"\\u959X",              NULL);
   gsFindAndReplaceU2(str, L"|sym pi|",                    L"\\u960X",              NULL);
   gsFindAndReplaceU2(str, L"|sym rho|",                   L"\\u961X",              NULL);
   gsFindAndReplaceU2(str, L"|sym signma|",                L"\\u963X",              NULL);
   gsFindAndReplaceU2(str, L"|sym tau|",                   L"\\u964X",              NULL);
   gsFindAndReplaceU2(str, L"|sym upsilon|",               L"\\u965X",              NULL);
   gsFindAndReplaceU2(str, L"|sym phi|",                   L"\\u966X",              NULL);
   gsFindAndReplaceU2(str, L"|sym chi|",                   L"\\u967X",              NULL);
   gsFindAndReplaceU2(str, L"|sym psi|",                   L"\\u968X",              NULL);
   gsFindAndReplaceU2(str, L"|sym omega|",                 L"\\u969X",              NULL);

   gsFindAndReplaceU2(str, L"|sup",                        L"\\super ",             NULL);
   gsFindAndReplaceU2(str, L"sup|",                        L"\\nosupersub ",        NULL);
   gsFindAndReplaceU2(str, L"|sub",                        L"\\sub ",               NULL);
   gsFindAndReplaceU2(str, L"sub|",                        L"\\nosupersub ",        NULL);
   gsFindAndReplaceU2(str, L"|c",                          L"\\f2 ",                NULL);
   gsFindAndReplaceU2(str, L"c|",                          L"\\f0 ",                NULL);
   gsFindAndReplaceU2(str, L"|b",                          L"\\b ",                 NULL);
   gsFindAndReplaceU2(str, L"b|",                          L"\\b0 ",                NULL);
   gsFindAndReplaceU2(str, L"|i",                          L"\\i ",                 NULL);
   gsFindAndReplaceU2(str, L"i|",                          L"\\i0 ",                NULL);
   gsFindAndReplaceU2(str, L"|u",                          L"\\ul1 ",               NULL);
   gsFindAndReplaceU2(str, L"u|",                          L"\\ul0 ",               NULL);
   gsFindAndReplaceU2(str, L"|s",                          L"\\strike ",            NULL);
   gsFindAndReplaceU2(str, L"s|",                          L"\\strike0 ",           NULL);
   gsFindAndReplaceU2(str, L"|\"",                         L"\\ldblquote ",         NULL);
   gsFindAndReplaceU2(str, L"\"|",                         L"\\rdblquote ",         NULL);

   gsFindAndReplaceU2(str, L"\\|",                         L"|",                    NULL);
   gsFindAndReplaceU2(str, L"|\\",                         L"|",                    NULL);

   greturn str;
}

/******************************************************************************
func: _WriteHTML
******************************************************************************/
static Gb _WriteHTML(Gpath const * const path, ParaArray const * const paraList)
{
   Gpath       *pathHtml;
   Gfile       *file;
   Gindex       index;
   Para        *para;
   Gs          *title,
               *chapter,
               *stemp;
   Gb           isTableRowActive,
                isTableColHActive,
                isTableColActive;
   ListType     listType[9];
   ListActive   isListActive[9];
   Gi4          listLevel;

   genter;

   isTableRowActive  = gbFALSE;
   isTableColHActive = gbFALSE;
   isTableColActive  = gbFALSE;

   forCount (index, 9)
   {
      listType[index]     = listTypeBULLET;
      isListActive[index] = listActiveNO;
   }
      
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
      "  <link rel=\"stylesheet\" type=\"text/css\" href=\"style_zword.css\">\n"
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
      " <body class=\"zword\">\n",
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
         _WriteHTMLPopList(file, gbFALSE, 0, listType, isListActive);

         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetA(file, gcTypeU1, "<p class=\"zword\">", NULL);
         gfileSetS(file, gcTypeU1, stemp,                NULL);
         gfileSetA(file, gcTypeU1, "</p>\n",             NULL);
         gsDestroy(stemp);
         break;

      case paraTypeKEY_VALUE:
         _WriteHTMLPopList(file, gbFALSE, 0, listType, isListActive);

         //TODO
         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetA(file, gcTypeU1, "<p class=\"zword\">", NULL);
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
         // Titles will never appear in a list.  Force any open list closed.
         _WriteHTMLPopList(file, gbTRUE, 0, listType, isListActive);

         switch (para->type)
         {
         case paraTypeTITLE_1:
         case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "\n\n<h1 class=\"zword\">", NULL); break;
         case paraTypeTITLE_2:
         case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "\n<h2 class=\"zword\">", NULL); break;
         case paraTypeTITLE_3:
         case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "\n<h3 class=\"zword\">", NULL); break;
         case paraTypeTITLE_4:
         case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "\n<h4 class=\"zword\">", NULL); break;
         case paraTypeTITLE_5:
         case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "\n<h5 class=\"zword\">", NULL); break;
         case paraTypeTITLE_6:
         case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "\n<h6 class=\"zword\">", NULL); break;
         case paraTypeTITLE_7:
         case paraTypeTITLE_TOC_7: gfileSetA(file, gcTypeU1, "\n<p class=\"zwordh7\">", NULL); break;
         case paraTypeTITLE_8:
         case paraTypeTITLE_TOC_8: gfileSetA(file, gcTypeU1, "\n<p class=\"zwordh8\">", NULL); break;
         case paraTypeTITLE_9:
         case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "\n<p class=\"zwordh9\">", NULL); break;
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

      case paraTypeLIST_ITEM_BULLET_1:
      case paraTypeLIST_ITEM_BULLET_2:
      case paraTypeLIST_ITEM_BULLET_3:
      case paraTypeLIST_ITEM_BULLET_4:
      case paraTypeLIST_ITEM_BULLET_5:
      case paraTypeLIST_ITEM_BULLET_6:
      case paraTypeLIST_ITEM_BULLET_7:
      case paraTypeLIST_ITEM_BULLET_8:
      case paraTypeLIST_ITEM_BULLET_9:
         listLevel = para->type - paraTypeLIST_ITEM_BULLET_1;

         // Close off lower levels.
         _WriteHTMLPopList(file, gbTRUE, listLevel + 1, listType, isListActive);

         // Start the list.
         if      (isListActive[listLevel] == listActiveNO)
         {
            gfileSetA(file, gcTypeU1, "<ul class=\"zword\">\n", NULL);
            listType[listLevel] = listTypeBULLET;
         }
         // Close off the last item.
         else if (isListActive[listLevel] == listActiveYES)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetA(file, gcTypeU1, "<li class=\"zword\">", NULL);
         gfileSetS(file, gcTypeU1, stemp,                 NULL);
         gfileSetA(file, gcTypeU1, "</li>\n",             NULL);
         gsDestroy(stemp);

         isListActive[listLevel] = listActiveYES_SINGLE_LINE;
         break;

      case paraTypeLIST_ITEM_NUMBER_1:
      case paraTypeLIST_ITEM_NUMBER_2:
      case paraTypeLIST_ITEM_NUMBER_3:
      case paraTypeLIST_ITEM_NUMBER_4:
      case paraTypeLIST_ITEM_NUMBER_5:
      case paraTypeLIST_ITEM_NUMBER_6:
      case paraTypeLIST_ITEM_NUMBER_7:
      case paraTypeLIST_ITEM_NUMBER_8:
      case paraTypeLIST_ITEM_NUMBER_9:
         listLevel = para->type - paraTypeLIST_ITEM_NUMBER_1;

         // Close off lower levels.
         _WriteHTMLPopList(file, gbTRUE, listLevel + 1, listType, isListActive);

         // Start the list.
         if      (isListActive[listLevel] == listActiveNO)
         {
            gfileSetA(file, gcTypeU1, "<ol class=\"zword\">\n", NULL);
            listType[listLevel] = listTypeNUMBER;
         }
         // Close off the last item.
         else if (isListActive[listLevel] == listActiveYES)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         stemp = _ProcessInlineHTML(para->str, para);
         gfileSetA(file, gcTypeU1, "<li class=\"zword\">", NULL);
         gfileSetS(file, gcTypeU1, stemp,                 NULL);
         gfileSetA(file, gcTypeU1, "</li>\n",             NULL);
         gsDestroy(stemp);

         isListActive[listLevel] = listActiveYES_SINGLE_LINE;
         break;

      case paraTypeLIST_ITEM_BULLET_START_1:
      case paraTypeLIST_ITEM_BULLET_START_2:
      case paraTypeLIST_ITEM_BULLET_START_3:
      case paraTypeLIST_ITEM_BULLET_START_4:
      case paraTypeLIST_ITEM_BULLET_START_5:
      case paraTypeLIST_ITEM_BULLET_START_6:
      case paraTypeLIST_ITEM_BULLET_START_7:
      case paraTypeLIST_ITEM_BULLET_START_8:
      case paraTypeLIST_ITEM_BULLET_START_9:
         listLevel = para->type - paraTypeLIST_ITEM_BULLET_START_1;

         // Close off lower levels.
         _WriteHTMLPopList(file, gbTRUE, listLevel + 1, listType, isListActive);

         // Start the list.
         if      (isListActive[listLevel] == listActiveNO)
         {
            gfileSetA(file, gcTypeU1, "<ol class=\"zword\">\n", NULL);
            listType[listLevel] = listTypeBULLET;
         }
         // Close off the last item.
         else if (isListActive[listLevel] == listActiveYES)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "<li class=\"zword\">", NULL);

         isListActive[listLevel] = listActiveYES;
         break;

      case paraTypeLIST_ITEM_NUMBER_START_1:
      case paraTypeLIST_ITEM_NUMBER_START_2:
      case paraTypeLIST_ITEM_NUMBER_START_3:
      case paraTypeLIST_ITEM_NUMBER_START_4:
      case paraTypeLIST_ITEM_NUMBER_START_5:
      case paraTypeLIST_ITEM_NUMBER_START_6:
      case paraTypeLIST_ITEM_NUMBER_START_7:
      case paraTypeLIST_ITEM_NUMBER_START_8:
      case paraTypeLIST_ITEM_NUMBER_START_9:
         listLevel = para->type - paraTypeLIST_ITEM_NUMBER_START_1;

         // Close off lower levels.
         _WriteHTMLPopList(file, gbTRUE, listLevel + 1, listType, isListActive);

         // Start the list.
         if      (isListActive[listLevel] == listActiveNO)
         {
            gfileSetA(file, gcTypeU1, "<ol class=\"zword\">\n", NULL);
            listType[listLevel] = listTypeNUMBER;
         }
         // Close off the last item.
         else if (isListActive[listLevel] == listActiveYES)
         {
            gfileSetA(file, gcTypeU1, "</li>\n", NULL);
         }

         gfileSetA(file, gcTypeU1, "<li class=\"zword\">", NULL);

         isListActive[listLevel] = listActiveYES;
         break;

      case paraTypeLIST_ITEM_BULLET_STOP_1:
      case paraTypeLIST_ITEM_BULLET_STOP_2:
      case paraTypeLIST_ITEM_BULLET_STOP_3:
      case paraTypeLIST_ITEM_BULLET_STOP_4:
      case paraTypeLIST_ITEM_BULLET_STOP_5:
      case paraTypeLIST_ITEM_BULLET_STOP_6:
      case paraTypeLIST_ITEM_BULLET_STOP_7:
      case paraTypeLIST_ITEM_BULLET_STOP_8:
      case paraTypeLIST_ITEM_BULLET_STOP_9:
         listLevel = para->type = paraTypeLIST_ITEM_BULLET_STOP_1;

         // Close off lower levels.
         _WriteHTMLPopList(file, gbTRUE, listLevel + 1, listType, isListActive);

         if (isListActive[listLevel] == listActiveYES ||
             isListActive[listLevel] == listActiveYES_SINGLE_LINE)
         {
            // Close off the last item.
            if (isListActive[listLevel] == listActiveYES)
            {
               gfileSetA(file, gcTypeU1, "</li>\n", NULL);
            }
   
            if (listType[listLevel] == listTypeBULLET)
            {
               gfileSetA(file, gcTypeU1, "</ul>\n", NULL);
            }
            else
            {
               gfileSetA(file, gcTypeU1, "</ol>\n", NULL);
            }
         }

         isListActive[listLevel] = listActiveNO;
         break;

      case paraTypeLIST_ITEM_NUMBER_STOP_1:
      case paraTypeLIST_ITEM_NUMBER_STOP_2:
      case paraTypeLIST_ITEM_NUMBER_STOP_3:
      case paraTypeLIST_ITEM_NUMBER_STOP_4:
      case paraTypeLIST_ITEM_NUMBER_STOP_5:
      case paraTypeLIST_ITEM_NUMBER_STOP_6:
      case paraTypeLIST_ITEM_NUMBER_STOP_7:
      case paraTypeLIST_ITEM_NUMBER_STOP_8:
      case paraTypeLIST_ITEM_NUMBER_STOP_9:
         listLevel = para->type = paraTypeLIST_ITEM_NUMBER_STOP_1;

         // Close off lower levels.
         _WriteHTMLPopList(file, gbTRUE, listLevel + 1, listType, isListActive);

         if (isListActive[listLevel] == listActiveYES ||
             isListActive[listLevel] == listActiveYES_SINGLE_LINE)
         {
            // Close off the last item.
            if (isListActive[listLevel] == listActiveYES)
            {
               gfileSetA(file, gcTypeU1, "</li>\n", NULL);
            }
   
            if (listType[listLevel] == listTypeBULLET)
            {
               gfileSetA(file, gcTypeU1, "</ul>\n", NULL);
            }
            else
            {
               gfileSetA(file, gcTypeU1, "</ol>\n", NULL);
            }
         }

         isListActive[listLevel] = listActiveNO;
         break;

      case paraTypeTABLE_START:
         // Close off any applicable lists.
         _WriteHTMLPopList(file, gbFALSE, 0, listType, isListActive);

         gfileSetA(file, gcTypeU1, "<table class=\"zword\"><tbody class=\"zword\">\n", NULL);
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

         gfileSetA(file, gcTypeU1, "\n</tbody></table>\n", NULL);
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

         gfileSetA(file, gcTypeU1, "<tr class=\"zword\">\n", NULL);
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
         case paraTypeTABLE_COL_HEADER:            gfileSetA(file, gcTypeU1, "<th class=\"zword\">",       NULL); break;
         case paraTypeTABLE_COL_HEADER_NO_BREAK:   gfileSetA(file, gcTypeU1, "<th class=\"zword\"><nobr>", NULL); break;
         case paraTypeTABLE_COL_HEADER_FILL:       gfileSetA(file, gcTypeU1, "<th class=\"zword_fill\">",  NULL); break;
         case paraTypeTABLE_COL:                   gfileSetA(file, gcTypeU1, "<td class=\"zword\">",       NULL); break;
         case paraTypeTABLE_COL_NO_BREAK:          gfileSetA(file, gcTypeU1, "<td class=\"zword\"><nobr>", NULL); break;
         case paraTypeTABLE_COL_FILL:              gfileSetA(file, gcTypeU1, "<td class=\"zword_fill\">",  NULL); break;
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
         // Close off any applicable lists.
         _WriteHTMLPopList(file, gbFALSE, 0, listType, isListActive);

         gfileSetA(file, gcTypeU1, "<pre class=\"zword\">\n", NULL);
         gfileSetS(file, gcTypeU1, para->str, NULL);
         gfileSetA(file, gcTypeU1, "\n</pre>\n", NULL);
         break;

      case paraTypeTABLE_OF_CONTENTS:
         // Close off any applicable lists.
         _WriteHTMLPopList(file, gbTRUE, 0, listType, isListActive);

         _WriteHTMLTOC(file, paraList);
         break;

      case paraTypePAGE_BREAK:
         // Close off any applicable lists.
         _WriteHTMLPopList(file, gbTRUE, 0, listType, isListActive);
         gfileSetA(file, gcTypeU1, "\n<hr />\n", NULL);
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
func: _WriteHTMLPopList
******************************************************************************/
static void _WriteHTMLPopList(Gfile * const file, Gb const isForced, Gindex const listLevel, 
   ListType * const listType, ListActive * const isListActive)
{
   Gindex index;

   genter;

   for (index = 8; index >= listLevel; index--)
   {
      // Level is not active.
      continueIf(isListActive[index] == listActiveNO);

      // If the list is active but it is a multi-paragraph item then we are still
      // inside the list item.
      if (!isForced)
      {
         breakIf(isListActive[index] == listActiveYES);
      }

      // The level is a single line list item.  If we are not seeing another 
      // list item then this function is called.  And we should close up the
      // list.

      // Only happens when forced.  Close off the list item.
      if (isListActive[index] == listActiveYES)
      {
         gfileSetA(file, gcTypeU1, "</li>\n", NULL);
      }

      // Terminate the list.
      if (listType[index] == listTypeBULLET)
      {
         gfileSetA(file, gcTypeU1, "</ul>\n", NULL);
      }
      else
      {
         gfileSetA(file, gcTypeU1, "</ol>\n", NULL);
      }

      // Set the level inactive.
      isListActive[index] = listActiveNO;
      listType[    index] = listTypeBULLET;
   }

   greturn;
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
      case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc1\">",  NULL); break;
      case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc2\">",  NULL); break;
      case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc3\">",  NULL); break;
      case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc4\">",  NULL); break;
      case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc5\">",  NULL); break;
      case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc6\">",  NULL); break;
      case paraTypeTITLE_TOC_7: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc7\">",  NULL); break;
      case paraTypeTITLE_TOC_8: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc8\">",  NULL); break;
      case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "<p class=\"zword_toc9\">",  NULL); break;
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
func: _WriteMD
******************************************************************************/
static Gb _WriteMD(Gpath const * const path, ParaArray const * const paraList)
{
   Gpath *pathHtml;
   Gfile *file;
   Gindex       index,
                hindex,
                lindex;
   Para        *para;
   Gs          *stemp;
   Gcount       tableHeaderCount;
   Gb           isTableRowActive,
                isTableColHActive,
                isTableColActive,
                isTableHeaderLineWritten;
   ListType     listType[9];
   ListActive   isListActive[9];

   genter;

   tableHeaderCount         = 0;
   isTableHeaderLineWritten = gbFALSE;
   isTableRowActive         = gbFALSE;
   isTableColHActive        = gbFALSE;
   isTableColActive         = gbFALSE;
      
   forCount (index, 9)
   {
      listType[index]     = listTypeBULLET;
      isListActive[index] = listActiveNO;
   }

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

      case paraTypeKEY_VALUE:
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

      case paraTypeLIST_ITEM_BULLET_1:
      case paraTypeLIST_ITEM_BULLET_2:
      case paraTypeLIST_ITEM_BULLET_3:
      case paraTypeLIST_ITEM_BULLET_4:
      case paraTypeLIST_ITEM_BULLET_5:
      case paraTypeLIST_ITEM_BULLET_6:
      case paraTypeLIST_ITEM_BULLET_7:
      case paraTypeLIST_ITEM_BULLET_8:
      case paraTypeLIST_ITEM_BULLET_9:
      case paraTypeLIST_ITEM_NUMBER_1:
      case paraTypeLIST_ITEM_NUMBER_2:
      case paraTypeLIST_ITEM_NUMBER_3:
      case paraTypeLIST_ITEM_NUMBER_4:
      case paraTypeLIST_ITEM_NUMBER_5:
      case paraTypeLIST_ITEM_NUMBER_6:
      case paraTypeLIST_ITEM_NUMBER_7:
      case paraTypeLIST_ITEM_NUMBER_8:
      case paraTypeLIST_ITEM_NUMBER_9:
      case paraTypeLIST_ITEM_BULLET_START_1:
      case paraTypeLIST_ITEM_BULLET_START_2:
      case paraTypeLIST_ITEM_BULLET_START_3:
      case paraTypeLIST_ITEM_BULLET_START_4:
      case paraTypeLIST_ITEM_BULLET_START_5:
      case paraTypeLIST_ITEM_BULLET_START_6:
      case paraTypeLIST_ITEM_BULLET_START_7:
      case paraTypeLIST_ITEM_BULLET_START_8:
      case paraTypeLIST_ITEM_BULLET_START_9:
      case paraTypeLIST_ITEM_NUMBER_START_1:
      case paraTypeLIST_ITEM_NUMBER_START_2:
      case paraTypeLIST_ITEM_NUMBER_START_3:
      case paraTypeLIST_ITEM_NUMBER_START_4:
      case paraTypeLIST_ITEM_NUMBER_START_5:
      case paraTypeLIST_ITEM_NUMBER_START_6:
      case paraTypeLIST_ITEM_NUMBER_START_7:
      case paraTypeLIST_ITEM_NUMBER_START_8:
      case paraTypeLIST_ITEM_NUMBER_START_9:

         gfileSetA(file, gcTypeU1, "\n", NULL);

         lindex = 0;
         switch (para->type)
         {
         case paraTypeLIST_ITEM_BULLET_9:
         case paraTypeLIST_ITEM_NUMBER_9:
         case paraTypeLIST_ITEM_BULLET_START_9:
         case paraTypeLIST_ITEM_NUMBER_START_9:
            //if (listType[lindex] == listTypeNUMBER)
            //{
            //   gfileSetA(file, gcTypeU1, "1", NULL);
            //}
            //else
            //{
            //   gfileSetA(file, gcTypeU1, "*", NULL);
            //}
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_8:
         case paraTypeLIST_ITEM_NUMBER_8:
         case paraTypeLIST_ITEM_BULLET_START_8:
         case paraTypeLIST_ITEM_NUMBER_START_8:
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_7:
         case paraTypeLIST_ITEM_NUMBER_7:
         case paraTypeLIST_ITEM_BULLET_START_7:
         case paraTypeLIST_ITEM_NUMBER_START_7:
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_6:
         case paraTypeLIST_ITEM_NUMBER_6:
         case paraTypeLIST_ITEM_BULLET_START_6:
         case paraTypeLIST_ITEM_NUMBER_START_6:
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_5:
         case paraTypeLIST_ITEM_NUMBER_5:
         case paraTypeLIST_ITEM_BULLET_START_5:
         case paraTypeLIST_ITEM_NUMBER_START_5:
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_4:
         case paraTypeLIST_ITEM_NUMBER_4:
         case paraTypeLIST_ITEM_BULLET_START_4:
         case paraTypeLIST_ITEM_NUMBER_START_4:
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_3:
         case paraTypeLIST_ITEM_NUMBER_3:
         case paraTypeLIST_ITEM_BULLET_START_3:
         case paraTypeLIST_ITEM_NUMBER_START_3:
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_2:
         case paraTypeLIST_ITEM_NUMBER_2:
         case paraTypeLIST_ITEM_BULLET_START_2:
         case paraTypeLIST_ITEM_NUMBER_START_2:
            gfileSetA(file, gcTypeU1, "    ", NULL);
            lindex++;
            // Fall through

         case paraTypeLIST_ITEM_BULLET_1:
         case paraTypeLIST_ITEM_NUMBER_1:
         case paraTypeLIST_ITEM_BULLET_START_1:
         case paraTypeLIST_ITEM_NUMBER_START_1:
            if (listType[lindex] == listTypeNUMBER)
            {
               gfileSetA(file, gcTypeU1, "1. ", NULL);
            }
            else
            {
               gfileSetA(file, gcTypeU1, "*  ", NULL);
            }
            lindex++;
            // Fall through
         }

         switch (para->type)
         {
         case paraTypeLIST_ITEM_BULLET_1:
         case paraTypeLIST_ITEM_BULLET_2:
         case paraTypeLIST_ITEM_BULLET_3:
         case paraTypeLIST_ITEM_BULLET_4:
         case paraTypeLIST_ITEM_BULLET_5:
         case paraTypeLIST_ITEM_BULLET_6:
         case paraTypeLIST_ITEM_BULLET_7:
         case paraTypeLIST_ITEM_BULLET_8:
         case paraTypeLIST_ITEM_BULLET_9:
         case paraTypeLIST_ITEM_NUMBER_1:
         case paraTypeLIST_ITEM_NUMBER_2:
         case paraTypeLIST_ITEM_NUMBER_3:
         case paraTypeLIST_ITEM_NUMBER_4:
         case paraTypeLIST_ITEM_NUMBER_5:
         case paraTypeLIST_ITEM_NUMBER_6:
         case paraTypeLIST_ITEM_NUMBER_7:
         case paraTypeLIST_ITEM_NUMBER_8:
         case paraTypeLIST_ITEM_NUMBER_9:
            stemp = _ProcessInlineMD(para->str, para);
            gfileSetS(file, gcTypeU1, stemp, NULL);
            gfileSetA(file, gcTypeU1, "\n",  NULL);
            gsDestroy(stemp);
            break;
         }
         break;

      case paraTypeLIST_ITEM_BULLET_STOP_1:
      case paraTypeLIST_ITEM_BULLET_STOP_2:
      case paraTypeLIST_ITEM_BULLET_STOP_3:
      case paraTypeLIST_ITEM_BULLET_STOP_4:
      case paraTypeLIST_ITEM_BULLET_STOP_5:
      case paraTypeLIST_ITEM_BULLET_STOP_6:
      case paraTypeLIST_ITEM_BULLET_STOP_7:
      case paraTypeLIST_ITEM_BULLET_STOP_8:
      case paraTypeLIST_ITEM_BULLET_STOP_9:
      case paraTypeLIST_ITEM_NUMBER_STOP_1:
      case paraTypeLIST_ITEM_NUMBER_STOP_2:
      case paraTypeLIST_ITEM_NUMBER_STOP_3:
      case paraTypeLIST_ITEM_NUMBER_STOP_4:
      case paraTypeLIST_ITEM_NUMBER_STOP_5:
      case paraTypeLIST_ITEM_NUMBER_STOP_6:
      case paraTypeLIST_ITEM_NUMBER_STOP_7:
      case paraTypeLIST_ITEM_NUMBER_STOP_8:
      case paraTypeLIST_ITEM_NUMBER_STOP_9:
         gfileSetA(file, gcTypeU1, "\n", NULL);
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

      gfileSetA(file, gcTypeU1, "<br />\n",  NULL);
   }

   // Clean up
   paraArrayDestroy(tocList);

   greturn gbTRUE;
}

/******************************************************************************
func: _WriteZLYT
******************************************************************************/
static Gb _WriteZLYT(Gpath const * const path, ParaArray const * const paraList)
{
   genter;

   path; paraList;

   greturn gbTRUE;
}

/******************************************************************************
func: _WriteRTF
******************************************************************************/
static Gb _WriteRTF(Gpath const * const path, ParaArray const * const paraList)
{
   Gpath       *pathRtf;
   Gfile       *file;
   Gindex       index;
   Para        *para;
   Gs          *stemp;
   Gb           isTableRowActive,
                isTableColHActive,
                isTableColActive;
   ListType     listType[9];
   ListActive   isListActive[9];

   genter;

   isTableRowActive  = gbFALSE;
   isTableColHActive = gbFALSE;
   isTableColActive  = gbFALSE;

   forCount (index, 9)
   {
      listType[index]     = listTypeBULLET;
      isListActive[index] = listActiveNO;
   }
      
   // Make the destination file path.
   pathRtf = gpathCreateFrom(path);
   gpathPopExtension(  pathRtf);
   gpathPushExtensionA(pathRtf, "rtf");

   // Create the file.
   file = gfileOpen(pathRtf, gfileOpenModeREAD_WRITE_NEW);

   gpathDestroy(pathRtf);

   greturnFalseIf(!file);

#define RTF_STYLE_00 "\\s0"  "\\sb36"  "\\sa36"  "\\widctlpar"                                                 "\\li432"  "\\lin432"              "\\ri432"  "\\rin432"  "\\faauto" "\\sl0" "\\slmult1"                                             "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_01 "\\s1"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li0"    "\\lin0"                "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                      "\\brdrb" "\\brdrhair" "\\f1" "\\b0" "\\fs60" "\\strike0" "\\ulnone"
#define RTF_STYLE_02 "\\s2"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li144"  "\\lin144"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs52" "\\strike0" "\\ulnone"
#define RTF_STYLE_03 "\\s3"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li288"  "\\lin288"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs46" "\\strike0" "\\ulnone"
#define RTF_STYLE_04 "\\s4"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li432"  "\\lin432"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs40" "\\strike0" "\\ulnone"
#define RTF_STYLE_05 "\\s5"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li576"  "\\lin576"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs36" "\\strike0" "\\ulnone"
#define RTF_STYLE_06 "\\s6"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li720"  "\\lin720"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs32" "\\strike0" "\\ulnone"
#define RTF_STYLE_07 "\\s7"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li864"  "\\lin864"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs28" "\\strike0" "\\ulnone"
#define RTF_STYLE_08 "\\s8"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li1008" "\\lin1008"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs24" "\\strike0" "\\ulnone"
#define RTF_STYLE_09 "\\s9"  "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn"                              "\\li1152" "\\lin1152"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_10 "\\s10" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li432"  "\\lin432"  "\\fi288"   "\\ri432"  "\\rin432"  "\\faauto" "\\sl0" "\\slmult1"                                             "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_11 "\\s11" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level0" "\\outlinelevel0" "\\li0"    "\\lin0"                "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                      "\\brdrb" "\\brdrhair" "\\f1" "\\b0" "\\fs60" "\\strike0" "\\ulnone"
#define RTF_STYLE_12 "\\s12" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level1" "\\outlinelevel1" "\\li144"  "\\lin144"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs52" "\\strike0" "\\ulnone"
#define RTF_STYLE_13 "\\s13" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level2" "\\outlinelevel2" "\\li288"  "\\lin288"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs46" "\\strike0" "\\ulnone"
#define RTF_STYLE_14 "\\s14" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level3" "\\outlinelevel3" "\\li432"  "\\lin432"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs40" "\\strike0" "\\ulnone"
#define RTF_STYLE_15 "\\s15" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level4" "\\outlinelevel4" "\\li576"  "\\lin576"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs36" "\\strike0" "\\ulnone"
#define RTF_STYLE_16 "\\s16" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level5" "\\outlinelevel5" "\\li720"  "\\lin720"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs32" "\\strike0" "\\ulnone"
#define RTF_STYLE_17 "\\s17" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level6" "\\outlinelevel6" "\\li864"  "\\lin864"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs28" "\\strike0" "\\ulnone"
#define RTF_STYLE_18 "\\s18" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level7" "\\outlinelevel7" "\\li1008" "\\lin1008"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs24" "\\strike0" "\\ulnone"
#define RTF_STYLE_19 "\\s19" "\\sb144" "\\sa144" "\\widctlpar" "\\keep" "\\keepn" "\\level8" "\\outlinelevel8" "\\li1152" "\\lin1152"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1"                                             "\\f1" "\\b0" "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_20 "\\s20" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2880" "\\lin2880" "\\fi-2448" "\\ri432"  "\\rin432"  "\\faauto" "\\sl0" "\\slmult1" "\\tx2880"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_21 "\\s21" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li432"  "\\lin432"              "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1440"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_22 "\\s22" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li576"  "\\lin576"              "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1584"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_23 "\\s23" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li720"  "\\lin720"              "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1728"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_24 "\\s24" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li1728" "\\lin1728"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1872"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_25 "\\s25" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li1872" "\\lin1872"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2016"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_26 "\\s26" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2016" "\\lin2016"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2160"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_27 "\\s27" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2160" "\\lin2160"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2304"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_28 "\\s28" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2304" "\\lin2304"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2448"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_29 "\\s29" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2448" "\\lin2448"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2592"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_30 "\\s30" "\\sb36"  "\\sa36"  "\\widctlpar"                                                 "\\li432"  "\\lin432"              "\\ri432"  "\\rin432"  "\\faauto" "\\sl0" "\\slmult1" "\\nowrap" "\\box" "\\brdrhair" "\\cbpat16" "\\f2"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_31 "\\s31" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li432"  "\\lin432"              "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1440"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_32 "\\s32" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li576"  "\\lin576"              "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1584"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_33 "\\s33" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li720"  "\\lin720"              "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1728"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_34 "\\s34" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li1728" "\\lin1728"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx1872"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_35 "\\s35" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li1872" "\\lin1872"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2016"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_36 "\\s36" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2016" "\\lin2016"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2160"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_37 "\\s37" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2160" "\\lin2160"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2304"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_38 "\\s38" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2304" "\\lin2304"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2448"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_39 "\\s39" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2448" "\\lin2448"             "\\ri1296" "\\rin1296" "\\faauto" "\\sl0" "\\slmult1" "\\tx2592"                                  "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_41 "\\s41" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li432"  "\\lin432"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0" "\\b0" "\\fs20" "\\strike0" "\\ul1"
#define RTF_STYLE_42 "\\s42" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li576"  "\\lin576"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_43 "\\s43" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li720"  "\\lin720"              "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_44 "\\s44" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li1728" "\\lin1728"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_45 "\\s45" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li1872" "\\lin1872"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_46 "\\s46" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2016" "\\lin2016"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_47 "\\s47" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2160" "\\lin2160"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_48 "\\s48" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2304" "\\lin2304"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"
#define RTF_STYLE_49 "\\s49" "\\sb0"   "\\sa0"   "\\widctlpar"                                                 "\\li2448" "\\lin2448"             "\\ri0"    "\\rin0"    "\\faauto" "\\sl0" "\\slmult1" "\\tqr" "\\tldot" "\\tx10080"               "\\f0"        "\\fs20" "\\strike0" "\\ulnone"

   // Write the header
   gfileSetA(
      file,
      gcTypeU1,
      "{\\rtf1\n"
      "\\fbidis"
      "\\deff0\\adeff0"
      "\\deflang1033\n"
      "{\\fonttbl"
      "{\\f0" "\\froman"  "\\fcharset0" "\\fprq2 Times New Roman;}"  // Body text font
      "{\\f1" "\\fswiss"  "\\fcharset0" "\\fprq2 Arial;}"            // Title text font
      "{\\f2" "\\fmodern" "\\fcharset0" "\\fprq1 Lucidia Console;}"  // Code text font
      "}"
      "{\\stylesheet"
      "{" RTF_STYLE_00               "\\snext0"  " Paragraph Technical;}"
      "{" RTF_STYLE_01 "\\sbasedon0" "\\snext0"  " Title 1;}"
      "{" RTF_STYLE_02 "\\sbasedon0" "\\snext0"  " Title 2;}"
      "{" RTF_STYLE_03 "\\sbasedon0" "\\snext0"  " Title 3;}"
      "{" RTF_STYLE_04 "\\sbasedon0" "\\snext0"  " Title 4;}"
      "{" RTF_STYLE_05 "\\sbasedon0" "\\snext0"  " Title 5;}"
      "{" RTF_STYLE_06 "\\sbasedon0" "\\snext0"  " Title 6;}"
      "{" RTF_STYLE_07 "\\sbasedon0" "\\snext0"  " Title 7;}"
      "{" RTF_STYLE_08 "\\sbasedon0" "\\snext0"  " Title 8;}"
      "{" RTF_STYLE_09 "\\sbasedon0" "\\snext0"  " Title 9;}"
      "{" RTF_STYLE_10 "\\sbasedon0" "\\snext0"  " Paragraph Novel;}"
      "{" RTF_STYLE_11 "\\sbasedon0" "\\snext0"  " Title 1 TOC;}"
      "{" RTF_STYLE_12 "\\sbasedon0" "\\snext0"  " Title 2 TOC;}"
      "{" RTF_STYLE_13 "\\sbasedon0" "\\snext0"  " Title 3 TOC;}"
      "{" RTF_STYLE_14 "\\sbasedon0" "\\snext0"  " Title 4 TOC;}"
      "{" RTF_STYLE_15 "\\sbasedon0" "\\snext0"  " Title 5 TOC;}"
      "{" RTF_STYLE_16 "\\sbasedon0" "\\snext0"  " Title 6 TOC;}"
      "{" RTF_STYLE_17 "\\sbasedon0" "\\snext0"  " Title 7 TOC;}"
      "{" RTF_STYLE_18 "\\sbasedon0" "\\snext0"  " Title 8 TOC;}"
      "{" RTF_STYLE_19 "\\sbasedon0" "\\snext0"  " Title 9 TOC;}"
      "{" RTF_STYLE_20 "\\sbasedon0" "\\snext0"  " Paragraph Glossary;}"
      "{" RTF_STYLE_21 "\\sbasedon0" "\\snext21" " List Bullet;}"
      "{" RTF_STYLE_22 "\\sbasedon0" "\\snext22" " List Bullet;}"
      "{" RTF_STYLE_23 "\\sbasedon0" "\\snext23" " List Bullet;}"
      "{" RTF_STYLE_24 "\\sbasedon0" "\\snext24" " List Bullet;}"
      "{" RTF_STYLE_25 "\\sbasedon0" "\\snext25" " List Bullet;}"
      "{" RTF_STYLE_26 "\\sbasedon0" "\\snext26" " List Bullet;}"
      "{" RTF_STYLE_27 "\\sbasedon0" "\\snext27" " List Bullet;}"
      "{" RTF_STYLE_28 "\\sbasedon0" "\\snext28" " List Bullet;}"
      "{" RTF_STYLE_29 "\\sbasedon0" "\\snext29" " List Bullet;}"
      "{" RTF_STYLE_30 "\\sbasedon0" "\\snext0"  " Paragraph Code;}"
      "{" RTF_STYLE_31 "\\sbasedon0" "\\snext31" " List Number;}"
      "{" RTF_STYLE_32 "\\sbasedon0" "\\snext32" " List Number;}"
      "{" RTF_STYLE_33 "\\sbasedon0" "\\snext33" " List Number;}"
      "{" RTF_STYLE_34 "\\sbasedon0" "\\snext34" " List Number;}"
      "{" RTF_STYLE_35 "\\sbasedon0" "\\snext35" " List Number;}"
      "{" RTF_STYLE_36 "\\sbasedon0" "\\snext36" " List Number;}"
      "{" RTF_STYLE_37 "\\sbasedon0" "\\snext37" " List Number;}"
      "{" RTF_STYLE_38 "\\sbasedon0" "\\snext38" " List Number;}"
      "{" RTF_STYLE_39 "\\sbasedon0" "\\snext39" " List Number;}"
      "{" RTF_STYLE_41 "\\sbasedon0" "\\snext41" " TOC 1;}"
      "{" RTF_STYLE_42 "\\sbasedon0" "\\snext42" " TOC 2;}"
      "{" RTF_STYLE_43 "\\sbasedon0" "\\snext43" " TOC 3;}"
      "{" RTF_STYLE_44 "\\sbasedon0" "\\snext44" " TOC 4;}"
      "{" RTF_STYLE_45 "\\sbasedon0" "\\snext45" " TOC 5;}"
      "{" RTF_STYLE_46 "\\sbasedon0" "\\snext46" " TOC 6;}"
      "{" RTF_STYLE_47 "\\sbasedon0" "\\snext47" " TOC 7;}"
      "{" RTF_STYLE_48 "\\sbasedon0" "\\snext48" " TOC 8;}"
      "{" RTF_STYLE_49 "\\sbasedon0" "\\snext49" " TOC 9;}"
      "}"
      "{\\colortbl;"
      "\\red0"   "\\green0"   "\\blue0;"     //  0
      "\\red255" "\\green255" "\\blue255;"   //  1
      "\\red16"  "\\green16"  "\\blue16;"    //  2 
      "\\red32"  "\\green32"  "\\blue32;"    //  3 
      "\\red48"  "\\green48"  "\\blue48;"    //  4
      "\\red64"  "\\green64"  "\\blue64;"    //  5
      "\\red80"  "\\green80"  "\\blue80;"    //  6
      "\\red96"  "\\green96"  "\\blue96;"    //  7
      "\\red112" "\\green112" "\\blue112;"   //  8
      "\\red128" "\\green128" "\\blue128;"   //  9
      "\\red144" "\\green144" "\\blue144;"   // 10
      "\\red160" "\\green160" "\\blue160;"   // 11
      "\\red176" "\\green176" "\\blue176;"   // 12
      "\\red192" "\\green192" "\\blue192;"   // 13
      "\\red208" "\\green208" "\\blue208;"   // 14
      "\\red224" "\\green224" "\\blue224;"   // 15
      "\\red240" "\\green240" "\\blue240;"   // 16
      "}"
      "{\\*\\generator Zekaric:Word v1.0;}"
      "\\doctype0\n" 
      "\\jexpand"
      "{\\*\\wgrffmtfilter 012f}" // 0x1 All styles present. 0x2 Only custom styles to be shown, 0x4 all latent style to be shown, 0x8 only show used styles, 0x20 show all heading styles, 0x100 all unique forms of run level direct formating to be used.
      "\\readonlyrecommended"
      "\\viewkind1\n"
      "\\nospaceforul\\sprsspbf\\nolnhtadjtbl\\alntblind\\lyttblrtgr\\dntblnsbdb\\noxlattoyen\\wrppunct"
      "\\nobrkwrptbl\\expshrtn\\snaptogridincell\\asianbrkrule\\noultrlspc\\useltbaln\\splytwnine"
      "\\ftnlytwnine\\lytcalctblwd\\lnbrkrule\\nouicompat\\nofeaturethrottle1\\nojkernpunct\n"
      "\\pgbrdrhead\\pgbrdrfoot\n"
      "\\sectd\\sectlinegrid360"
      "\\pgwsxn12240\\pghsxn15480\\marglsxn720\\margrsxn720\\margtsxn720\\margbsxn720\\guttersxn0" // letter .5" margin
      "\\headery708\\footery708"
      "\\ltrsect",
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
         gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL);

         stemp = _ProcessInlineRTF(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

         gfileSetA(file, gcTypeU1, "\\par\n", NULL);
         break;

      case paraTypeKEY_VALUE:
         gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_20 "\\ilvl0\n", NULL);

         stemp = _ProcessInlineRTF(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

         gfileSetA(file, gcTypeU1, "\\par\n", NULL);
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
         case paraTypeTITLE_1:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_01 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_11 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_2:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_02 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_12 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_3:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_03 "\\ilvl0\n", NULL); break; 
         case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_13 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_4:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_04 "\\ilvl0\n", NULL); break;                                                        
         case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_14 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_5:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_05 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_15 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_6:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_06 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_16 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_7:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_07 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_TOC_7: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_17 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_8:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_08 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_TOC_8: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_18 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_9:     gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_09 "\\ilvl0\n", NULL); break;
         case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_19 "\\ilvl0\n", NULL); break;
         }

         stemp = _ProcessInlineRTF(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

         gfileSetA(file, gcTypeU1, "\\par\n", NULL);
         break;

      case paraTypeLIST_ITEM_BULLET_1:
      case paraTypeLIST_ITEM_BULLET_2:
      case paraTypeLIST_ITEM_BULLET_3:
      case paraTypeLIST_ITEM_BULLET_4:
      case paraTypeLIST_ITEM_BULLET_5:
      case paraTypeLIST_ITEM_BULLET_6:
      case paraTypeLIST_ITEM_BULLET_7:
      case paraTypeLIST_ITEM_BULLET_8:
      case paraTypeLIST_ITEM_BULLET_9:
         switch (para->type)
         {
         case paraTypeLIST_ITEM_BULLET_1: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_21 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_2: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_22 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_3: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_23 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_4: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_24 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_5: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_25 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_6: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_26 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_7: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_27 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_8: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_28 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_9: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_29 "\\ilvl0\n\\bullet  ", NULL); break;
         }

         stemp = _ProcessInlineRTF(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

         gfileSetA(file, gcTypeU1, "\\par\n", NULL);
         break;

      case paraTypeLIST_ITEM_NUMBER_1:
      case paraTypeLIST_ITEM_NUMBER_2:
      case paraTypeLIST_ITEM_NUMBER_3:
      case paraTypeLIST_ITEM_NUMBER_4:
      case paraTypeLIST_ITEM_NUMBER_5:
      case paraTypeLIST_ITEM_NUMBER_6:
      case paraTypeLIST_ITEM_NUMBER_7:
      case paraTypeLIST_ITEM_NUMBER_8:
      case paraTypeLIST_ITEM_NUMBER_9:
         switch (para->type)
         {
         case paraTypeLIST_ITEM_BULLET_1: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_21 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_2: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_22 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_3: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_23 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_4: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_24 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_5: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_25 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_6: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_26 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_7: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_27 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_8: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_28 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_9: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_29 "\\ilvl0\n# ", NULL); break;
         }

         stemp = _ProcessInlineRTF(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

         gfileSetA(file, gcTypeU1, "\\par\n", NULL);
         break;

      case paraTypeLIST_ITEM_BULLET_START_1:
      case paraTypeLIST_ITEM_BULLET_START_2:
      case paraTypeLIST_ITEM_BULLET_START_3:
      case paraTypeLIST_ITEM_BULLET_START_4:
      case paraTypeLIST_ITEM_BULLET_START_5:
      case paraTypeLIST_ITEM_BULLET_START_6:
      case paraTypeLIST_ITEM_BULLET_START_7:
      case paraTypeLIST_ITEM_BULLET_START_8:
      case paraTypeLIST_ITEM_BULLET_START_9:
         switch (para->type)
         {
         case paraTypeLIST_ITEM_BULLET_1: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_21 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_2: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_22 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_3: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_23 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_4: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_24 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_5: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_25 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_6: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_26 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_7: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_27 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_8: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_28 "\\ilvl0\n\\bullet  ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_9: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_29 "\\ilvl0\n\\bullet  ", NULL); break;
         }
         break;

      case paraTypeLIST_ITEM_NUMBER_START_1:
      case paraTypeLIST_ITEM_NUMBER_START_2:
      case paraTypeLIST_ITEM_NUMBER_START_3:
      case paraTypeLIST_ITEM_NUMBER_START_4:
      case paraTypeLIST_ITEM_NUMBER_START_5:
      case paraTypeLIST_ITEM_NUMBER_START_6:
      case paraTypeLIST_ITEM_NUMBER_START_7:
      case paraTypeLIST_ITEM_NUMBER_START_8:
      case paraTypeLIST_ITEM_NUMBER_START_9:
         switch (para->type)
         {
         case paraTypeLIST_ITEM_BULLET_1: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_21 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_2: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_22 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_3: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_23 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_4: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_24 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_5: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_25 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_6: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_26 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_7: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_27 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_8: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_28 "\\ilvl0\n# ", NULL); break;
         case paraTypeLIST_ITEM_BULLET_9: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_29 "\\ilvl0\n# ", NULL); break;
         }
         break;

      case paraTypeLIST_ITEM_BULLET_STOP_1:
      case paraTypeLIST_ITEM_BULLET_STOP_2:
      case paraTypeLIST_ITEM_BULLET_STOP_3:
      case paraTypeLIST_ITEM_BULLET_STOP_4:
      case paraTypeLIST_ITEM_BULLET_STOP_5:
      case paraTypeLIST_ITEM_BULLET_STOP_6:
      case paraTypeLIST_ITEM_BULLET_STOP_7:
      case paraTypeLIST_ITEM_BULLET_STOP_8:
      case paraTypeLIST_ITEM_BULLET_STOP_9:
      case paraTypeLIST_ITEM_NUMBER_STOP_1:
      case paraTypeLIST_ITEM_NUMBER_STOP_2:
      case paraTypeLIST_ITEM_NUMBER_STOP_3:
      case paraTypeLIST_ITEM_NUMBER_STOP_4:
      case paraTypeLIST_ITEM_NUMBER_STOP_5:
      case paraTypeLIST_ITEM_NUMBER_STOP_6:
      case paraTypeLIST_ITEM_NUMBER_STOP_7:
      case paraTypeLIST_ITEM_NUMBER_STOP_8:
      case paraTypeLIST_ITEM_NUMBER_STOP_9:
         gfileSetA(file, gcTypeU1, "\\par\n", NULL);
         break;

      case paraTypeTABLE_START:
         //gfileSetA(file, gcTypeU1, "<table class=\"zword\"><tbody class=\"zword\">\n", NULL);
         isTableRowActive  = gbFALSE;
         isTableColActive  = gbFALSE;
         isTableColHActive = gbFALSE;
         break;

      case paraTypeTABLE_STOP:
         //if (isTableColActive)
         //{
         //   gfileSetA(file, gcTypeU1, "</td>\n", NULL);
         //}
         //if (isTableColHActive)
         //{
         //   gfileSetA(file, gcTypeU1, "</th>\n", NULL);
         //}
         //if (isTableRowActive)
         //{
         //   gfileSetA(file, gcTypeU1, "\n</tr>\n", NULL);
         //}

         //gfileSetA(file, gcTypeU1, "\n</tbody></table>\n", NULL);
         isTableRowActive = gbFALSE;
         isTableColActive = gbFALSE;
         break;

      case paraTypeTABLE_ROW:
         //if (isTableColActive)
         //{
         //   gfileSetA(file, gcTypeU1, "</td>\n", NULL);
         //}
         //if (isTableColHActive)
         //{
         //   gfileSetA(file, gcTypeU1, "</th>\n", NULL);
         //}
         //if (isTableRowActive)
         //{
         //   gfileSetA(file, gcTypeU1, "\n</tr>\n", NULL);
         //}

         //gfileSetA(file, gcTypeU1, "<tr class=\"zword\">\n", NULL);
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
         //if (isTableColHActive)
         //{
         //   gfileSetA(file, gcTypeU1, "</th>\n", NULL);
         //}
         //if (isTableColActive)
         //{
         //   gfileSetA(file, gcTypeU1, "</td>\n", NULL);
         //}


         switch (para->type)
         {
         case paraTypeTABLE_COL_HEADER:            gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL); break;
         case paraTypeTABLE_COL_HEADER_NO_BREAK:   gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL); break;
         case paraTypeTABLE_COL_HEADER_FILL:       gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL); break;
         case paraTypeTABLE_COL:                   gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL); break;
         case paraTypeTABLE_COL_NO_BREAK:          gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL); break;
         case paraTypeTABLE_COL_FILL:              gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL); break;
         }

         stemp = _ProcessInlineRTF(para->str, para);
         gfileSetS(file, gcTypeU1, stemp, NULL);
         gsDestroy(stemp);

         gfileSetA(file, gcTypeU1, "\\par\n", NULL);

         //switch (para->type)
         //{
         //case paraTypeTABLE_COL_HEADER:            
         //case paraTypeTABLE_COL_HEADER_FILL:       gfileSetA(file, gcTypeU1, "</th>\n",        NULL); break;
         //case paraTypeTABLE_COL_HEADER_NO_BREAK:   gfileSetA(file, gcTypeU1, "</nobr></th>\n", NULL); break;
         //case paraTypeTABLE_COL:            
         //case paraTypeTABLE_COL_FILL:              gfileSetA(file, gcTypeU1, "</td>\n",        NULL); break;
         //case paraTypeTABLE_COL_NO_BREAK:          gfileSetA(file, gcTypeU1, "</nobr></td>\n", NULL); break;
         //}
         
         isTableColHActive = gbFALSE;
         break;

      case paraTypeFORMATED_START:
         gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_30 "\\ilvl0\n ", NULL);

         gfileSetS(file, gcTypeU1, para->str, NULL);

         gfileSetA(file, gcTypeU1, "\\par\n", NULL);
         break;

      case paraTypeTABLE_OF_CONTENTS:
         _WriteRTFTOC(file, paraList);
         break;

      case paraTypePAGE_BREAK:
         // Close off any applicable lists.
         gfileSetA(file, gcTypeU1, "\\page\n", NULL);
         break;
      }
   }

   // Write the terminators.
   gfileSetA(file, gcTypeU1, "}", NULL);

   // Clean up.
   gfileClose(file);

   greturn gbTRUE;
}

/******************************************************************************
func: _WriteRTFTOC
******************************************************************************/
static Gb _WriteRTFTOC(Gfile * const file, ParaArray const * const paraList)
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
      case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_41 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_2: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_42 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_3: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_43 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_4: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_44 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_5: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_45 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_6: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_46 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_7: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_47 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_8: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_48 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_9: gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_49 "\\ilvl0\n",  NULL); break;
      }

      stemp = _ProcessInlineRTF(para->str, para);
      gfileSetS(file, gcTypeU1, stemp, NULL);
      gsDestroy(stemp);

      gfileSetA(file, gcTypeU1, "\\par\n", NULL);

      // Clean up
      gsDestroy(chapter);
   }

   // Clean up
   paraArrayDestroy(tocList);

   greturn gbTRUE;
}

