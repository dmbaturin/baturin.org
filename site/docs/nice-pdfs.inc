<h1>Making nice and easy to navigate PDFs</h1>

<div id="generated-toc"> </div>

<p>
For some reason many people make PDFs that are nearly impossible to navigate. The PDF format provides the best of both worlds:
it's a paged medium with fixed page geometry and thus allows traditional typesetting, but it's also a digital medium that
allows full text search and uniquely digital navigation methods such as clickable hyperlinks and document outline menus.
</p>
<p>
While typesetting quality is somewhat subjective, and I don't mind reading poorly typeset documents if their content is worth it,
I find lack of digital navigation aids very irritating, not least because they are so easy to produce automatically in
almost any decent publishing system or office suite.
</p>
<p>
If you make a PDF, please set the correct title and include the outline, or a hyperlinked ToC, or both.
It will show your readers that you really care about them and add a professional touch to your documents.
It's simple, I'll show you how.
</p>

<h2>Document title</h2>

<p>
A minor issue with PDFs exported from text processors I always find annoying is that they often have the original file name in their
title, or, worse, have it prefixed with the name of the text processor. It's absolutely not informative, it makes it hard to find
the window you want if you have too many windows open (is there anyone this doesn't happen to?) and it just looks sloppy.
</p>
<p>
A real title of a document in the window title simply looks a lot better than &ldquo;Microsoft Word - my document.docx&rdquo;.
</p>

<h3>Open/Libre Office Writer</h3>

<p>
In the File menu, go to Properties. In the properties dialog, select the Description tab and enter the title there.
</p>

<img src="/images/nice-pdfs/oowriter_properties_title.png" alt="Libre Office Writer file properties dialog" />

<h3>Google Docs</h3>

<p>
I couldn't find a way to set a PDF title in Google Docs. Apparently the only option is to import into something else
and re-export from there.
</p>

<h3>LaTeX</h3>

<p>Use the <em>pdftitle</em> option in <em>\hypersetup</em>. Example:</p>
<pre>
\hypersetup{
    ...
    pdftitle={Making nice PDFs},
    pdfauthor={Daniil Baturin}
}
</pre>

<h2>Outline (also known as bookmarks)</h2>

<p>
The outline seems to be a relatively obscure feature for some reason. While everyone has seen and used it at least once,
there's no standardized name for it. It is a document structure tree generated from its ToC that is displayed in the side pane of the
PDF viewer.
</p>
<p>
In different PDF software it's variously named &ldquo;outline&rdquo;, &ldquo;index&rdquo;,
or &ldquo;bookmarks&rdquo;. I'll use the term &ldquo;outline&rdquo; because it doesn't conflict with anything else, both index
and bookmarks have additional meanings in the PDF context. This is what is looks like in Atril (which calls it index):
</p>

<img src="/images/nice-pdfs/pdf_index.png" alt="PDF outline side pane in Atril" />


<h3>Open/Libre Office Writer</h3>

<p>
LibreOffice Writer is capable of producing both outlines and hyperlinked tables of contents. The ToC in the exported PDFs is hyperlinked by default.
The outline is not enabled by default, but it's easy to enable.
</p>
<p>
In the Export as PDF dialog, check the Export Bookmarks checkbox.
</p>

<img src="/images/nice-pdfs/oowriter_pdf_bookmarks.png" alt="Export as PDF, check the Export Bookmarks checkbox" />

<h3>Google Docs</h3>

<p>
Hyperlinked ToC remains hyperlinked in the exported PDF. However, there's no way to produce an outline.
Looks like the only solution is to import into something else and export to PDF from there.
</p>

<h3>LaTeX</h3>

<p>
The outline is called bookmarks in the world of TeX. Note that you'll need to compile the document with pdflatex
if you want PDF-specific features to work.
</p>
<p>
There are lots of options for controlling the PDF output, but the minimal setup that you'll need to enable the bookmarks/outline
is just:
</p>
<pre>
\usepackage[pdftex, bookmarks=true]{hyperref}
</pre>

<p>
However, if your document is in a language other than English and you are using the unicode inputenc, don't forget to add
the &ldquo;unicode&rdquo; option.
</p>

<pre>
\usepackage[unicode, pdftex, bookmarks=true]{hyperref}
</pre>

<time id="last-modified">2018-04-08</time>
