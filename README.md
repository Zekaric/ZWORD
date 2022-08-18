
# Zekaric: WORD


## Table Of Contents

**1 - About Z:WORD**<br />
    1.1 - Description<br />
    1.2 - Why?<br />
    1.3 - Goals<br />
**2 - Usage**<br />
**3 - Format**<br />
    3.1 - File level markup.<br />
    3.2 - Inline level markup<br />

# 1 - About Z:WORD


## 1.1 - Description


WORD : W)ords O)rganized R)udimentary D)ocumentation

This program converts zword files to different targets.  The the goals for speed of writing, modification, and generation of target formats.

The format is a simple text file using simple markup for styling.

## 1.2 - Why?


I have been doing a lot of documentation lately.  And All options for writing doc are getting to be very annoying and there are too many options...  So I made yet another option.  Go figure.

Why not use MS Word?  I was using word for a while.  I had a template with hot keys all set up and the doc created was consistent in display and worked realy well.  Problem is that in order to view it you had to down load the file and open it in MS Word.  It wasn't online viewable easily.  Old Word files suffer from neglect.

Why not use HTML?  There isn't an HTML editor that I enjoy using.  They all frustrate at some point.  I even tried to write doc manually in a simple text editor but that ended up being more annoying than anything else.

Why not use a Wiki?  Markup languages on wiki's are not consistent.  Sometimes hard to get what you want.  At work the wiki's and software we used changed over time so old stuff was not easy to port over or left where they were but no one goes there anymore so no one knows the doc exists.

Why not MD?  GitHub uses MD...  And as far as I know, only GitHub.  Nothing at my work uses GitHub.

Why not use what is being used at work?  Again no standard markup and very flakey or annoying web interface.

Why not plain text?  Plain text is good but it lacks a certain presentation.  Plus when it comes to editing to a line length, you end up spending a lot of time just editing line length instead of working on content.

## 1.3 - Goals



* Simple markup that is easily remembered.

* Fast to write the markup format and still have it readable.

* Simple converted to other targets.

# 2 - Usage


[cmd] will be one of...

****[cmd]****<br>
    **Description**

**all**<br>
    Convert the zword file to all supported formats.

**html**<br>
    Convert the zword file to an html file.

**md**<br>
    Convert the zword file to a markdown file.

**rtf**<br>
    Convert the zword file to an RTF file.  A paperFile file is required.  Currently does not do tables.


# 3 - Format


Everything is based on the idea of a paragraph.

The markup is just the first tag of a paragraph, must not have any space before the tag as a space denotes a regular paragraph.

A blank line is meaningless but it can be used for making the file more readable.

## 3.1 - File level markup.


Note, not all markup may have a meaning in the target format that you will ask the converter to target.

****Markup****<br>
    **Description**

**h, hh, hhh, hhhh, hhhhh**<br>
    Title lines.  "h" is the most significant title. "hh" less significant.  etc.  There title lines will be included in a table of contents if "toc" markup is used.  Single line of text only.

**H, HH, HHH, HHHH, HHHHH**<br>
    Title lines.  Exactly like "h" except these titles will not be included in the table of contents.

**toc**<br>
    Insert a table of contents.  Nothing follows this tag.

**[white space]**<br>
    Any white space (space or tab), will denote the paragraph is just a regular text paragraph.  Do not split the paragraph over multiple lines.  A paragraph should just be on one line up to the new line.  Set your text editor to wrap text.  You can add empty lines to spread the markup for readability.  Empty lines are ignored.

**/, //**<br>
    Comment line.  Line is ignored.

