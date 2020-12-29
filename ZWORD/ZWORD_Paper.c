/******************************************************************************
file:       ZWORD_Paper
author:     Robbert de Groot
copyright:  2020, 

description:
Loads in the paper definition and stylying.
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "ZWORD.h"

/******************************************************************************
local:
constant:
******************************************************************************/

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/
static Paper       _paper;
static PaperPara   _paperPara[paraStyleCOUNT];
static Gs         *_paperFont[paraFontStyleCOUNT];

/******************************************************************************
prototype:
******************************************************************************/
static void _ReadFont(  Gs * const value);
static void _ReadPaper( Gs * const value);
static void _ReadPara(  Gs * const value, ParaStyle const style);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: paperGet
******************************************************************************/
Paper *PaperGet(void)
{
   return &_paper;
}

/******************************************************************************
func: paperFontGet
******************************************************************************/
Gs *PaperFontGet(ParaFontStyle style)
{
   return _paperFont[style];
}

/******************************************************************************
func: PaperParaGet
******************************************************************************/
PaperPara *PaperParaGet(ParaStyle style)
{
   return &_paperPara[style];
}

/******************************************************************************
func: PaperLoad
******************************************************************************/
void PaperLoad(Gs const * const filePaper)
{
   ParaType     type;
   Gpath       *path;
   Gfile       *file;
   GfilePref   *pref;
   Gs          *key,
               *value;

   genter;

   file = NULL;
   pref = NULL;
   path = gpathCreateFrom(filePaper);
   if (!path ||
       !gpathSetFromSystem(path))
   {
      gpathDestroy(path);
      gconSetA("ERROR: Path for paper file is mal formed.  Using defaults.\n");
      greturn;
   }

   // Open the file.
   loopOnce
   {
      file = gfileOpen(path, gfileOpenModeREAD_ONLY);
      if (!file)
      {
         gconSetA("ERROR: File for paper file could not be opened.  Using defaults.\n");
         break;
      }

      // Start reading the preference file.
      pref = gfilePrefCreate(gfilePrefModeFILE, file);
      if (!pref)
      {
         gconSetA("Error: File for paper file could not be properly read.  Using defaults.\n");
         break;
      }

      loop
      {
         breakIf(!gfilePrefGet(pref));

         key   = gfilePrefGetKey(  pref);
         value = gfilePrefGetValue(pref);

         if      (gsIsEqualA(key, "paper"))
         {
            _ReadPaper(value);
         }
         else if (gsIsEqualA(key, "font"))
         {
            _ReadFont(value);
         }
         else 
         {
            type = paraStyleREGULAR;
            if      (gsIsEqualA(key, "para"))              type = paraStyleREGULAR;
            else if (gsIsEqualA(key, "paraCode"))          type = paraStyleFORMATTED;
            else if (gsIsEqualA(key, "paraKeyValue"))      type = paraStyleKEY_VALUE;
            else if (gsIsEqualA(key, "paraTitle1"))        type = paraStyleTITLE_1;
            else if (gsIsEqualA(key, "paraTitle2"))        type = paraStyleTITLE_2;
            else if (gsIsEqualA(key, "paraTitle3"))        type = paraStyleTITLE_3;
            else if (gsIsEqualA(key, "paraTitle4"))        type = paraStyleTITLE_4;
            else if (gsIsEqualA(key, "paraTitle5"))        type = paraStyleTITLE_5;
            else if (gsIsEqualA(key, "paraTitle1Toc"))     type = paraStyleTITLE_1_TOC;
            else if (gsIsEqualA(key, "paraTitle2Toc"))     type = paraStyleTITLE_2_TOC;
            else if (gsIsEqualA(key, "paraTitle3Toc"))     type = paraStyleTITLE_3_TOC;
            else if (gsIsEqualA(key, "paraTitle4Toc"))     type = paraStyleTITLE_4_TOC;
            else if (gsIsEqualA(key, "paraTitle5Toc"))     type = paraStyleTITLE_5_TOC;
            else if (gsIsEqualA(key, "paraToc1"))          type = paraStyleTOC_1;
            else if (gsIsEqualA(key, "paraToc2"))          type = paraStyleTOC_2;
            else if (gsIsEqualA(key, "paraToc3"))          type = paraStyleTOC_3;
            else if (gsIsEqualA(key, "paraToc4"))          type = paraStyleTOC_4;
            else if (gsIsEqualA(key, "paraToc5"))          type = paraStyleTOC_5;
            else if (gsIsEqualA(key, "paraListBullet1"))   type = paraStyleLIST_BULLET_1;
            else if (gsIsEqualA(key, "paraListBullet2"))   type = paraStyleLIST_BULLET_2;
            else if (gsIsEqualA(key, "paraListBullet3"))   type = paraStyleLIST_BULLET_3;
            else if (gsIsEqualA(key, "paraListBullet4"))   type = paraStyleLIST_BULLET_4;
            else if (gsIsEqualA(key, "paraListBullet5"))   type = paraStyleLIST_BULLET_5;
            else if (gsIsEqualA(key, "paraListBullet6"))   type = paraStyleLIST_BULLET_6;
            else if (gsIsEqualA(key, "paraListBullet7"))   type = paraStyleLIST_BULLET_7;
            else if (gsIsEqualA(key, "paraListBullet8"))   type = paraStyleLIST_BULLET_8;
            else if (gsIsEqualA(key, "paraListBullet9"))   type = paraStyleLIST_BULLET_9;
            else if (gsIsEqualA(key, "paraListNumber1"))   type = paraStyleLIST_NUMBER_1;
            else if (gsIsEqualA(key, "paraListNumber2"))   type = paraStyleLIST_NUMBER_2;
            else if (gsIsEqualA(key, "paraListNumber3"))   type = paraStyleLIST_NUMBER_3;
            else if (gsIsEqualA(key, "paraListNumber4"))   type = paraStyleLIST_NUMBER_4;
            else if (gsIsEqualA(key, "paraListNumber5"))   type = paraStyleLIST_NUMBER_5;
            else if (gsIsEqualA(key, "paraListNumber6"))   type = paraStyleLIST_NUMBER_6;
            else if (gsIsEqualA(key, "paraListNumber7"))   type = paraStyleLIST_NUMBER_7;
            else if (gsIsEqualA(key, "paraListNumber8"))   type = paraStyleLIST_NUMBER_8;
            else if (gsIsEqualA(key, "paraListNumber9"))   type = paraStyleLIST_NUMBER_9;

            _ReadPara(value, type);
         }
      }
   }

   // Cleanup
   gfilePrefDestroy(pref);
   gfileClose(file);
   gpathDestroy(path);

   greturn;
}

