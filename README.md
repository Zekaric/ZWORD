
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

[cmd] will be one of...


# Format

Everything is based on the idea of a paragraph.

The markup is just the first tag of a paragraph, must not have any space befor the tag as a space denotes a regular paragraph.

A blank line is meaningless but it can be used for making the file more readable.

## File level markup.

Note, not all markup may have a meaning in the target format that you will ask the converter to target.


## Inline level markup

[Currently not implemented.]