**:[**<br>
    Start of a bullet point list block.  Nothing follows this tag.

**#[**<br>
    Start of a numbered list block.  Nothing follows this tag.

**=[**<br>
    Start of a key value point list block.  Nothing follows this tag.

**-**<br>
    A simple point on one line.  The "." denotes nested level.  If a key value item then the key is the part of the string before the first tab character and the value is the rest of the string.

**-[**<br>
    Start of a complex point block.  You can nest lists this way or have other paragraphs inside.  Nothing follows this tag.

**t[**<br>
    Start of a table block.  Nothing follows this tag.

**t-**<br>
    New row marker.  Nothing follows this tag.

**th**<br>
    New column header marker.  Contents follow the tag on one line.

**thx**<br>
    New column header marker.  Contents follow the tag on one line. The contents will not be broken at whitespace.

**th&#42;**<br>
    New column header marker.  Contents follow the tag on one line. The column will fill the remaining space of the table.

**tc**<br>
    New column marker.  Contents follow the tag on one line.

**tcx**<br>
    New column marker.  Contents follow the tag on one line.  The contents will not be broken at whitespace.

**tc&#42;**<br>
    New column marker.  Contents follow the tag on one line.  The column will fill the remaining space of the table.

**tc1**<br>
    New column marker.  Contents follow the tag on one line.  Number column, right aligned.

**tc1&#42;**<br>
    New column marker.  Contents follow the tag on one line.  Number column, right aligned.  The column will fill the remaining space of the table.

**.**<br>
    A simple monospaced text string on one line.  No inline markup processing performed on the contents.

**.[**<br>
    Start of a monospaced text block.  No inline markup processing performed on the contents.  Nothing follows this tag.

**]**<br>
    End of a block.  Nothing follows this tag.

**===**<br>
    Insert a page break.  Nothing follows this tag.

**v**<br>
    Variable.  The variable Key is first followed by a tab, and the rest of the line is the Value for the variable.  To use this variable inline surround the key with &#124; characters.  Care must be taken to ensure you are not redefining an existing code or adding recursion.  Meaning a variable should not include itself in the value or a variable that will eventually include this variable again.


## 3.2 - Inline level markup


****Markup****<br>
    **Descrption**

**&#124;chapter&#124;**<br>
    Chapter number.  "3.1.4.1.5.9"

**&#124;b &#124;...&#124; b&#124;**<br>
    **Bold** the contents. Space is important.

**&#124;i &#124;...&#124; i&#124;**<br>
    *Italicize* the contents. Space is important.

**&#124;u &#124;...&#124; u&#124;**<br>
    <u>Underline</u> the contents. Space is important.

**&#124;s &#124;...&#124; s&#124;**<br>
    ~~Strike~~ through the contents. Space is important.

**&#124;' &#124;...&#124; '&#124;**<br>
    <sup>Superscript</sup> the contents. Space is important.

**&#124;, &#124;...&#124; ,&#124;**<br>
    <sub>Subscript</sub> the contents. Space is important.

**&#124;. &#124;...&#124; .&#124;**<br>
    `monospace` the contents. Space is important.

**&#124;img &#124;...&#124; img&#124;**<br>
    Insert an image inline.  Path must be relative to the zword file.

**&#124;link &#124;...&#124; link &#124;...&#124; link&#124;**<br>
    Create a [LINK](http://www.zekaric.com/) web link.  First ... is the line.  Second ... is the nice text.

**&#124;line&#124;**<br>
    Break the line.  Move to the next line.

**&#124;line &#124;**<br>
    Break the line.  Leave a space.  Start on the next line.

**&#124;line  &#124;**<br>
    Break the line.  Leave two spaces.  Start on the next line.

**&#124;_&#124;**<br>
    Non breaking space.

**" &#124; "**<br>
    Escape for a |.  No space between "s and |.  Only required if the use of the | could be misinterpreted as one of these inline markup sequences.

**" _ "**<br>
    Empty character.  No space between "s and _.

**&#124;&#42;...&#124;**<br>
    Insert a symbol.  No space between &#42; and the following words.  One of...<br /><br />**Legal:** copyright, registered tm, tm,<br /><br />|&#42;copyright||&#42;registered tm||&#42;tm|<br /><br />**Currency:** bitcoin, cent, dollar, euro, franc, lira, lira turkey, peso, pound, ruble, rupee, rupee india, won, yen<br /><br />|&#42;bitcoin||&#42;cent||&#42;dollar||&#42;euro||&#42;franc||&#42;lira||&#42;lira turkey||&#42;peso||&#42;pound||&#42;ruble||&#42;rupee||&#42;rupee india||&#42;won||&#42;yen|<br /><br />**Punctuation:** ..., 1/4, 1/2, 3/4, ampersand, at, bullet, dagger s, dagger d, hash, inv!, inv?, quote angle d l, quote angle d r, quote angle s l, quote angle s r, quote d, quote d l, quote d r, quote d low, quote s, quote s l, quote s r, quote s low, para, prime d, prime s<br /><br />|&#42;...||&#42;1/4||&#42;1/2||&#42;3/4||&#42;ampersand||&#42;at||&#42;bullet||&#42;dagger s||&#42;dagger d||&#42;hash||&#42;inv!||&#42;inv?||&#42;quote angle d l||&#42;quote angle d r||&#42;quote angle s l||&#42;quote angle s r||&#42;quote d||&#42;quote d l||&#42;quote d r||&#42;quote d low||&#42;quote s||&#42;quote s l||&#42;quote s r||&#42;quote s low||&#42;para||&#42;prime d||&#42;prime s|<br /><br />**Game:** arrow d, arrow l, arrow r, arrow u, arrow lr, arrow ud, arrow \d, arrow /d, arrow \u, arrow /u, arrow d l, arrow d lr, arrow d r, arrow redo, arrow undo, card suit c, card suit d, card suit h, card suit s, chess b k, chess b q, chess b r, chess b b, chess b h, chess b p, chess w k, chess w q, chess w r, chess w b, chess w h, chess w p, die 1, die 2, die 3, die 4, die 5, die 6, ball baseball, ball soccer<br /><br />|&#42;arrow d||&#42;arrow l||&#42;arrow r||&#42;arrow u||&#42;arrow lr||&#42;arrow ud||&#42;arrow \d||&#42;arrow /d||&#42;arrow \u||&#42;arrow /u||&#42;arrow d l||&#42;arrow d lr||&#42;arrow d r||&#42;arrow redo||&#42;arrow undo||&#42;card suit c||&#42;card suit d||&#42;card suit h||&#42;card suit s||&#42;chess b k||&#42;chess b q||&#42;chess b r||&#42;chess b b||&#42;chess b h||&#42;chess b p||&#42;chess w k||&#42;chess w q||&#42;chess w r||&#42;chess w b||&#42;chess w h||&#42;chess w p||&#42;die 1||&#42;die 2||&#42;die 3||&#42;die 4||&#42;die 5||&#42;die 6||&#42;ball baseball||&#42;ball soccer|<br /><br />**Symbols:** checkbox off, checkbox on, circle 0%, circle 25%, circle 50%, circle 75%, circle 100%, correct, biohazard, dot, dot filled, envelope, gender f, gender m, gender mf, incorrect, plane, poison, radioactive, Rx, recycle, scissor, snowflake, star 5, star 5 filled, sun<br /><br />|&#42;checkbox off||&#42;checkbox on||&#42;circle 0%||&#42;circle 25%||&#42;circle 50%||&#42;circle 75%||&#42;circle 100%||&#42;correct||&#42;biohazard||&#42;dot||&#42;dot filled||&#42;envelope||&#42;gender f||&#42;gender m||&#42;gender mf||&#42;incorrect||&#42;plane||&#42;poison||&#42;radioactive||&#42;Rx||&#42;recycle||&#42;scissor||&#42;snowflake||&#42;star 5||&#42;star 5 filled||&#42;sun|<br /><br />**Math:** +/-, angle, contains, degree, dot, empty set, equal almost, equal approx, equal greater than, equal less than, equal same, equal not, for all, infinity, integral, intersection, is in, is not in, math phi, math pi, minus, nabla, partial diff, product, proportional to, root, sum, there exists, therefore, union<br /><br />|&#42;+/-||&#42;angle||&#42;contains||&#42;degree||&#42;dot||&#42;empty set||&#42;equal almost||&#42;equal approx ||&#42;equal greater than||&#42;equal less than||&#42;equal same||&#42;equal not||&#42;for all||&#42;infinity||&#42;integral||&#42;intersection||&#42;is in||&#42;is not in||&#42;math phi||&#42;math pi||&#42;minus||&#42;nabla||&#42;partial diff||&#42;product||&#42;proportional to||&#42;root||&#42;sum||&#42;there exists||&#42;therefore||&#42;union|<br /><br />**Greek:** alpha cap, beta cap, gamma cap, delta cap, epsilon cap, zeta cap, eta cap, theta cap, iota cap, kappa cap, lambda cap, mu cap, nu cap, xi cap, omicron cap, pi cap, rho cap, signma cap, tau cap, upsilon cap, phi cap, chi cap, psi cap, omega cap, alpha, beta, gamma, delta, epsilon, zeta, eta, theta, iota, kappa, lambda, mu, nu, xi, omicron, pi, rho, signma, tau, upsilon, phi, chi, psi, omega<br /><br />|&#42;alpha cap||&#42;beta cap||&#42;gamma cap||&#42;delta cap||&#42;epsilon cap||&#42;zeta cap||&#42;eta cap||&#42;theta cap||&#42;iota cap||&#42;kappa cap||&#42;lambda cap||&#42;mu cap||&#42;nu cap||&#42;xi cap||&#42;omicron cap||&#42;pi cap||&#42;rho cap||&#42;signma cap||&#42;tau cap||&#42;upsilon cap||&#42;phi cap||&#42;chi cap||&#42;psi cap||&#42;omega cap||&#42;alpha||&#42;beta||&#42;gamma||&#42;delta||&#42;epsilon||&#42;zeta||&#42;eta||&#42;theta||&#42;iota||&#42;kappa||&#42;lambda||&#42;mu||&#42;nu||&#42;xi||&#42;omicron||&#42;pi||&#42;rho||&#42;signma||&#42;tau||&#42;upsilon||&#42;phi||&#42;chi||&#42;psi||&#42;omega|

