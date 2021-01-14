/******************************************************************************
file:       ZWORD_MD
author:     Robbert de Groot
copyright:  2020, Robbert de Groot
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "ZWORD.h"

/******************************************************************************
local:
prototype:
******************************************************************************/
static Gs      *_ProcessInline(                 Gs          * const inStr, Para          const * const para);

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
static Gb       _FileWriteTOC(                  Gfile       * const file,  ParaArray     const * const paraList);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: GetFunctionsMD
******************************************************************************/
WriteFunctions GetFunctionsMD(void)
{
   WriteFunctions func;

   func.FileOpen                      = _FileOpen;
   func.FileWriteComment              = _FileWriteComment;
   func.FileWriteFormatFooter         = _FileWriteFormatFooter;
   func.FileWriteFormatHeader         = _FileWriteFormatHeader;
   func.FileWriteKeyValue             = _FileWriteKeyValue;
   func.FileWritePageBreak            = _FileWritePageBreak;
   func.FileWriteParagraph            = _FileWriteParagraph;
   func.FileWriteScopeStart           = _FileWriteScopeStart;
   func.FileWriteScopeStop            = _FileWriteScopeStop;
   func.FileWriteString               = _FileWriteString;
   func.FileWriteStringUnaltered      = _FileWriteStringUnaltered;
   func.FileWriteTableHeaderSeparator = _FileWriteTableHeaderSeparator;
   func.FileWriteTitle                = _FileWriteTitle;
   func.FileWriteTOC                  = _FileWriteTOC;

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

   gsFindAndReplaceU2(str, L"<",                        L"&#60;",               NULL);
   gsFindAndReplaceU2(str, L">",                        L"&#62;",               NULL);

   gsFindAndReplaceU2(str, L"|chapter|",                gsGet(para->chapterStr),NULL);

   // Styling
   gsFindAndReplaceU2(str, L"|img |",                   L"![image](",           NULL);
   gsFindAndReplaceU2(str, L"| img|",                   L")",                   NULL);
   gsFindAndReplaceU2(str, L"|link |",                  L"[LINK](",             NULL);
   gsFindAndReplaceU2(str, L"| link |",                 L") ",                  NULL);
   gsFindAndReplaceU2(str, L"| link|",                  L"",                    NULL);
   gsFindAndReplaceU2(str, L"|line|",                   L"<br />",              NULL);
   gsFindAndReplaceU2(str, L"|line |",                  L"<br /><br />",        NULL);
   gsFindAndReplaceU2(str, L"|line  |",                 L"<br /><br /><br />",  NULL);

   // Legal
   gsFindAndReplaceU2(str, L"|*copyright|",             L"&copy;",              NULL);
   gsFindAndReplaceU2(str, L"|*registered tm|",         L"&reg;",               NULL);
   gsFindAndReplaceU2(str, L"|*tm|",                    L"&trade;",             NULL);

   // Currency
   gsFindAndReplaceU2(str, L"|*bitcoin|",               L"&#x20BF;",            NULL);
   gsFindAndReplaceU2(str, L"|*cent|",                  L"&cent;",              NULL);
   gsFindAndReplaceU2(str, L"|*dollar|",                L"$",                   NULL);
   gsFindAndReplaceU2(str, L"|*euro|",                  L"&euro;",              NULL);
   gsFindAndReplaceU2(str, L"|*franc|",                 L"&#x20A3;",            NULL);
   gsFindAndReplaceU2(str, L"|*lira|",                  L"&#x20A4;",            NULL);
   gsFindAndReplaceU2(str, L"|*lira turkey|",           L"&#x20BA;",            NULL);
   gsFindAndReplaceU2(str, L"|*peso|",                  L"&#x20B1;",            NULL);
   gsFindAndReplaceU2(str, L"|*pound|",                 L"&pound;",             NULL);
   gsFindAndReplaceU2(str, L"|*ruble|",                 L"&#x20BD;",            NULL);
   gsFindAndReplaceU2(str, L"|*rupee|",                 L"&#x20A8;",            NULL);
   gsFindAndReplaceU2(str, L"|*rupee india|",           L"&#x20B9;",            NULL);
   gsFindAndReplaceU2(str, L"|*won|",                   L"&#x20A9;",            NULL);
   gsFindAndReplaceU2(str, L"|*yen|",                   L"&yen;",               NULL);

   // Punctuation
   gsFindAndReplaceU2(str, L"|*...|",                   L"&hellip;",            NULL);
   gsFindAndReplaceU2(str, L"|*1/4|",                   L"&frac14;",            NULL);
   gsFindAndReplaceU2(str, L"|*1/2|",                   L"&frac12;",            NULL);
   gsFindAndReplaceU2(str, L"|*3/4|",                   L"&frac34;",            NULL);
   gsFindAndReplaceU2(str, L"|*ampersand|",             L"&amp;",               NULL);
   gsFindAndReplaceU2(str, L"|*at|",                    L"@",                   NULL);
   gsFindAndReplaceU2(str, L"|*bullet|",                L"&bull;",              NULL);
   gsFindAndReplaceU2(str, L"|*dagger s|",              L"&dagger;",            NULL);
   gsFindAndReplaceU2(str, L"|*dagger d|",              L"&Dagger;",            NULL);
   gsFindAndReplaceU2(str, L"|*hash|",                  L"#",                   NULL);
   gsFindAndReplaceU2(str, L"|*inv!|",                  L"&iexcl;",             NULL);
   gsFindAndReplaceU2(str, L"|*inv?|",                  L"&iquest;",            NULL);
   gsFindAndReplaceU2(str, L"|*quote angle d l|",       L"&laquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*quote angle d r|",       L"&raquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*quote angle s l|",       L"&lsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"|*quote angle s r|",       L"&rsaquo;",            NULL);
   gsFindAndReplaceU2(str, L"|*quote d|",               L"\"",                  NULL);
   gsFindAndReplaceU2(str, L"|*quote d l|",             L"&ldquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*quote d r|",             L"&rdquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*quote d low|",           L"&bdquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*quote s|",               L"'",                   NULL);
   gsFindAndReplaceU2(str, L"|*quote s l|",             L"&lsquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*quote s r|",             L"&rsquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*quote s low|",           L"&sbquo;",             NULL);
   gsFindAndReplaceU2(str, L"|*para|",                  L"&para;",              NULL);
   gsFindAndReplaceU2(str, L"|*prime d|",               L"&Prime;",             NULL);
   gsFindAndReplaceU2(str, L"|*prime s|",               L"&prime;",             NULL);

   // Game
   gsFindAndReplaceU2(str, L"|*arrow d|",               L"&darr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow l|",               L"&larr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow r|",               L"&rarr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow u|",               L"&uarr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow lr|",              L"&harr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow ud|",              L"&#x2195;",            NULL);
   gsFindAndReplaceU2(str, L"|*arrow \\d|",             L"&#x2198;",            NULL);
   gsFindAndReplaceU2(str, L"|*arrow /d|",              L"&#x2199;",            NULL);
   gsFindAndReplaceU2(str, L"|*arrow \\u|",             L"&#x2196;",            NULL);
   gsFindAndReplaceU2(str, L"|*arrow /u|",              L"&#x2197;",            NULL);
   gsFindAndReplaceU2(str, L"|*arrow d l|",             L"&lArr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow d lr|",            L"&hArr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow d r|",             L"&rArr;",              NULL);
   gsFindAndReplaceU2(str, L"|*arrow redo|",            L"&#x21B7;",            NULL);
   gsFindAndReplaceU2(str, L"|*arrow undo|",            L"&#x21B6;",            NULL);
   gsFindAndReplaceU2(str, L"|*card suit c|",           L"&clubs;",             NULL);
   gsFindAndReplaceU2(str, L"|*card suit d|",           L"&diams;",             NULL);
   gsFindAndReplaceU2(str, L"|*card suit h|",           L"&hearts;",            NULL);
   gsFindAndReplaceU2(str, L"|*card suit s|",           L"&spades;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess b k|",             L"&#x265A;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess b q|",             L"&#x265B;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess b r|",             L"&#x265C;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess b b|",             L"&#x265D;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess b h|",             L"&#x265E;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess b p|",             L"&#x265F;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess w k|",             L"&#x2654;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess w q|",             L"&#x2655;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess w r|",             L"&#x2656;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess w b|",             L"&#x2657;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess w h|",             L"&#x2658;",            NULL);
   gsFindAndReplaceU2(str, L"|*chess w p|",             L"&#x2659;",            NULL);
   gsFindAndReplaceU2(str, L"|*die 1|",                 L"&#x2680;",            NULL);
   gsFindAndReplaceU2(str, L"|*die 2|",                 L"&#x2681;",            NULL);
   gsFindAndReplaceU2(str, L"|*die 3|",                 L"&#x2682;",            NULL);
   gsFindAndReplaceU2(str, L"|*die 4|",                 L"&#x2683;",            NULL);
   gsFindAndReplaceU2(str, L"|*die 5|",                 L"&#x2684;",            NULL);
   gsFindAndReplaceU2(str, L"|*die 6|",                 L"&#x2685;",            NULL);
   gsFindAndReplaceU2(str, L"|*ball baseball|",         L"&#x26BE;",            NULL);
   gsFindAndReplaceU2(str, L"|*ball soccer|",           L"&#x26BD;",            NULL);

   // Symbols
   gsFindAndReplaceU2(str, L"|*checkbox off|",          L"&#x2610;",            NULL);
   gsFindAndReplaceU2(str, L"|*checkbox on|",           L"&#x2611;",            NULL);
   gsFindAndReplaceU2(str, L"|*circle 0%|",             L"&#x25CB;",            NULL);
   gsFindAndReplaceU2(str, L"|*circle 25%|",            L"&#x25D4;",            NULL);
   gsFindAndReplaceU2(str, L"|*circle 50%|",            L"&#x25D1;",            NULL);
   gsFindAndReplaceU2(str, L"|*circle 75%|",            L"&#x25D5;",            NULL);
   gsFindAndReplaceU2(str, L"|*circle 100%|",           L"&#x25CF;",            NULL);
   gsFindAndReplaceU2(str, L"|*correct|",               L"&#x2713;",            NULL);
   gsFindAndReplaceU2(str, L"|*biohazard|",             L"&#x2623;",            NULL);
   gsFindAndReplaceU2(str, L"|*dot|",                   L"&#x26AA;",            NULL);
   gsFindAndReplaceU2(str, L"|*dot filled|",            L"&#x26AB;",            NULL);
   gsFindAndReplaceU2(str, L"|*envelope|",              L"&#x2709;",            NULL);
   gsFindAndReplaceU2(str, L"|*gender f|",              L"&#x2640;",            NULL);
   gsFindAndReplaceU2(str, L"|*gender m|",              L"&#x2642;",            NULL);
   gsFindAndReplaceU2(str, L"|*gender mf|",             L"&#x26A5;",            NULL);
   gsFindAndReplaceU2(str, L"|*incorrect|",             L"&#x2717;",            NULL);
   gsFindAndReplaceU2(str, L"|*plane|",                 L"&#x2708;",            NULL);
   gsFindAndReplaceU2(str, L"|*poison|",                L"&#x2620;",            NULL);
   gsFindAndReplaceU2(str, L"|*radioactive|",           L"&#x2622;",            NULL);
   gsFindAndReplaceU2(str, L"|*Rx|",                    L"&#x211E;",            NULL);
   gsFindAndReplaceU2(str, L"|*recycle|",               L"&#x267B;",            NULL);
   gsFindAndReplaceU2(str, L"|*scissor|",               L"&#x2702;",            NULL);
   gsFindAndReplaceU2(str, L"|*snowflake|",             L"&#x2744;",            NULL);
   gsFindAndReplaceU2(str, L"|*star 5|",                L"&#x2606;",            NULL);
   gsFindAndReplaceU2(str, L"|*star 5 filled|",         L"&#x2605;",            NULL);
   gsFindAndReplaceU2(str, L"|*sun|",                   L"&#x2600;",            NULL);

   // Math
   gsFindAndReplaceU2(str, L"|*+/-|",                   L"&plusmn;",            NULL);
   gsFindAndReplaceU2(str, L"|*angle|",                 L"&ang;",               NULL);
   gsFindAndReplaceU2(str, L"|*contains|",              L"&ni;",                NULL);
   gsFindAndReplaceU2(str, L"|*degree|",                L"&deg;",               NULL);
   gsFindAndReplaceU2(str, L"|*dot|",                   L"&sdot;",              NULL);
   gsFindAndReplaceU2(str, L"|*empty set|",             L"&empty;",             NULL);
   gsFindAndReplaceU2(str, L"|*equal almost|",          L"&asymp;",             NULL);
   gsFindAndReplaceU2(str, L"|*equal approx |",         L"&cong;",              NULL);
   gsFindAndReplaceU2(str, L"|*equal greater than|",    L"&ge;",                NULL);
   gsFindAndReplaceU2(str, L"|*equal less than|",       L"&le;",                NULL);
   gsFindAndReplaceU2(str, L"|*equal same|",            L"&equiv;",             NULL);
   gsFindAndReplaceU2(str, L"|*equal not|",             L"&ne;",                NULL);
   gsFindAndReplaceU2(str, L"|*for all|",               L"&forall;",            NULL);
   gsFindAndReplaceU2(str, L"|*infinity|",              L"&infin;",             NULL);
   gsFindAndReplaceU2(str, L"|*integral|",              L"&int;",               NULL);
   gsFindAndReplaceU2(str, L"|*intersection|",          L"&cap;",               NULL);
   gsFindAndReplaceU2(str, L"|*is in|",                 L"&isin;",              NULL);
   gsFindAndReplaceU2(str, L"|*is not in|",             L"&notin;",             NULL);
   gsFindAndReplaceU2(str, L"|*math phi|",              L"&straightphi;",       NULL);
   gsFindAndReplaceU2(str, L"|*math pi|",               L"&piv;",               NULL);
   gsFindAndReplaceU2(str, L"|*minus|",                 L"&minus;",             NULL);
   gsFindAndReplaceU2(str, L"|*nabla|",                 L"&nabla;",             NULL);
   gsFindAndReplaceU2(str, L"|*partial diff|",          L"&part;",              NULL);
   gsFindAndReplaceU2(str, L"|*product|",               L"&prod;",              NULL);
   gsFindAndReplaceU2(str, L"|*proportional to|",       L"&prop;",              NULL);
   gsFindAndReplaceU2(str, L"|*root|",                  L"&radic;",             NULL);
   gsFindAndReplaceU2(str, L"|*sum|",                   L"&sum;",               NULL);
   gsFindAndReplaceU2(str, L"|*there exists|",          L"&exist;",             NULL);
   gsFindAndReplaceU2(str, L"|*therefore|",             L"&there4;",            NULL);
   gsFindAndReplaceU2(str, L"|*union|",                 L"&cup;",               NULL);

   // Greek
   gsFindAndReplaceU2(str, L"|*alpha cap|",             L"&Alpha;",             NULL);
   gsFindAndReplaceU2(str, L"|*beta cap|",              L"&Beta;",              NULL);
   gsFindAndReplaceU2(str, L"|*gamma cap|",             L"&Gamma;",             NULL);
   gsFindAndReplaceU2(str, L"|*delta cap|",             L"&Delta;",             NULL);
   gsFindAndReplaceU2(str, L"|*epsilon cap|",           L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|*zeta cap|",              L"&Zeta;",              NULL);
   gsFindAndReplaceU2(str, L"|*eta cap|",               L"&Eta;",               NULL);
   gsFindAndReplaceU2(str, L"|*theta cap|",             L"&Theta;",             NULL);
   gsFindAndReplaceU2(str, L"|*iota cap|",              L"&Iota;",              NULL);
   gsFindAndReplaceU2(str, L"|*kappa cap|",             L"&Kappa;",             NULL);
   gsFindAndReplaceU2(str, L"|*lambda cap|",            L"&Lambda;",            NULL);
   gsFindAndReplaceU2(str, L"|*mu cap|",                L"&Mu;",                NULL);
   gsFindAndReplaceU2(str, L"|*nu cap|",                L"&Nu;",                NULL);
   gsFindAndReplaceU2(str, L"|*xi cap|",                L"&Xi;",                NULL);
   gsFindAndReplaceU2(str, L"|*omicron cap|",           L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"|*pi cap|",                L"&Pi;",                NULL);
   gsFindAndReplaceU2(str, L"|*rho cap|",               L"&Rho;",               NULL);
   gsFindAndReplaceU2(str, L"|*signma cap|",            L"&Sigma;",             NULL);
   gsFindAndReplaceU2(str, L"|*tau cap|",               L"&Tau;",               NULL);
   gsFindAndReplaceU2(str, L"|*upsilon cap|",           L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|*phi cap|",               L"&Phi;",               NULL);
   gsFindAndReplaceU2(str, L"|*chi cap|",               L"&Chi;",               NULL);
   gsFindAndReplaceU2(str, L"|*psi cap|",               L"&Psi;",               NULL);
   gsFindAndReplaceU2(str, L"|*omega cap|",             L"&Omega;",             NULL);
   gsFindAndReplaceU2(str, L"|*alpha|",                 L"&alpha;",             NULL);
   gsFindAndReplaceU2(str, L"|*beta|",                  L"&beta;",              NULL);
   gsFindAndReplaceU2(str, L"|*gamma|",                 L"&gamma;",             NULL);
   gsFindAndReplaceU2(str, L"|*delta|",                 L"&delta;",             NULL);
   gsFindAndReplaceU2(str, L"|*epsilon|",               L"&Epsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|*zeta|",                  L"&zeta;",              NULL);
   gsFindAndReplaceU2(str, L"|*eta|",                   L"&eta;",               NULL);
   gsFindAndReplaceU2(str, L"|*theta|",                 L"&theta;",             NULL);
   gsFindAndReplaceU2(str, L"|*iota|",                  L"&iota;",              NULL);
   gsFindAndReplaceU2(str, L"|*kappa|",                 L"&kappa;",             NULL);
   gsFindAndReplaceU2(str, L"|*lambda|",                L"&lambda;",            NULL);
   gsFindAndReplaceU2(str, L"|*mu|",                    L"&mu;",                NULL);
   gsFindAndReplaceU2(str, L"|*nu|",                    L"&nu;",                NULL);
   gsFindAndReplaceU2(str, L"|*xi|",                    L"&xi;",                NULL);
   gsFindAndReplaceU2(str, L"|*omicron|",               L"&Omicron;",           NULL);
   gsFindAndReplaceU2(str, L"|*pi|",                    L"&pi;",                NULL);
   gsFindAndReplaceU2(str, L"|*rho|",                   L"&rho;",               NULL);
   gsFindAndReplaceU2(str, L"|*signma|",                L"&sigma;",             NULL);
   gsFindAndReplaceU2(str, L"|*tau|",                   L"&tau;",               NULL);
   gsFindAndReplaceU2(str, L"|*upsilon|",               L"&Upsilon;",           NULL);
   gsFindAndReplaceU2(str, L"|*phi|",                   L"&phi;",               NULL);
   gsFindAndReplaceU2(str, L"|*chi|",                   L"&chi;",               NULL);
   gsFindAndReplaceU2(str, L"|*psi|",                   L"&psi;",               NULL);
   gsFindAndReplaceU2(str, L"|*omega|",                 L"&omega;",             NULL);

   gsFindAndReplaceU2(str, L"*",                        L"&#42;",               NULL);
   gsFindAndReplaceU2(str, L"`",                        L"&#96;",               NULL);
   gsFindAndReplaceU2(str, L"~",                        L"&#126;",              NULL);
   

   gsFindAndReplaceU2(str, L"|' |",                     L"<sup>",               NULL);
   gsFindAndReplaceU2(str, L"| '|",                     L"</sup>",              NULL);
   gsFindAndReplaceU2(str, L"|, |",                     L"<sub>",               NULL);
   gsFindAndReplaceU2(str, L"| ,|",                     L"</sub>",              NULL);
   gsFindAndReplaceU2(str, L"|. |",                     L"`",                   NULL);
   gsFindAndReplaceU2(str, L"| .|",                     L"`",                   NULL);
   gsFindAndReplaceU2(str, L"|b |",                     L"**",                  NULL);
   gsFindAndReplaceU2(str, L"| b|",                     L"**",                  NULL);
   gsFindAndReplaceU2(str, L"|i |",                     L"*",                   NULL);
   gsFindAndReplaceU2(str, L"| i|",                     L"*",                   NULL);
   gsFindAndReplaceU2(str, L"|u |",                     L"<u>",                 NULL);
   gsFindAndReplaceU2(str, L"| u|",                     L"</u>",                NULL);
   gsFindAndReplaceU2(str, L"|s |",                     L"~~",                  NULL);
   gsFindAndReplaceU2(str, L"| s|",                     L"~~",                  NULL);
   gsFindAndReplaceU2(str, L"|_|",                      L" ",                   NULL);
   gsFindAndReplaceU2(str, L"|\" |",                    L"&ldquo;",             NULL);
   gsFindAndReplaceU2(str, L"| \"|",                    L"&rdquo;",             NULL);
   
   gsFindAndReplaceU2(str, L"\"|\"",                    L"&#124;",              NULL);
   gsFindAndReplaceU2(str, L"\"_\"",                    L"",                    NULL);

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
   gpathPushExtensionA(pathFile, "md");

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

   // No comment strings.
   file; 
   para;

   greturn gbTRUE;
}

/******************************************************************************
func: _FileWriteFormatFooter
******************************************************************************/
static Gb _FileWriteFormatFooter(Gfile * const file)
{
   genter;

   // No format footer.
   file; 

   greturn gbTRUE;
}

/******************************************************************************
func: _FileWriteFormatHeader
******************************************************************************/
static Gb _FileWriteFormatHeader(Gfile * const file, Gs const * const title)
{
   genter;

   // No format header
   file; 
   title;

   greturn gbTRUE;
}

/******************************************************************************
func: _FileWriteKeyValue 
******************************************************************************/
static Gb _FileWriteKeyValue(Gfile * const file, Para const * const para)
{
   Gb        result;
   Gs       *stemp;
   GsArray  *stempArray;

   genter;

   result = gbTRUE;

   stempArray = gsCreateSplit(para->str, L'\t');

   result &= gfileSetA(file, gcTypeU1, "**", NULL);
   
   stemp   = _ProcessInline(gsArrayGetAt(stempArray, 0), para);
   result &= gfileSetS(file, gcTypeU1, stemp, NULL);
   gsDestroy(stemp);

   result &= gfileSetA(file, gcTypeU1, "**<br>\n    ", NULL);

   stemp   = _ProcessInline(gsArrayGetAt(stempArray, 1), para);
   result &= gfileSetS(file, gcTypeU1, stemp, NULL);
   gsDestroy(stemp);

   result &= gfileSetA(file, gcTypeU1, "\n\n", NULL);

   gsArrayForEach(stempArray, gsDestroyFunc);
   gsArrayDestroy(stempArray);

   greturn result;
}

/******************************************************************************
func: _FileWritePageBreak
******************************************************************************/
static Gb _FileWritePageBreak(Gfile * const file)
{
   Gb result;

   genter;

   result = gfileSetA(file, gcTypeU1, "\n\n---\n\n", NULL);

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

   stemp = _ProcessInline(para->str, para);

   result &= gfileSetA(file, gcTypeU1, "\n",  NULL);
   result &= gfileSetS(file, gcTypeU1, stemp, NULL);
   result &= gfileSetA(file, gcTypeU1, "\n",  NULL);

   gsDestroy(stemp);

   greturn result;
}

/******************************************************************************
func: _FileWriteScopeStart
******************************************************************************/
static Gb _FileWriteScopeStart(Gfile * const file, ParaType const type,
   Gindex const indentLevel, ParaType const parentType)
{
   Gb     result;
   Gindex index;

   genter;

   result = gbTRUE;

   switch (type)
   {
   case paraTypeFORMATED:
   case paraTypeSCOPE_FORMATED:
      result &= gfileSetA(file, gcTypeU1, "\n```\n", NULL);
      break;

   case paraTypeITEM:
   case paraTypeSCOPE_ITEM:
      result &= gfileSetA(file, gcTypeU1, "\n", NULL);
      forCount (index, indentLevel)
      {
         result &= gfileSetA(file, gcTypeU1, "    ", NULL);
      }
      
      switch (parentType)
      {
      default:
      case paraTypeSCOPE_LIST_BULLET:
         result &= gfileSetA(file, gcTypeU1, "* ", NULL);
         break;

      case paraTypeSCOPE_LIST_NUMBER:
         result &= gfileSetA(file, gcTypeU1, "1. ", NULL);
         break;
      }
      break;

   case paraTypeSCOPE_LIST_BULLET:
   case paraTypeSCOPE_LIST_KEY_VALUE:
   case paraTypeSCOPE_LIST_NUMBER:
   case paraTypeSCOPE_TABLE:
      result &= gfileSetA(file, gcTypeU1, "\n", NULL);
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
      result &= gfileSetA(file, gcTypeU1, "| ",       NULL);
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
      result &= gfileSetA(file, gcTypeU1, "\n```\n", NULL);
      break;

   case paraTypeITEM:
   case paraTypeSCOPE_ITEM:
      result &= gfileSetA(file, gcTypeU1, "\n", NULL);
      break;

   case paraTypeSCOPE_TABLE:
      result &= gfileSetA(file, gcTypeU1, "\n", NULL);
      break;

   case paraTypeSCOPE_LIST_BULLET:
   case paraTypeSCOPE_LIST_KEY_VALUE:
   case paraTypeSCOPE_LIST_NUMBER:
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
      result &= gfileSetA(file, gcTypeU1, " ", NULL);
      break;

   case paraTypeTABLE_ROW:
      result &= gfileSetA(file, gcTypeU1, "|\n", NULL);
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
func: _FileWriteTableHeaderSeparator
******************************************************************************/
static Gb _FileWriteTableHeaderSeparator(Gfile * const file, ParaTypeArray const * const headerList)
{
   Gb     result;
   Gindex index;

   genter;

   result = gbTRUE;

   forCount (index, paraTypeArrayGetCount(headerList))
   {
      result &= gfileSetA(file, gcTypeU1, "| --- ", NULL);
   }
   result &= gfileSetA(file, gcTypeU1, "|\n", NULL);

   greturn gbTRUE;
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
   case paraTypeTITLE_1:
   case paraTypeTITLE_TOC_1: 
      result &= gfileSetA(file, gcTypeU1, "\n# ", NULL);
      break;

   case paraTypeTITLE_2:
   case paraTypeTITLE_TOC_2: 
      result &= gfileSetA(file, gcTypeU1, "\n## ",   NULL);
      break;

   case paraTypeTITLE_3:                                                           
   case paraTypeTITLE_TOC_3: 
      result &= gfileSetA(file, gcTypeU1, "\n### ",   NULL);
      break;

   case paraTypeTITLE_4:                                                           
   case paraTypeTITLE_TOC_4: 
      result &= gfileSetA(file, gcTypeU1, "\n#### ",   NULL);
      break;

   case paraTypeTITLE_5:                                                           
   case paraTypeTITLE_TOC_5: 
      result &= gfileSetA(file, gcTypeU1, "\n##### ",   NULL);
      break;
   }

   stemp   = _ProcessInline(para->str, para);
   result &= gfileSetS(file, gcTypeU1, stemp, NULL);
   gsDestroy(stemp);

   result &= gfileSetA(file, gcTypeU1, "\n\n", NULL);

   greturn gbTRUE;
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
      case paraTypeTITLE_TOC_1: result &= gfileSetA(file, gcTypeU1, "**",                NULL); break;
      case paraTypeTITLE_TOC_2: result &= gfileSetA(file, gcTypeU1, "    ",              NULL); break;
      case paraTypeTITLE_TOC_3: result &= gfileSetA(file, gcTypeU1, "        ",          NULL); break;
      case paraTypeTITLE_TOC_4: result &= gfileSetA(file, gcTypeU1, "            ",      NULL); break;
      case paraTypeTITLE_TOC_5: result &= gfileSetA(file, gcTypeU1, "                ",  NULL); break;
      }

      stemp = _ProcessInline(para->str, para);
      result &= gfileSetS(file, gcTypeU1, stemp,         NULL);
      gsDestroy(stemp);

      switch (para->type)
      {
      case paraTypeTITLE_TOC_1: gfileSetA(file, gcTypeU1, "**",  NULL); break;
      }

      gfileSetA(file, gcTypeU1, "<br />\n",  NULL);
   }

   greturn result;
}
