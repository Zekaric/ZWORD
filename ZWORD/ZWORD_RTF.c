/******************************************************************************
file:       ZWORD_RTF
author:     Robbert de Groot
copyright:  2020, Robbert de Groot
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "ZWORD.h"

/******************************************************************************
local:
constant:
******************************************************************************/
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

/******************************************************************************
prototype:
******************************************************************************/
static Gs      *_ProcessInline(                 Gs          * const inStr, Para const * const para);

static Gfile   *_FileOpen(                      Gpath const * const path);
static Gb       _FileWriteComment(              Gfile       * const file,  Para          const * const para);
static Gb       _FileWriteFormatFooter(         Gfile       * const file);
static Gb       _FileWriteFormatHeader(         Gfile       * const file,  Gs            const * const title);
static Gb       _FileWriteKeyValue(             Gfile       * const file,  Para          const * const para);
static Gb       _FileWritePageBreak(            Gfile       * const file);
static Gb       _FileWriteParagraph(            Gfile       * const file,  Para          const * const para);
static Gb       _FileWriteScopeStart(           Gfile       * const file,  ParaType              const type, Gindex const indentLevel, ParaType const parentType);
static Gb       _FileWriteScopeStop(            Gfile       * const file,  ParaType              const type, ParaType const parentType);
static Gb       _FileWriteString(               Gfile       * const file,  Para          const * const para);
static Gb       _FileWriteStringUnaltered(      Gfile       * const file,  Para          const * const para);
static Gb       _FileWriteTableHeaderSeparator( Gfile       * const file,  ParaTypeArray const * const headerList);
static Gb       _FileWriteTitle(                Gfile       * const file,  Para          const * const para);
static Gb       _FileWriteTOC(                  Gfile       * const file,  ParaArray     const * const tocList);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: GetFunctionsRTF
******************************************************************************/
WriteFunctions GetFunctionsRTF(void)
{
   WriteFunctions func;

   func.FileOpen                 = _FileOpen;
   func.FileWriteComment         = _FileWriteComment;
   func.FileWriteFormatFooter    = _FileWriteFormatFooter;
   func.FileWriteFormatHeader    = _FileWriteFormatHeader;
   func.FileWriteKeyValue        = _FileWriteKeyValue;
   func.FileWritePageBreak       = _FileWritePageBreak;
   func.FileWriteParagraph       = _FileWriteParagraph;
   func.FileWriteScopeStart      = _FileWriteScopeStart;
   func.FileWriteScopeStop       = _FileWriteScopeStop;
   func.FileWriteString          = _FileWriteString;
   func.FileWriteStringUnaltered = _FileWriteStringUnaltered;
   func.FileWriteTitle           = _FileWriteTitle;
   func.FileWriteTOC             = _FileWriteTOC;

   return func;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _ProcessInline

Process inline markup.
******************************************************************************/
static Gs *_ProcessInline(Gs * const inStr, Para const * const para)
{
   Gs *str;

   genter;

   str = gsCreateFrom(inStr);

   gsFindAndReplaceU2(str, L"{",                        L"\\{",                  NULL);
   gsFindAndReplaceU2(str, L"}",                        L"\\}",                  NULL);

   gsFindAndReplaceU2(str, L"|chapter|",                gsGet(para->chapterStr), NULL);

   // Styling
   gsFindAndReplaceU2(str, L"|img |",                   L"(",                    NULL);
   gsFindAndReplaceU2(str, L"| img|",                   L")",                    NULL);
   gsFindAndReplaceU2(str, L"|link |",                  L"[",                    NULL);
   gsFindAndReplaceU2(str, L"| link |",                 L"|",                    NULL);
   gsFindAndReplaceU2(str, L"| link|",                  L"]",                    NULL);
   gsFindAndReplaceU2(str, L"|line|",                   L"\\line ",              NULL);
   gsFindAndReplaceU2(str, L"|line |",                  L"\\line\\line ",        NULL);
   gsFindAndReplaceU2(str, L"|line  |",                 L"\\line\\line\\line ",  NULL);

   // Legal
   gsFindAndReplaceU2(str, L"|*copyright|",             L"\\u169X",              NULL);
   gsFindAndReplaceU2(str, L"|*registered tm|",         L"\\u174X",              NULL);
   gsFindAndReplaceU2(str, L"|*tm|",                    L"\\u8482X",             NULL);

   // Currency
   gsFindAndReplaceU2(str, L"|*bitcoin|",               L"\\u8383B",             NULL);
   gsFindAndReplaceU2(str, L"|*cent|",                  L"\\u162c",              NULL);
   gsFindAndReplaceU2(str, L"|*dollar|",                L"$",                    NULL);
   gsFindAndReplaceU2(str, L"|*euro|",                  L"\\u8364E",             NULL);
   gsFindAndReplaceU2(str, L"|*franc|",                 L"\\u8355F",             NULL);
   gsFindAndReplaceU2(str, L"|*lira|",                  L"\\u8356L",             NULL);
   gsFindAndReplaceU2(str, L"|*lira turkey|",           L"\\u8378L",             NULL);
   gsFindAndReplaceU2(str, L"|*peso|",                  L"\\u8369P",             NULL);
   gsFindAndReplaceU2(str, L"|*pound|",                 L"\\u163L",              NULL);
   gsFindAndReplaceU2(str, L"|*ruble|",                 L"\\u8381R",             NULL);
   gsFindAndReplaceU2(str, L"|*rupee|",                 L"\\u8360R",             NULL);
   gsFindAndReplaceU2(str, L"|*rupee india|",           L"\\u8377R",             NULL);
   gsFindAndReplaceU2(str, L"|*won|",                   L"\\u8361W",             NULL);
   gsFindAndReplaceU2(str, L"|*yen|",                   L"\\u165Y",              NULL);

   // Punctuation
   gsFindAndReplaceU2(str, L"|*...|",                   L"\\u8230W",             NULL);
   gsFindAndReplaceU2(str, L"|*1/4|",                   L"\\u188X",              NULL);
   gsFindAndReplaceU2(str, L"|*1/2|",                   L"\\u189X",              NULL);
   gsFindAndReplaceU2(str, L"|*3/4|",                   L"\\u190X",              NULL);
   gsFindAndReplaceU2(str, L"|*ampersand|",             L"&",                    NULL);
   gsFindAndReplaceU2(str, L"|*at|",                    L"@",                    NULL);
   gsFindAndReplaceU2(str, L"|*bullet|",                L"\\bullet ",            NULL);
   gsFindAndReplaceU2(str, L"|*dagger s|",              L"\\u8224X",             NULL);
   gsFindAndReplaceU2(str, L"|*dagger d|",              L"\\u8225X",             NULL);
   gsFindAndReplaceU2(str, L"|*hash|",                  L"#",                    NULL);
   gsFindAndReplaceU2(str, L"|*inv!|",                  L"\\u161!",              NULL);
   gsFindAndReplaceU2(str, L"|*inv?|",                  L"\\u191?",              NULL);
   gsFindAndReplaceU2(str, L"|*quote angle d l|",       L"\\u171X",              NULL);
   gsFindAndReplaceU2(str, L"|*quote angle d r|",       L"\\u187X",              NULL);
   gsFindAndReplaceU2(str, L"|*quote angle s l|",       L"\\u8249X",             NULL);
   gsFindAndReplaceU2(str, L"|*quote angle s r|",       L"\\u8250X",             NULL);
   gsFindAndReplaceU2(str, L"|*quote d|",               L"\"",                   NULL);
   gsFindAndReplaceU2(str, L"|*quote d l|",             L"\\u8220X",             NULL);
   gsFindAndReplaceU2(str, L"|*quote d r|",             L"\\u8221X",             NULL);
   gsFindAndReplaceU2(str, L"|*quote d low|",           L"\\u8222X",             NULL);
   gsFindAndReplaceU2(str, L"|*quote s|",               L"'",                    NULL);
   gsFindAndReplaceU2(str, L"|*quote s l|",             L"\\u8216X",             NULL);
   gsFindAndReplaceU2(str, L"|*quote s r|",             L"\\u8217X",             NULL);
   gsFindAndReplaceU2(str, L"|*quote s low|",           L"\\u8218X",             NULL);
   gsFindAndReplaceU2(str, L"|*para|",                  L"\\u182X",              NULL);
   gsFindAndReplaceU2(str, L"|*prime d|",               L"\\u8242X",             NULL);
   gsFindAndReplaceU2(str, L"|*prime s|",               L"\\u8243X",             NULL);

   // Game
   gsFindAndReplaceU2(str, L"|*arrow d|",               L"\\u8595X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow l|",               L"\\u8592X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow r|",               L"\\u8594X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow u|",               L"\\u8593X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow lr|",              L"\\u8596X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow ud|",              L"\\u8597X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow \\d|",             L"\\u8600X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow /d|",              L"\\u8601X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow \\u|",             L"\\u8598X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow /u|",              L"\\u8599X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow d l|",             L"\\u8656X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow d lr|",            L"\\u8660X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow d r|",             L"\\u8658X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow redo|",            L"\\u8631X",             NULL);
   gsFindAndReplaceU2(str, L"|*arrow undo|",            L"\\u8630X",             NULL);
   gsFindAndReplaceU2(str, L"|*card suit c|",           L"\\u9827X",             NULL);
   gsFindAndReplaceU2(str, L"|*card suit d|",           L"\\u9830X",             NULL);
   gsFindAndReplaceU2(str, L"|*card suit h|",           L"\\u9829X",             NULL);
   gsFindAndReplaceU2(str, L"|*card suit s|",           L"\\u9828X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess b k|",             L"\\u9818X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess b q|",             L"\\u9819X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess b r|",             L"\\u9820X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess b b|",             L"\\u9821X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess b h|",             L"\\u9822X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess b p|",             L"\\u9823X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess w k|",             L"\\u9812X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess w q|",             L"\\u9813X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess w r|",             L"\\u9814X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess w b|",             L"\\u9815X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess w h|",             L"\\u9816X",             NULL);
   gsFindAndReplaceU2(str, L"|*chess w p|",             L"\\u9817X",             NULL);
   gsFindAndReplaceU2(str, L"|*die 1|",                 L"\\u9856X",             NULL);
   gsFindAndReplaceU2(str, L"|*die 2|",                 L"\\u9857X",             NULL);
   gsFindAndReplaceU2(str, L"|*die 3|",                 L"\\u9858X",             NULL);
   gsFindAndReplaceU2(str, L"|*die 4|",                 L"\\u9859X",             NULL);
   gsFindAndReplaceU2(str, L"|*die 5|",                 L"\\u9860X",             NULL);
   gsFindAndReplaceU2(str, L"|*die 6|",                 L"\\u9861X",             NULL);
   gsFindAndReplaceU2(str, L"|*ball baseball|",         L"\\u9918X",             NULL);
   gsFindAndReplaceU2(str, L"|*ball soccer|",           L"\\u9917X",             NULL);

   // Symbols
   gsFindAndReplaceU2(str, L"|*checkbox off|",          L"\\u9744X",             NULL);
   gsFindAndReplaceU2(str, L"|*checkbox on|",           L"\\u9745X",             NULL);
   gsFindAndReplaceU2(str, L"|*circle 0%|",             L"\\u9675X",             NULL);
   gsFindAndReplaceU2(str, L"|*circle 25%|",            L"\\u9684X",             NULL);
   gsFindAndReplaceU2(str, L"|*circle 50%|",            L"\\u9681X",             NULL);
   gsFindAndReplaceU2(str, L"|*circle 75%|",            L"\\u9685X",             NULL);
   gsFindAndReplaceU2(str, L"|*circle 100%|",           L"\\u9679X",             NULL);
   gsFindAndReplaceU2(str, L"|*correct|",               L"\\u10003X",            NULL);
   gsFindAndReplaceU2(str, L"|*biohazard|",             L"\\u9763X",             NULL);
   gsFindAndReplaceU2(str, L"|*dot|",                   L"\\u9898X",             NULL);
   gsFindAndReplaceU2(str, L"|*dot filled|",            L"\\u9899X",             NULL);
   gsFindAndReplaceU2(str, L"|*envelope|",              L"\\u9993X",             NULL);
   gsFindAndReplaceU2(str, L"|*gender f|",              L"\\u9792X",             NULL);
   gsFindAndReplaceU2(str, L"|*gender m|",              L"\\u9794X",             NULL);
   gsFindAndReplaceU2(str, L"|*gender mf|",             L"\\u9893X",             NULL);
   gsFindAndReplaceU2(str, L"|*incorrect|",             L"\\u10007X",            NULL);
   gsFindAndReplaceU2(str, L"|*plane|",                 L"\\u9992X",             NULL);
   gsFindAndReplaceU2(str, L"|*poison|",                L"\\u9760X",             NULL);
   gsFindAndReplaceU2(str, L"|*radioactive|",           L"\\u9762X",             NULL);
   gsFindAndReplaceU2(str, L"|*Rx|",                    L"\\u8478X",             NULL);
   gsFindAndReplaceU2(str, L"|*recycle|",               L"\\u9851X",             NULL);
   gsFindAndReplaceU2(str, L"|*scissor|",               L"\\u9986X",             NULL);
   gsFindAndReplaceU2(str, L"|*snowflake|",             L"\\u10052X",            NULL);
   gsFindAndReplaceU2(str, L"|*star 5|",                L"\\u9734X",             NULL);
   gsFindAndReplaceU2(str, L"|*star 5 filled|",         L"\\u9733X",             NULL);
   gsFindAndReplaceU2(str, L"|*sun|",                   L"\\u9728X",             NULL);

   // Math
   gsFindAndReplaceU2(str, L"|*+/-|",                   L"\\u177X",              NULL);
   gsFindAndReplaceU2(str, L"|*angle|",                 L"\\u8736X",             NULL);
   gsFindAndReplaceU2(str, L"|*contains|",              L"\\u8715X",             NULL);
   gsFindAndReplaceU2(str, L"|*degree|",                L"\\u176X",              NULL);
   gsFindAndReplaceU2(str, L"|*dot|",                   L"\\u8901X",             NULL);
   gsFindAndReplaceU2(str, L"|*empty set|",             L"\\u8709X",             NULL);
   gsFindAndReplaceU2(str, L"|*equal almost|",          L"\\u8776X",             NULL);
   gsFindAndReplaceU2(str, L"|*equal approx |",         L"\\u8773X",             NULL);
   gsFindAndReplaceU2(str, L"|*equal greater than|",    L"\\u8805X",             NULL);
   gsFindAndReplaceU2(str, L"|*equal less than|",       L"\\u8804X",             NULL);
   gsFindAndReplaceU2(str, L"|*equal same|",            L"\\u8801X",             NULL);
   gsFindAndReplaceU2(str, L"|*equal not|",             L"\\u8800X",             NULL);
   gsFindAndReplaceU2(str, L"|*for all|",               L"\\u8704X",             NULL);
   gsFindAndReplaceU2(str, L"|*infinity|",              L"\\u8734X",             NULL);
   gsFindAndReplaceU2(str, L"|*integral|",              L"\\u8747X",             NULL);
   gsFindAndReplaceU2(str, L"|*intersection|",          L"\\u8745X",             NULL);
   gsFindAndReplaceU2(str, L"|*is in|",                 L"\\u8712X",             NULL);
   gsFindAndReplaceU2(str, L"|*is not in|",             L"\\u8713X",             NULL);
   gsFindAndReplaceU2(str, L"|*math phi|",              L"\\u981PX",             NULL);
   gsFindAndReplaceU2(str, L"|*math pi|",               L"\\u982PX",             NULL);
   gsFindAndReplaceU2(str, L"|*minus|",                 L"\\u8722X",             NULL);
   gsFindAndReplaceU2(str, L"|*nabla|",                 L"\\u8711X",             NULL);
   gsFindAndReplaceU2(str, L"|*partial diff|",          L"\\u8706X",             NULL);
   gsFindAndReplaceU2(str, L"|*product|",               L"\\u8719X",             NULL);
   gsFindAndReplaceU2(str, L"|*proportional to|",       L"\\u8733X",             NULL);
   gsFindAndReplaceU2(str, L"|*root|",                  L"\\u8730X",             NULL);
   gsFindAndReplaceU2(str, L"|*sum|",                   L"\\u8721X",             NULL);
   gsFindAndReplaceU2(str, L"|*there exists|",          L"\\u8707X",             NULL);
   gsFindAndReplaceU2(str, L"|*therefore|",             L"\\u8756X",             NULL);
   gsFindAndReplaceU2(str, L"|*union|",                 L"\\u8746X",             NULL);

   // Greek
   gsFindAndReplaceU2(str, L"|*alpha cap|",             L"\\u913X",              NULL);
   gsFindAndReplaceU2(str, L"|*beta cap|",              L"\\u914X",              NULL);
   gsFindAndReplaceU2(str, L"|*gamma cap|",             L"\\u915X",              NULL);
   gsFindAndReplaceU2(str, L"|*delta cap|",             L"\\u916X",              NULL);
   gsFindAndReplaceU2(str, L"|*epsilon cap|",           L"\\u917X",              NULL);
   gsFindAndReplaceU2(str, L"|*zeta cap|",              L"\\u918X",              NULL);
   gsFindAndReplaceU2(str, L"|*eta cap|",               L"\\u919X",              NULL);
   gsFindAndReplaceU2(str, L"|*theta cap|",             L"\\u920X",              NULL);
   gsFindAndReplaceU2(str, L"|*iota cap|",              L"\\u921X",              NULL);
   gsFindAndReplaceU2(str, L"|*kappa cap|",             L"\\u922X",              NULL);
   gsFindAndReplaceU2(str, L"|*lambda cap|",            L"\\u923X",              NULL);
   gsFindAndReplaceU2(str, L"|*mu cap|",                L"\\u924X",              NULL);
   gsFindAndReplaceU2(str, L"|*nu cap|",                L"\\u925X",              NULL);
   gsFindAndReplaceU2(str, L"|*xi cap|",                L"\\u926X",              NULL);
   gsFindAndReplaceU2(str, L"|*omicron cap|",           L"\\u927X",              NULL);
   gsFindAndReplaceU2(str, L"|*pi cap|",                L"\\u928X",              NULL);
   gsFindAndReplaceU2(str, L"|*rho cap|",               L"\\u929X",              NULL);
   gsFindAndReplaceU2(str, L"|*signma cap|",            L"\\u931X",              NULL);
   gsFindAndReplaceU2(str, L"|*tau cap|",               L"\\u932X",              NULL);
   gsFindAndReplaceU2(str, L"|*upsilon cap|",           L"\\u933X",              NULL);
   gsFindAndReplaceU2(str, L"|*phi cap|",               L"\\u934X",              NULL);
   gsFindAndReplaceU2(str, L"|*chi cap|",               L"\\u935X",              NULL);
   gsFindAndReplaceU2(str, L"|*psi cap|",               L"\\u936X",              NULL);
   gsFindAndReplaceU2(str, L"|*omega cap|",             L"\\u937X",              NULL);
   gsFindAndReplaceU2(str, L"|*alpha|",                 L"\\u945X",              NULL);
   gsFindAndReplaceU2(str, L"|*beta|",                  L"\\u946X",              NULL);
   gsFindAndReplaceU2(str, L"|*gamma|",                 L"\\u947X",              NULL);
   gsFindAndReplaceU2(str, L"|*delta|",                 L"\\u948X",              NULL);
   gsFindAndReplaceU2(str, L"|*epsilon|",               L"\\u949X",              NULL);
   gsFindAndReplaceU2(str, L"|*zeta|",                  L"\\u950X",              NULL);
   gsFindAndReplaceU2(str, L"|*eta|",                   L"\\u951X",              NULL);
   gsFindAndReplaceU2(str, L"|*theta|",                 L"\\u952X",              NULL);
   gsFindAndReplaceU2(str, L"|*iota|",                  L"\\u953X",              NULL);
   gsFindAndReplaceU2(str, L"|*kappa|",                 L"\\u954X",              NULL);
   gsFindAndReplaceU2(str, L"|*lambda|",                L"\\u955X",              NULL);
   gsFindAndReplaceU2(str, L"|*mu|",                    L"\\u956X",              NULL);
   gsFindAndReplaceU2(str, L"|*nu|",                    L"\\u957X",              NULL);
   gsFindAndReplaceU2(str, L"|*xi|",                    L"\\u958X",              NULL);
   gsFindAndReplaceU2(str, L"|*omicron|",               L"\\u959X",              NULL);
   gsFindAndReplaceU2(str, L"|*pi|",                    L"\\u960X",              NULL);
   gsFindAndReplaceU2(str, L"|*rho|",                   L"\\u961X",              NULL);
   gsFindAndReplaceU2(str, L"|*signma|",                L"\\u963X",              NULL);
   gsFindAndReplaceU2(str, L"|*tau|",                   L"\\u964X",              NULL);
   gsFindAndReplaceU2(str, L"|*upsilon|",               L"\\u965X",              NULL);
   gsFindAndReplaceU2(str, L"|*phi|",                   L"\\u966X",              NULL);
   gsFindAndReplaceU2(str, L"|*chi|",                   L"\\u967X",              NULL);
   gsFindAndReplaceU2(str, L"|*psi|",                   L"\\u968X",              NULL);
   gsFindAndReplaceU2(str, L"|*omega|",                 L"\\u969X",              NULL);

   gsFindAndReplaceU2(str, L"|' ",                      L"\\super ",             NULL);
   gsFindAndReplaceU2(str, L" '|",                      L"\\nosupersub ",        NULL);
   gsFindAndReplaceU2(str, L"|, ",                      L"\\sub ",               NULL);
   gsFindAndReplaceU2(str, L" ,|",                      L"\\nosupersub ",        NULL);
   gsFindAndReplaceU2(str, L"|. ",                      L"\\f2 ",                NULL);
   gsFindAndReplaceU2(str, L" .|",                      L"\\f0 ",                NULL);
   gsFindAndReplaceU2(str, L"|b ",                      L"\\b ",                 NULL);
   gsFindAndReplaceU2(str, L" b|",                      L"\\b0 ",                NULL);
   gsFindAndReplaceU2(str, L"|i ",                      L"\\i ",                 NULL);
   gsFindAndReplaceU2(str, L" i|",                      L"\\i0 ",                NULL);
   gsFindAndReplaceU2(str, L"|u ",                      L"\\ul1 ",               NULL);
   gsFindAndReplaceU2(str, L" u|",                      L"\\ul0 ",               NULL);
   gsFindAndReplaceU2(str, L"|s ",                      L"\\strike ",            NULL);
   gsFindAndReplaceU2(str, L" s|",                      L"\\strike0 ",           NULL);
   gsFindAndReplaceU2(str, L"|\"",                      L"\\ldblquote ",         NULL);
   gsFindAndReplaceU2(str, L"\"|",                      L"\\rdblquote ",         NULL);

   gsFindAndReplaceU2(str, L"\\|",                      L"|",                    NULL);
   gsFindAndReplaceU2(str, L"|\\",                      L"|",                    NULL);

   gsFindAndReplaceU2(str, L"'",                        L"\\rquote ",            NULL);
   gsFindAndReplaceU2(str, L"`",                        L"\\lquote ",            NULL);
   gsFindAndReplaceU2(str, L"*",                        L"\\u42s",               NULL);

   greturn str;
}

/******************************************************************************
func: _FileOpen
******************************************************************************/
static Gfile *_FileOpen(Gpath const * const path)
{
   Gfile *file;
   Gpath *pathFile;

   genter;

   // Make the destination file path.
   pathFile = gpathCreateFrom(path);
   gpathPopExtension(pathFile);
   gpathPushExtensionA(pathFile, "rtf");

   // Create the file.
   file = gfileOpen(pathFile, gfileOpenModeREAD_WRITE_NEW);

   gpathDestroy(pathFile);

   greturn file;
}

/******************************************************************************
func: _FileWriteComment
******************************************************************************/
static Gb _FileWriteComment(Gfile * const file, Para const * const para)
{
   genter;

   file; 
   para;

   greturn gbTRUE;
}

/******************************************************************************
func: _FileWriteFormatFooter
******************************************************************************/
static Gb _FileWriteFormatFooter(Gfile * const file)
{
   Gb result;

   genter;

   // Write the terminators.
   result = gfileSetA(file, gcTypeU1, "}", NULL);

   greturn result;
}

/******************************************************************************
func: _FileWriteFormatHeader
******************************************************************************/
static Gb _FileWriteFormatHeader(Gfile * const file, Gs const * const title)
{
   Gb result;

   genter;

   title;

   // Write the header
   result = gfileSetA(
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

   greturn result;
}

/******************************************************************************
func: _FileWritePageBreak
******************************************************************************/
static Gb _FileWritePageBreak(Gfile * const file)
{
   Gb result;

   genter;

   result = gfileSetA(file, gcTypeU1, "\\page\n", NULL);

   greturn result;
}

/******************************************************************************
func: _FileWriteParagraph
******************************************************************************/
static Gb _FileWriteParagraph(Gfile * const file, Para const * const para)
{
   Gb  result;
   Gs *stemp;

   genter;

   result = gbTRUE;

   result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_00 "\\ilvl0\n", NULL);

   stemp   = _ProcessInline(para->str, para);
   result &= gfileSetS(file, gcTypeU1, stemp, NULL);
   gsDestroy(stemp);

   result &= gfileSetA(file, gcTypeU1, "\\par\n", NULL);

   greturn result;
}

/******************************************************************************
func: _FileWriteKeyValue 
******************************************************************************/
static Gb _FileWriteKeyValue(Gfile * const file, Para const * const para)
{
   Gb  result;
   Gs *stemp;

   genter;

   result = gbTRUE;

   result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_20 "\\ilvl0\n", NULL);

   stemp   = _ProcessInline(para->str, para);
   result &= gfileSetS(file, gcTypeU1, stemp, NULL);
   gsDestroy(stemp);

   result &= gfileSetA(file, gcTypeU1, "\\par\n", NULL);

   greturn result;
}

