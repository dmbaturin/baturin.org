{% set date 2017 December 05 %}

<h1>Making easy to navigate PDFs</h1>

<p>
For some reason many people make PDFs that are nearly impossible to navigate. PDF is a format that is best of both worlds:
it's a paged medium with fixed page geometry and thus allows traditional typesetting, but it's also a digital medium that
allows full text search and uniquely digital navigation methods such as clickable hyperlinks and document outline menus.
</p>
<p>
While typesetting quality is somewhat subjective, and I don't mind reading poorly typeset documents if their content is worth it,
I find lack of digital navigation aids very irritating, especially because they are so easy to produce automatically with
almost any decent publishing system or office suite.
</p>
<p>
If you respect your readers, please include the outline, or a hyperlinked ToC, or both. It's simple, I'll tell you how.
</p>

<h2>The outline</h2>

<p>
The outline seems to be a relatively obscure feature for some reason. While everyone has seen and used it at least once,
there's no standardized name for it. It is a document structure tree generated from its ToC that is displayed in the side pane of the
PDF viewer.
</p>
<p>
In different PDF software it's variously named &ldquo;outline&rdquo;, &ldquo;index&rdquo;,
or &ldquo;bookmarks&rdquo;. I'll use the term &ldquo;outline&rdquo; because it doesn't conflict with anything else, both index
and bookmarks have additional meanings. This is what is looks like in Atril (which calls it index):
</p>

<img src="/images/nice-pdfs/pdf_index.png" alt="PDF outline side pane in Atril" />


<h3>Open/Libre Office Writer</h3>

<p>
LibreOffice Writer is capable of producing both outlines and hyperlinked tables of contents. The ToC in the exported PDFs is hyperlinked by default.
The outline is not enabled by default, but it's easy to enable.
</p>

<img src="/images/nice-pdfs/oowriter_pdf_bookmarks.png" alt="Export as PDF, check the Export Bookmarks checkbox" />

<h3>Google Docs</h3>

<p>
Hyperlinked ToC remains hyperlinked in the exported PDF. However, there's  no way to produce indexed PDFs.
Looks like the only solution is to import to something else and export to PDF from there.
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