/******************************************************************************
func: PaperStart
******************************************************************************/
void PaperStart(void)
{
   Gindex index;

   genter;

   _paperFont[paraFontStyleSANS]  = gsCreateFromA("Arial");
   _paperFont[paraFontStyleSERIF] = gsCreateFromA("Times New Roman");
   _paperFont[paraFontStyleMONO]  = gsCreateFromA("Lucidia Console");

   _paper.height_MM     = 11.0 * GrMM_PER_INCH;
   _paper.width_MM      =  8.5 * GrMM_PER_INCH;
   _paper.marginB_MM    =
      _paper.marginL_MM =
      _paper.marginR_MM =
      _paper.marginT_MM = 0.5 * GrMM_PER_INCH;

   forCount (index, paraStyleCOUNT)
   {
      _paperPara[index].borderBSize_MM    =
         _paperPara[index].borderLSize_MM =
         _paperPara[index].borderRSize_MM =
         _paperPara[index].borderTSize_MM = 0.0;
      _paperPara[index].fontStyle         = paraFontStyleSANS;
      _paperPara[index].fontIsBold        =
         _paperPara[index].fontIsItalic   = gbFALSE;
      _paperPara[index].fontSize_Point    = 8.0;
      _paperPara[index].indentL_MM        = 
         _paperPara[index].indentR_MM     = 0.0;
      _paperPara[index].background        = -1;
   }

   greturn;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _ReadFont
******************************************************************************/
static void _ReadFont(Gs * const prefValue)
{
   GfilePref *pref;
   Gs        *key,
             *value;

   genter;

   pref = gfilePrefCreate(gfilePrefModeSTR, prefValue);
   if (!pref)
   {
      greturn;
   }

   loop
   {
      breakIf(!gfilePrefGet(pref));

      key   = gfilePrefGetKey(  pref);
      value = gfilePrefGetValue(pref);

      if      (gsIsEqualA(key, "sans"))
      {
         gsSet(_paperFont[paraFontStyleSANS], value);
      }
      else if (gsIsEqualA(key, "serif"))
      {
         gsSet(_paperFont[paraFontStyleSERIF], value);
      }
      else if (gsIsEqualA(key, "mono"))
      {
         gsSet(_paperFont[paraFontStyleMONO], value);
      }
   }

   // Clean up
   gfilePrefDestroy(pref);

   greturn;
}

/******************************************************************************
func: _ReadPaper
******************************************************************************/
static void _ReadPaper(Gs * const prefValue)
{
   GfilePref *pref;
   Gs        *key,
             *value,
             *unit;
   GsArray   *sarrayTemp;
   Gr         conversion;
   Gc         space;

   genter;

   space = gcFromA(' ');

   pref = gfilePrefCreate(gfilePrefModeSTR, prefValue);
   if (!pref)
   {
      greturn;
   }

   loop
   {
      breakIf(!gfilePrefGet(pref));

      key   = gfilePrefGetKey(  pref);
      value = gfilePrefGetValue(pref);

      if      (gsIsEqualA(key, "size"))
      {
         sarrayTemp = gsCreateSplit(value, space);

         // Get the units.
         conversion = 1.0;
         unit = gsArrayGetAt(sarrayTemp, 0);
         if (gsIsEqualA(unit, "inch"))
         {
            conversion = GrMM_PER_INCH;
         }

         // Get the width and height of the paper.
         _paper.width_MM  = gsGetR(gsArrayGetAt(sarrayTemp, 1)) * conversion;
         _paper.height_MM = gsGetR(gsArrayGetAt(sarrayTemp, 2)) * conversion;

         gsArrayDestroy(sarrayTemp);
      }
      else if (gsIsEqualA(key, "margin"))
      {
         sarrayTemp = gsCreateSplit(value, space);

         // Get the units.
         conversion = 1.0;
         unit = gsArrayGetAt(sarrayTemp, 0);
         if (gsIsEqualA(unit, "inch"))
         {
            conversion = GrMM_PER_INCH;
         }

         // Get the left, right, top, and bottom margins of the paper.
         _paper.marginL_MM = gsGetR(gsArrayGetAt(sarrayTemp, 1)) * conversion;
         _paper.marginR_MM = gsGetR(gsArrayGetAt(sarrayTemp, 2)) * conversion;
         _paper.marginT_MM = gsGetR(gsArrayGetAt(sarrayTemp, 3)) * conversion;
         _paper.marginB_MM = gsGetR(gsArrayGetAt(sarrayTemp, 4)) * conversion;

         gsArrayDestroy(sarrayTemp);
      }
   }

   // Clean up
   gfilePrefDestroy(pref);

   greturn;
}

/******************************************************************************
func: _ReadPara
******************************************************************************/
static void _ReadPara(Gs * const prefValue, ParaStyle const style)
{
   GfilePref *pref;
   Gs        *key,
             *value;
   GsArray   *sarrayTemp;
   Gr         conversion;
   Gc         space;

   genter;

   space = gcFromA(' ');

   pref = gfilePrefCreate(gfilePrefModeSTR, prefValue);
   if (!pref)
   {
      greturn;
   }

   loop
   {
      breakIf(!gfilePrefGet(pref));

      key   = gfilePrefGetKey(  pref);
      value = gfilePrefGetValue(pref);

      if      (gsIsEqualA(key, "font"))
      {
         sarrayTemp = gsCreateSplit(value, space);

         // Get the units.
         conversion = 1.0;
         if      (gsIsEqualA(gsArrayGetAt(sarrayTemp, 0), "inch"))
         {
            conversion = GrPOINT_PER_INCH;
         }
         else if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 0), "mm"))
         {
            conversion = GrPOINT_PER_MM;
         }

         // Get the width and height of the paper.
         _paperPara[style].fontSize_Point = gsGetR(gsArrayGetAt(sarrayTemp, 1)) * conversion;

         // Get the attribute plain/bold/bold-italic/italic
         if      (gsIsEqualA(gsArrayGetAt(sarrayTemp, 2), "bold"))
         {
            _paperPara[style].fontIsBold = gbTRUE;
         }
         else if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 2), "italic"))
         {
            _paperPara[style].fontIsItalic = gbTRUE;
         }
         else if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 2), "bold-italic"))
         {
            _paperPara[style].fontIsBold      =
               _paperPara[style].fontIsItalic = gbTRUE;
         }

         // Get the font family
         _paperPara[style].fontStyle = paraFontStyleSANS;
         if      (gsIsEqualA(gsArrayGetAt(sarrayTemp, 3), "serif"))
         {
            _paperPara[style].fontStyle = paraFontStyleSERIF;
         }
         else if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 3), "mono"))
         {
            _paperPara[style].fontStyle = paraFontStyleMONO;
         }

         gsArrayDestroy(sarrayTemp);
      }
      else if (gsIsEqualA(key, "indent"))
      {
         sarrayTemp = gsCreateSplit(value, space);

         // Get the units.
         conversion = 1.0;
         if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 0), "inch"))
         {
            conversion = GrMM_PER_INCH;
         }         

         // Get the left, right, top, and bottom margins of the paper.
         _paperPara[style].indentL_MM = gsGetR(gsArrayGetAt(sarrayTemp, 1)) * conversion;
         _paperPara[style].indentR_MM = gsGetR(gsArrayGetAt(sarrayTemp, 2)) * conversion;

         gsArrayDestroy(sarrayTemp);
      }
      else if (gsIsEqualA(key, "indent1"))
      {
         sarrayTemp = gsCreateSplit(value, space);

         // Get the units.
         conversion = 1.0;
         if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 0), "inch"))
         {
            conversion = GrMM_PER_INCH;
         }

         // Get the first line indent value.
         _paperPara[style].indentL_MM_FirstLine = gsGetR(gsArrayGetAt(sarrayTemp, 1)) * conversion;

         gsArrayDestroy(sarrayTemp);
      }
      else if (gsIsEqualA(key, "border"))
      {
         sarrayTemp = gsCreateSplit(value, space);

         // Get the units
         conversion = 1.0;
         if      (gsIsEqualA(gsArrayGetAt(sarrayTemp, 0), "inch"))
         {
            conversion = GrPOINT_PER_INCH;
         }
         else if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 0), "mm"))
         {
            conversion = GrPOINT_PER_MM;
         }

         _paperPara[style].borderLSize_MM = gsGetR(gsArrayGetAt(sarrayTemp, 1)) * conversion;
         _paperPara[style].borderRSize_MM = gsGetR(gsArrayGetAt(sarrayTemp, 2)) * conversion;
         _paperPara[style].borderTSize_MM = gsGetR(gsArrayGetAt(sarrayTemp, 3)) * conversion;
         _paperPara[style].borderBSize_MM = gsGetR(gsArrayGetAt(sarrayTemp, 4)) * conversion;
      }
      else if (gsIsEqualA(key, "colorB"))
      {
         _paperPara[style].background = (Gindex) gsGetI(value);
      }
      else if (gsIsEqualA(key, "tabStop"))
      {
         sarrayTemp = gsCreateSplit(value, space);

         // Get the units.
         conversion = 1.0;
         if (gsIsEqualA(gsArrayGetAt(sarrayTemp, 0), "inch"))
         {
            conversion = GrMM_PER_INCH;
         }         

         _paperPara[style].tabStop_MM = gsGetR(gsArrayGetAt(sarrayTemp, 1)) * conversion;
      }
   }

   // Clean up
   gfilePrefDestroy(pref);

   greturn;
}