/******************************************************************************
func: _FileWriteScopeStart
******************************************************************************/
static Gb _FileWriteScopeStart(Gfile * const file, ParaType const type,
   Gindex const indentLevel, ParaType const parentType)
{
   Gb result;

   genter;

   result = gbTRUE;

   switch (type)
   {
   case paraTypeFORMATED:
   case paraTypeSCOPE_FORMATED:
      result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_30 "\\ilvl0\n ", NULL);
      break;

   case paraTypeITEM:
   case paraTypeSCOPE_ITEM:
      if (parentType == paraTypeSCOPE_LIST_BULLET)
      {
         switch (indentLevel)
         {
         case 0: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_21 "\\ilvl0\n\\bullet  ", NULL); break;
         case 1: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_22 "\\ilvl0\n\\bullet  ", NULL); break;
         case 2: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_23 "\\ilvl0\n\\bullet  ", NULL); break;
         case 3: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_24 "\\ilvl0\n\\bullet  ", NULL); break;
         case 4: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_25 "\\ilvl0\n\\bullet  ", NULL); break;
         case 5: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_26 "\\ilvl0\n\\bullet  ", NULL); break;
         case 6: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_27 "\\ilvl0\n\\bullet  ", NULL); break;
         case 7: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_28 "\\ilvl0\n\\bullet  ", NULL); break;
         case 8: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_29 "\\ilvl0\n\\bullet  ", NULL); break;
         }
      }
      else if (parentType == paraTypeSCOPE_LIST_NUMBER)
      {
         switch (indentLevel)
         {
         case 0: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_21 "\\ilvl0\n# ", NULL); break;
         case 1: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_22 "\\ilvl0\n# ", NULL); break;
         case 2: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_23 "\\ilvl0\n# ", NULL); break;
         case 3: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_24 "\\ilvl0\n# ", NULL); break;
         case 4: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_25 "\\ilvl0\n# ", NULL); break;
         case 5: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_26 "\\ilvl0\n# ", NULL); break;
         case 6: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_27 "\\ilvl0\n# ", NULL); break;
         case 7: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_28 "\\ilvl0\n# ", NULL); break;
         case 8: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_29 "\\ilvl0\n# ", NULL); break;
         }
      }
      break;

   case paraTypeSCOPE_LIST_BULLET:
   case paraTypeSCOPE_LIST_KEY_VALUE:
   case paraTypeSCOPE_LIST_NUMBER:
      break;

   case paraTypeSCOPE_TABLE:
      break;

   case paraTypeTABLE_COLUMN_HEADER:
   case paraTypeSCOPE_TABLE_COLUMN_HEADER:
   case paraTypeTABLE_COLUMN_HEADER_NO_BREAK:
   case paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK:
   case paraTypeTABLE_COLUMN_HEADER_FILL:
   case paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL:
   case paraTypeTABLE_COLUMN:
   case paraTypeSCOPE_TABLE_COLUMN:
   case paraTypeTABLE_COLUMN_NO_BREAK:
   case paraTypeSCOPE_TABLE_COLUMN_NO_BREAK:
   case paraTypeTABLE_COLUMN_FILL:
   case paraTypeSCOPE_TABLE_COLUMN_FILL:
   case paraTypeTABLE_COLUMN_NUMBER:
   case paraTypeSCOPE_TABLE_COLUMN_NUMBER:
   case paraTypeTABLE_COLUMN_NUMBER_FILL:
   case paraTypeSCOPE_TABLE_COLUMN_NUMBER_FILL:
      break;

   case paraTypeTABLE_ROW:
      break;
   }

   greturn result;
}

