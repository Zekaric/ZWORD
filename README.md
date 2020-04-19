
# Zekaric: Doccer

## Table Of Contents
**About Z:Doccer**
 Description
 Why?
 Goals
**Usage**
**Format**
 File level markup.
 Inline level markup

# About Z:Doccer

## Description

This program converts zdoc files to different targets.  The the goals for speed of writing, modification, and generation of target formats.

The format is a simple text file using simple markup for styling.

## Why?

I have been doing a lot of documentation lately.  And All options for writing doc are getting to be very annoying and there are too many options...  So I made yet another option.  Go figure.

Why not use MS Word?  I was using word for a while.  I had a template with hot keys all set up and the doc created was consistent in display and worked realy well.  Problem is that in order to view it you had to down load the file and open it in MS Word.  It wasn't online viewable easily.  Old Word files suffer from neglect.

Why not use HTML?  There isn't an HTML editor that I enjoy using.  They all frustrate at some point.  I even tried to write doc manually in a simple text editor but that ended up being more annoying than anything else.

Why not use a Wiki?  Markup languages on wiki's are not consistent.  Sometimes hard to get what you want.  At work the wiki's and software we used changed over time so old stuff was not easy to port over or left where they were but no one goes there anymore so no one knows the doc exists.

Why not MD?  GitHub uses MD...  And as far as I know, only GitHub.  Nothing at my work uses GitHub.

Why not use what is being used at work?  Again no standard markup and very flakey or annoying web interface.

Why not plain text?  Plain text is good but it lacks a certain presentation.  Plus when it comes to editing to a line length, you end up spending a lot of time just editing line length instead of working on content.

## Goals

* Simple markup that is easily remembered.

* Fast to write the markup format and still have it readable.

* Simple converted to other targets.


# Usage

```
zdoc [cmd] [path to file name].doc
```

[cmd] will be one of...
| [cmd] | Description |
| all | Convert the zdoc file to all supported formats. |
| html | Convert the zdoc file to an html file. |
| md | Convert the zdoc file to a markdown file. |


# Format

Everything is based on the idea of a paragraph.

The markup is just the first tag of a paragraph, must not have any space befor the tag as a space denotes a regular paragraph.

A blank line is meaningless but it can be used for making the file more readable.

## File level markup.

Note, not all markup may have a meaning in the target format that you will ask the converter to target.
| Markup | Description |
| *, **, ***, ... ********* | Title lines.  "*" is the most significant title. "**" less significant.  etc.  There title lines will be included in a table of contents if "toc" markup is used.  Single line of text only. |
| ~, ~~, ~~~, ... ~~~~~~~~~ | Title lines.  Exactly like "*" except these titles will not be included in the table of contents. |
| toc | Insert a table of contents.  Nothing follows this tag. |
| [white space] | Any white space, like a table or a space, will denote the paragraph is just a regular text paragraph.  You can split the paragraph over multiple lines however the following lines must not start with a known markup tag or a space.  If it does then the program will think it is a new paragraph. |
| =[ | Start of a bullet point list.  Nothing follows this tag. |
| ]= | End of a bullet point list.  Nothing follows this tag. |
| #[ | Start of a numbered point list.  Nothing follows this tag. |
| ]# | End of a bullet point list.  Nothing follows this tag. |
| - | A simple bullet point on one line. |
| -[ | Start of a complex bullet point.  You can nest lists this way or have other paragraphs inside. |
| ]- | End of a bullet point list. |
| t[ | Start of a table.  Nothing follows this tag. |
| ]t | End of a table.  Nothing follows this tag. |
| t- | New row marker.  Nothing follows this tag. |
| th | New column header marker.  Contents follow the tag on one line. |
| thx | New column header marker.  Contents follow the tag on one line. The contents will not be broken at whitespace. |
| th* | New column header marker.  Contents follow the tag on one line. The column will fill the remaining space of the table. |
| t| | New column marker.  Contents follow the tag on one line. |
| t|x | New column marker.  Contents follow the tag on one line.  The contents will not be broken at whitespace. |
| t|* | New column marker.  Contents follow the tag on one line.  The column will fill the remaining space of the table. |
| [ | A simple formated text string on one line. |
| [[ | Start of a formated text block. |
| ]] | End of a bullet point list. |
| === | Insert a page break. |


## Inline level markup

[Currently not implemented.]
| Markup | Descrption |
| [chapter] | Chapter number.  "3.1.4.1.5.9" |
| [img ...] | Insert an image inline.  Path must be relative to the zdoc file. |
| [b ...] | Bold the contents. |
| [i ...] | Italicize the contents. |
| [_ ...] | Underline the contents. |
| [- ...] | Strick through the contents. |
| [^ ...] | Superscript the contents. |
| [v ...] | Subscript the contents. |
| [* ...] | Add an index item. |
| [link ...|...] | Create a web link.  First ... is the line.  Second ... is the nice text. |
| [sym ...] | Insert a symbol.  One of... |
|  | "copy" - copyright |
|  | "reg" - registered trademark |
|  | "pound" - British Pound currency |
|  | "euro" - Euro currency |
|  | "yen" - Japanese Yen currency |
|  | "dollar" - US/CDN/AUS/Etc. Dollar currency |
|  | "cent" - US/CDN/AUS/etc. Cent currency |
|  | "amp" - ampersand |
|  | "hash" - hash tag, pount sign, number significant |
|  | "at" - at sign |

