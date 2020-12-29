
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
    Convert the zword file to an RTF file.  A zwordRTF.def file is required.


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


## 3.2 - Inline level markup


****Markup****<br>
    **Descrption**

**&#124;chapter&#124;**<br>
    Chapter number.  "3.1.4.1.5.9"

**&#124;b ... b&#124;**<br>
    **Bold** the contents. Space is important.

**&#124;i ... i&#124;**<br>
    *Italicize* the contents. Space is important.

**&#124;u ... u&#124;**<br>
    <u>Underline</u> the contents. Space is important.

**&#124;s ... s&#124;**<br>
    ~~Strike~~ through the contents. Space is important.

**&#124;' ... '&#124;**<br>
    <sup>Superscript</sup> the contents. Space is important.

**&#124;, ... ,&#124;**<br>
    <sub>Subscript</sub> the contents. Space is important.

**&#124;. ... .&#124;**<br>
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

**\ &#124; , &#124; \ **<br>
    Escape for a \` No space between &#124; and \.

**&#124;&#42;...&#124;**<br>
    Insert a symbol.  No space between &#42; and the following words.  One of...<br /><br />**Legal:** copyright, registered tm, tm,<br /><br />&copy;&reg;&trade;<br /><br />**Currency:** bitcoin, cent, dollar, euro, franc, lira, lira turkey, peso, pound, ruble, rupee, rupee india, won, yen<br /><br />&#x20BF;&cent;$&euro;&#x20A3;&#x20A4;&#x20BA;&#x20B1;&pound;&#x20BD;&#x20A8;&#x20B9;&#x20A9;&yen;<br /><br />**Punctuation:** ..., 1/4, 1/2, 3/4, ampersand, at, bullet, dagger s, dagger d, hash, inv!, inv?, quote angle d l, quote angle d r, quote angle s l, quote angle s r, quote d, quote d l, quote d r, quote d low, quote s, quote s l, quote s r, quote s low, para, prime d, prime s<br /><br />&hellip;&frac14;&frac12;&frac34;&amp;@&bull;&dagger;&Dagger;#&iexcl;&iquest;&laquo;&raquo;&lsaquo;&rsaquo;"&ldquo;&rdquo;&bdquo;'&lsquo;&rsquo;&sbquo;&para;&Prime;&prime;<br /><br />**Game:** arrow d, arrow l, arrow r, arrow u, arrow lr, arrow ud, arrow \d, arrow /d, arrow \u, arrow /u, arrow d l, arrow d lr, arrow d r, arrow redo, arrow undo, card suit c, card suit d, card suit h, card suit s, chess b k, chess b q, chess b r, chess b b, chess b h, chess b p, chess w k, chess w q, chess w r, chess w b, chess w h, chess w p, die 1, die 2, die 3, die 4, die 5, die 6, ball baseball, ball soccer<br /><br />&darr;&larr;&rarr;&uarr;&harr;&#x2195;&#x2198;&#x2199;&#x2196;&#x2197;&lArr;&hArr;&rArr;&#x21B7;&#x21B6;&clubs;&diams;&hearts;&spades;&#x265A;&#x265B;&#x265C;&#x265D;&#x265E;&#x265F;&#x2654;&#x2655;&#x2656;&#x2657;&#x2658;&#x2659;&#x2680;&#x2681;&#x2682;&#x2683;&#x2684;&#x2685;&#x26BE;&#x26BD;<br /><br />**Symbols:** checkbox off, checkbox on, circle 0%, circle 25%, circle 50%, circle 75%, circle 100%, correct, biohazard, dot, dot filled, envelope, gender f, gender m, gender mf, incorrect, plane, poison, radioactive, Rx, recycle, scissor, snowflake, star 5, star 5 filled, sun<br /><br />&#x2610;&#x2611;&#x25CB;&#x25D4;&#x25D1;&#x25D5;&#x25CF;&#x2713;&#x2623;&#x26AA;&#x26AB;&#x2709;&#x2640;&#x2642;&#x26A5;&#x2717;&#x2708;&#x2620;&#x2622;&#x211E;&#x267B;&#x2702;&#x2744;&#x2606;&#x2605;&#x2600;<br /><br />**Math:** +/-, angle, contains, degree, dot, empty set, equal almost, equal approx, equal greater than, equal less than, equal same, equal not, for all, infinity, integral, intersection, is in, is not in, math phi, math pi, minus, nabla, partial diff, product, proportional to, root, sum, there exists, therefore, union<br /><br />&plusmn;&ang;&ni;&deg;&#x26AA;&empty;&asymp;&cong;&ge;&le;&equiv;&ne;&forall;&infin;&int;&cap;&isin;&notin;&straightphi;&piv;&minus;&nabla;&part;&prod;&prop;&radic;&sum;&exist;&there4;&cup;<br /><br />**Greek:** alpha cap, beta cap, gamma cap, delta cap, epsilon cap, zeta cap, eta cap, theta cap, iota cap, kappa cap, lambda cap, mu cap, nu cap, xi cap, omicron cap, pi cap, rho cap, signma cap, tau cap, upsilon cap, phi cap, chi cap, psi cap, omega cap, alpha, beta, gamma, delta, epsilon, zeta, eta, theta, iota, kappa, lambda, mu, nu, xi, omicron, pi, rho, signma, tau, upsilon, phi, chi, psi, omega<br /><br />&Alpha;&Beta;&Gamma;&Delta;&Epsilon;&Zeta;&Eta;&Theta;&Iota;&Kappa;&Lambda;&Mu;&Nu;&Xi;&Omicron;&Pi;&Rho;&Sigma;&Tau;&Upsilon;&Phi;&Chi;&Psi;&Omega;&alpha;&beta;&gamma;&delta;&Epsilon;&zeta;&eta;&theta;&iota;&kappa;&lambda;&mu;&nu;&xi;&Omicron;&pi;&rho;&sigma;&tau;&Upsilon;&phi;&chi;&psi;&omega;