/******************************************************************************
func: _FileWriteScopeStop
******************************************************************************/
static Gb _FileWriteScopeStop(Gfile * const file, ParaType const type, ParaType const parentType)
{
   Gb result;

   genter;

   parentType;

   result = gbTRUE;

   switch (type)
   {
   case paraTypeFORMATED:
   case paraTypeSCOPE_FORMATED:
   case paraTypeITEM:
   case paraTypeSCOPE_ITEM:
      result &= gfileSetA(file, gcTypeU1, "\\par\n", NULL);
      break;

   case paraTypeSCOPE_LIST_BULLET:
   case paraTypeSCOPE_LIST_KEY_VALUE:
   case paraTypeSCOPE_LIST_NUMBER:
      break;

   case paraTypeSCOPE_TABLE:
      break;

   case paraTypeTABLE_COLUMN_HEADER:
   case paraTypeSCOPE_TABLE_COLUMN_HEADER:
   case paraTypeTABLE_COLUMN_HEADER_NO_BREAK:
   case paraTypeSCOPE_TABLE_COLUMN_HEADER_NO_BREAK:
   case paraTypeTABLE_COLUMN_HEADER_FILL:
   case paraTypeSCOPE_TABLE_COLUMN_HEADER_FILL:
      break;

   case paraTypeTABLE_COLUMN:
   case paraTypeSCOPE_TABLE_COLUMN:
   case paraTypeTABLE_COLUMN_NO_BREAK:
   case paraTypeSCOPE_TABLE_COLUMN_NO_BREAK:
   case paraTypeTABLE_COLUMN_FILL:
   case paraTypeSCOPE_TABLE_COLUMN_FILL:
   case paraTypeTABLE_COLUMN_NUMBER:
   case paraTypeSCOPE_TABLE_COLUMN_NUMBER:
   case paraTypeTABLE_COLUMN_NUMBER_FILL:
   case paraTypeSCOPE_TABLE_COLUMN_NUMBER_FILL:
      break;

   case paraTypeTABLE_ROW:
      break;
   }

   greturn result;
}

/******************************************************************************
func: _FileWriteString
******************************************************************************/
static Gb _FileWriteString(Gfile * const file, Para const * const para)
{
   Gb  result;
   Gs *stemp;

   genter;

   stemp = _ProcessInline(para->str, para);
   
   result = gfileSetS(file, gcTypeU1, stemp, NULL);

   gsDestroy(stemp);

   greturn result;
}

/******************************************************************************
func: _FileWriteStringUnaltered
******************************************************************************/
static Gb _FileWriteStringUnaltered(Gfile * const file, Para const * const para)
{
   Gb result;

   genter;

   result = gfileSetS(file, gcTypeU1, para->str, NULL);

   greturn result;
}

/******************************************************************************
func: _FileWriteTitle
******************************************************************************/
static Gb _FileWriteTitle(Gfile * const file, Para const * const para)
{
   Gb  result;
   Gs *stemp;

   genter;

   result = gbTRUE;

   switch (para->type)
   {
   case paraTypeTITLE_1:     result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_01 "\\ilvl0\n", NULL); break;
   case paraTypeTITLE_TOC_1: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_11 "\\ilvl0\n", NULL); break;
   case paraTypeTITLE_2:     result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_02 "\\ilvl0\n", NULL); break;
   case paraTypeTITLE_TOC_2: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_12 "\\ilvl0\n", NULL); break;
   case paraTypeTITLE_3:     result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_03 "\\ilvl0\n", NULL); break; 
   case paraTypeTITLE_TOC_3: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_13 "\\ilvl0\n", NULL); break;
   case paraTypeTITLE_4:     result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_04 "\\ilvl0\n", NULL); break;                                                        
   case paraTypeTITLE_TOC_4: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_14 "\\ilvl0\n", NULL); break;
   case paraTypeTITLE_5:     result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_05 "\\ilvl0\n", NULL); break;
   case paraTypeTITLE_TOC_5: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_15 "\\ilvl0\n", NULL); break;
   }

   stemp   = _ProcessInline(para->str, para);
   result &= gfileSetS(file, gcTypeU1, stemp, NULL);
   gsDestroy(stemp);

   result &= gfileSetA(file, gcTypeU1, "\\par\n", NULL);

   greturn result;
}

/******************************************************************************
func: _FileWriteTOC
******************************************************************************/
static Gb _FileWriteTOC(Gfile * const file, ParaArray const * const tocList)
{
   Gb         result;
   Gindex     index;
   Para      *para;
   Gs        *stemp;

   genter;

   result = gbTRUE;

   // For all titles...
   forCount (index, paraArrayGetCount(tocList))
   {
      // Get the title.
      para = paraArrayGetAt(tocList, index);

      switch (para->type)
      {
      case paraTypeTITLE_TOC_1: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_41 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_2: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_42 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_3: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_43 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_4: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_44 "\\ilvl0\n",  NULL); break;
      case paraTypeTITLE_TOC_5: result &= gfileSetA(file, gcTypeU1, "\\pard\\plain " RTF_STYLE_45 "\\ilvl0\n",  NULL); break;
      }

      stemp = _ProcessInline(para->str, para);
      gfileSetS(file, gcTypeU1, stemp, NULL);
      gsDestroy(stemp);

      gfileSetA(file, gcTypeU1, "\\par\n", NULL);
   }

   greturn result;
}
