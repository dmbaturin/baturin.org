<h1>soupault</h1>

<div id="generated-toc"> </div>

<h2>Overview</h2>

<h3>Background</h3>
<p>Soupault (<em>soup-oh</em>) is a static website generator based on HTML rewriting.</p>
<p>
Pretty much like client-side JS scripts for generating a table of contents from <code>&lt;h1&gt;</code>,
<code>&lt;h2&gt;</code>, ... tags and similar tasks, just without the browser or interactivity.
</p>
<p>
It's named after the French dadaist and surrealist writer Philippe Soupault
because it's based on the <a href="https://github.com/aantron/lambdasoup">Lambda Soup</a>
library, which is a reference to the <a href="https://www.crummy.com/software/BeautifulSoup">Beautiful Soup</a>
library, which is a reference to the Mock Turtle chapter from Alice in Wonderland,
which is a reference an actual turtle soup imitation recipe, and also a reference
to <a href="https://en.wikipedia.org/wiki/Tag_soup">tag soup</a>, a derogatory term
for malformed HTML, which is a reference to... in any case, soupault is not the French
for a stick horse, for better or worse.
</p>
<p>Soupault is not like other website generators:</p>
<ol>
  <li>No special syntax inside pages, only plain old semantic HTML.</li>
  <li>No templates or themes. Any HTML page can be a soupault theme.</li>
  <li>No front matter. You tell soupault where to find metadata in your page using CSS selectors.</li>
</ol>
<p>
People have written lots of static website generators, but most of them are variations
on a single theme: take a file with some metadata (<em>front matter</em>) followed by content in a limited
markup format such as Markdown and feed it to a template processor.
</p>
<p>
This approach works well in many cases, but has its limitations. A template processor is only aware of its
own tags, but not of the full page structure. The front matter metadata is the only part of the
page source it can use, and it can only insert it into fixed locations in the template.
The part below the front matter is considered opaque, and formats like Markdown simply don't allow you to
add machine-readable annotations anyway.
</p>
<p>
Valid HTML, however, is a machine-readable format, and the libraries that can handle it existed for a long time.
As shown by <a href="http://microformats.org/">microformats</a>, you can embed a lot of information in it. More importantly,
unlike front matter metadata, HTML metadata is multi-purpose: for example, you can use the <code>id</code>
attribute for CSS styling and as a page anchor, as well as a unique element identifier for data extraction
or HTML manipulation programs.
</p>
<h3>How soupault works</h3>
<p>
Soupault takes a page &ldquo;template&rdquo;&mdash; an HTML file devoid of content,
parses it into an element tree, and locates the <em>content container</em> element inside it.
</p>
<p>
By default the content container is <code>&lt;body&gt;</code>, but you can use any selector:
<code>div#content</code> (a <code>&lt;div id="content"&gt;</code> element), <code>article</code> (the HTML5 article element),
<code>#post</code> (any element with <code>id="post"</code>) or any other valid CSS selector.
</p>
<p>
Then it traverses your site directory where page source files are stored, takes a page file, parses it into an HTML element tree too,
and inserts it into the <em>content container</em> element of the template.
</p>
<p>
The new HTML tree is then passed to <em>widgets</em>&mdash;HTML rewriting modules that manipulate it in different ways:
incude other files or outputs of external programs into specific elements, create breadcrumbs for your page,
they may delete unwanted elements too.
</p>
<p>
Processed pages are then written to disk, into a directory structure that mirrors your source
directory structure.
</p>

<h3>Why use soupault?</h3>

<h4>If you are starting a website</h4>
<p>
All website generators provide a default theme, but making new themes can be tricky.
With soupault, there are no additional steps between writing your page layout and building
your website from it.
</p>
<p>
In the simplest case you can just create a page skeleton in <code>templates/main.html</code>,
add a bunch of pages to <code>site/</code> (<code>site/about.html</code>, <code>site/cv.html</code>, ...)
and run the <code>soupault</code> command in that directory.
</p>

<h4>If you already have a website</h4>
<p>
If you have a website made of handwritten pages, you can use soupault as a drop-in automation tool.
Just take your existing page skeleton, strip the other pages down to their content,
and you are good to go.
</p>
<p>
If you have an existing directory structure that you don't want to change because it will break links,
soupault can mirror it exactly, with the <code>clean_urls = false</code> config option.
It also preseves original file names and extensions in that mode.
</p>

<h3>What kind of rewriting it can do</h3>
<p>
Soupalt comes with a bunch of built-in widgets that can:
</p>
<ul>
  <li>Include external files, HTML snippets, or output of external programs into an element.</li>
  <li>Set the page title from text in some element.</li>
  <li>Generate breadcrumbs.</li>
  <li>Move footnotes out of the text.</li>
  <li>Insert a table of contents</li>
  <li>Delete unwanted elements</li>
  <li>...more to come.</li>
</ul>
<p>For example, here's a config for the <code>title</code> widget that sets the page title.</p>
<pre>
[widgets.page-title]
  widget = "title"
  selector = "#title"
  default = "My Website"
  append = " &amp;mdash; My Website"
</pre>
<p>
It takes the text from the element with <code>id="title"</code> and copies it to the <code>&lt;title&gt;</code>
tag of the generated page. It can be any element, and it can be a different element in every page.
If you use <code>&lt;h1 id="title"&gt;</code> in <code>site/foo.html</code> and <code>&lt;strong id="title"&gt;</code>
in another, soupault will still find it.
</p>
<p>It's just as simple to prevent something from appearing on a particular page. Just don't use an element
that a widget uses for its target, and the widget will not run on that page.
</p>
<p>
Another example: to automatically include content of a <code>templates/nav-menu.html</code> into
the <code>&lt;nav&gt;</code> element, you can put this into your <code>soupault.conf</code> file:
</p>
<pre>
[widgets.nav-menu]
  widget = "include"
  selector = "nav"
  file = "templates/nav-menu.html"
</pre>

<h3>What soupault does not include</h3>

<p>By design:</p>
<dl>
  <dt>Development web server</dt>
  <dd>There are plenty of those, even <code>python3 -m http.server</code> is perfectly
      good for previews.</dd>
  <dt>Deployment automation</dt>
  <dd>Same reason, there are lots of tools for it.</dd>
</dl>

<p>Because it's not implemented yet:</p>
<dl>
  <dt>Section indices</dt>
  <dd>Will be there soon</dd>
  <dt>Running external preprocessors on a page file before parsing</dt>
  <dd>Will be there soon</dd>
  <dt>Asset management</dt>
  <dd>May or may not be added, feels out of scope</dd>
  <dt>Incremental builds</dt>
  <dd>May or may not be added</dd>
</dl>

<h2>Installation</h2>
<p>Soupault is under development. Right now you can only install it from <a href="https://github.com/dmbaturin/soupault">source</a>.</p>
<pre>
git clone https://github.com/dmbaturin/soupault.git
cd soupault
opam pin add soupault .
</pre>
<p>When it stabilizes, static binaries for GNU/Linux and Microsoft Windows will be available.</p>

<h2>Getting started</h2>
<h3>Create your first website</h3>
<p>
Soupault has only one file of its own: the config file <code>soupault.conf</code>.
It does not impose any particular directory layout on you. However, it has default
settings that allow you to run it unconfigured.
</p>
<p>You can initialize a simple project with default configuration using this command:</p>
<pre>
$ soupault --init
</pre>
<p>It will create the following directory structure:</p>
<pre>
.
├── site
│   └── index.html
├── templates
│   └── main.html
└── soupault.conf
</pre>
<ul>
  <li>The <code>site/</code> is a <em>site directory</em> where page files are stored.</li>
  <li>The <code>templates/</code> directory is just a convention, soupault uses <code>templates/main.html</code>
      as the default page template.</li>
  <li><code>soupault.conf</code> is the config file.</li>
</ul>
<p>
Now you can build your website. Just run <code>soupault</code> in your website directory,
and it will put the generated pages in <code>build/</code>. Your index page will become
<code>build/index.html</code>.
</p>
<p>
By default, soupault inserts page content into the <code>&lt;body&gt;</code> element
of the page template. Therefore, from the default template:
</p>
<pre>
&lt;html&gt;
  &lt;head&gt;&lt;/head&gt;
  &lt;body&gt;
  &lt;!-- your page content here --&gt;
  &lt;/body&gt;
&lt;/html&gt;
</pre>
<p>
and the default index page source that is <code>&lt;p&gt;Site powered by soupault&lt;/p&gt;</code>
it will make this page:
</p>
<pre>
&lt;!DOCTYPE html&gt;
&lt;html&gt;
 &lt;head&gt;&lt;/head&gt;
 &lt;body&gt;
  &lt;p&gt;
   Site powered by soupault
  &lt;/p&gt;
 &lt;/body&gt;
&lt;/html&gt;
</pre>
<p>
You can use any CSS selector to determine where your content goes. For example,
you can tell soupault to insert it into <code>&lt;div id="content&gt;</code>
by changing the <code>content_selector</code> in <code>soupault.conf</code>
to <code>content_selector = "div#content"</code>.

<h3>Page files</h3>
<p>
Everything in the site directory is assumed to be an HTML page. Extensions don't matter,
it can be <code>.html</code>, <code>.htm</code>, or something else entirely.
Soupault simply reads them, parses them, and insers into the element defined by the
<code>content_selector</code> option.
</p>
<ul>
  <li>If you put a picture in there, you'll get a page with a chunk of binary data in it.
      Store your assets elsewhere.</li>
  <li>Two pages with the same name but different extensions is undefined behaviour
      and anything can happen. Make sure file names are unique.</li>
</ul>

<h3>Clean URLs</h3>
<p>
Soupault uses clean URLs by default. If you add another page to <code>site/</code>,
for example, <code>site/about.html</code>, it will turn into <code>build/about/index.html</code>
so that it can be accessed as <code>https://mysite.example.com/about</code>.
Only pages not named <code>index</code> trigger subdirectory creation.
</p>
<p>
Index files, by default files whose name is <code>index</code> are simply copied to the target
directory. Note that right now soupault doesn't create any indices automatically,
you need to make one yourself.
</p>
<div class="info">
<p>
Note: a page named <code>foo.html</code> and a section directory named <code>foo/</code> is also
undefined behaviour when clean URLs are used. Don't do that to avoid unpredictable results.
</p>
</div>
<p>This is what soupault will make from a source directory:</p>
<pre>
$ tree site/
site/
├── about.html
├── cv.html
└── index.html

$ tree build/
build/
├── about
│   └── index.html
├── cv
│   └── index.html
└── index.html
</pre>

<h4>Disabling clean URLs</h4>
<p>
If you've had a website for a long time and there are links to your page that will
break if you change the URLs, you can make soupault mirror your site directory structure
exactly and preserve original file names.
</p>
<p>
Just add <code>clean_urls = false</code> to the <code>[settings]</code> table of your
<code>soupault.conf</code> file.
</p>
<pre>
[settings]
  clean_urls = false
</pre>

<h3>Nested structures</h3>
<p>
A flat layout is not always desirable. If you want to create site sections, just add some
directories to <code>site/</code>. Subdirectories are subsections, their subdirectories
are subsubsections and so on&mdash;they can go as deep as you want. Soupault will process
them all recursively and recreate the directories in <code>build/</code>.
</p>

<pre>
site/
├── articles
│   ├── goto-considered-harmful.html
│   ├── index.html
│   └── theorems-for-free.html
├── about.html
├── cv.html
└── index.html

build/
├── about
│   └── index.html
├── articles
│   ├── goto-considered-harmful
│   │   └── index.html
│   ├── theorems-for-free
│   │   └── index.html
│   └── index.html
├── cv
│   └── index.html
└── index.html
</pre>


<h2>Configuration</h2>
<p>
The default directory and file paths <code>soupault --init</code> creates are
not fixed, you can change any of them. If you prefer different names, or you
have an existing directory structure you want soupault to use, just edit
the <code>soupault.conf</code> file.
</p>
<pre>
[settings]
  # Where generated files go
  build_dir = "build"

  # Where page files are stored
  site_dir = "site"

  # Where to insert the content in the page template
  content_selector = "#content"

  default_template = "templates/main.html"

  # Name of the page used as a section index,
  # without extension
  index_page = "index"

  # Output file name for section indices (always)
  # and page directories (when clean URLs are used)
  # As in:
  # site/foo/index.inc -&gt; build/foo/index.html
  # site/foo.inc -&gt; build/foo/index.html
  index_file = "index.html"

  doctype = "&lt;!DOCTYPE html&gt;"

  # Don't print debugging information
  verbose = false

  # Don't fail on page processing errors
  strict = false

  # Create a directory per page, site/foo.html -&gt; build/foo/$index_file
  clean_urls = true
</pre>

<p>
Note that if you create <code>soupault.conf</code> file before running
<code>soupault --init</code>, it will use settings from that file instead
of default settings.
</p>
<p>
In this document, whenever a specific site or build dir has to be mentioned,
we'll use default values.
</p>

<h2>Widgets</h2>

<p>
Widgets provide additional functionality. When a page is processed, its content
is inserted into the template, and the resulting HTML element tree is passed
through a widget pipeline.
</p>
<h3>Widget behaviour</h3>
<p>
Widgets that require a <code>selector</code> option first check if there's an element matching
that selector in the page, and do nothing if it's not found, since they wouldn't
have a place to insert their output.
</p>
<p>
This way you can avoid having a widget run on a page simply by omitting the element
it's looking for.
</p>
<p>
If more than one element matches the selector, the first element is used.
</p>

<h3>Widget configuration</h3>
<p>
Widget configuration is stored in the <code>[widgets]</code> table. The TOML syntax
for nested tables is <code>[table.subtable]</code>, therefore, you will have entries
like <code>[widgets.foo]</code>, <code>[widgets.bar]</code> and so on.
</p>
<p>
Widget subtable names are purely informational and have no effect, the widget type
is determined by the <code>widget</code> option. Therefore, if you want to use
a hypothetical <code>frobnicator</code> widget, your entry will look like:
</p>
<pre>
[widgets.frobnicate]
  widget = "frobnicator"
  selector "div#frob"
</pre>
<p>
It may seen confusing and redundant, but if subtable name defined the
widget to be called, you could only have one widget of the same type,
and would have to choose whether to include the header or the footer
with the <code>include</code> widget for example.
</p>

<h3>Limiting widgets to specific pages or sections</h3>
<p>
If the widget target comes from the page content rather than the template,
you can simply not include any elements matching its <code>selector</code>
option.
</p>
<p>
If it comes from the template, you can explicitly limit a widget to a certain section or page.
</p>
<pre>
[widgets.site-news]
  # only on site/index.html
  page = "index.html"

  widget = "include"
  file = "includes/site-news.html"
  selector = "div#news"

[widgets.cat-picture]
  # only on site/cats/*
  section = "cats"

  widget = "insert_html"
  html = "&lt;img src=\"/images/lolcat_cookie.gif\" /&gt;"
  selector = "#catpic"
</pre>

<h3>Widget processing order</h3>
<p>
If in your <code>soupault.conf</code> one config for widget A is before widget B,
it doesn't guarantee that widget A will be processed first. By default, soupault
assumed widgets are independent and can be processed in arbitrary order.
You can think that they are processed in parallel (even though they actually aren't
as of now).
</p>
<p>
This can be an issue if one widget relies on putput from another. In that case,
you can order widgets explicitly with the <code>after</code> parameter.
It can be a single widget (<code>after = "mywidget"</code) or a list of widgets
(<code>after = ["some-widget", "another-widget"]</code>).
</p>
<p>
Here is an example from this website's config. In the template there's
a <code>&lt;div id="breadcrumbs"&gt;</code> where breadcrumbs are inserted by the
<code>add-breadcrumbs</code> widget. Since there may not be breadcrumbs if the
page is not deep enough, the <code>div</code> may be left empty, and that's not
<em>neat</em>, so the <code>cleanup-breadcrumbs</code> widget removes it.
</p>
<pre>
## Breadcrumbs
[widgets.add-breadcrumbs]
  widget = "breadcrumbs"
  selector = "#breadcrumbs"
  # <omitted>

## Remove div#breadcrumbs if the breadcrumbs widget left it empty
[widgets.cleanup-breadcrumbs]
  widget = "delete_element"
  selector = "#breadcrumbs"
  only_if_empty = true

  # Important!
  after = "add-breadcrumbs"
</pre>

<h2>Built-in widgets</h2>
<h3>File and output inclusion widgets</h3>
<p>These widgets include something into your page: a file, a snippet, or putput of an external program.</p>
<h4 id="widgets-include">include</h4>
<p>
The <code>include</code> widget simply reads a file and inserts its content
into some element.
</code>
<p>
The following configuration will insert the content of <code>templates/header.html</code>
into an element with <code>id="header"</code> and the content of <code>templates/footer.html</code>
into an element with <code>id="footer"</code>.
</p>
<pre>
[widgets.header]
  widget = "include"
  file = "templates/header.html"
  selector = "#header"

[widgets.footer]
  widget = "include"
  file = "templates/footer.html"
  selector = "#footer"
</pre>
<p>
This widget provides a <code>parse</code> option that controls whether the file is parsed
or included as a text node. Use <code>parse = false</code> if you want to include a file verbatim,
with HTML special characters escaped.
</p>

<h4 id="widgets-insert_html">insert_html</h4>
<p>For a small HTML snippet, a separate file may be too much. The <code>insert_html</code> widget </p>
<pre>
[widgets.tracking-script]
  widget = "insert_html"
  html = "&lt;script src=\"/scripts/evil-analytics.js\"&gt; &lt;/script&gt;"
  selector = "head"
</pre>

<h4 id="widgets-exec">exec</h4>
<p>
The <code>exec</code> widget executes an external program and includes its output into
an element. The program is executed in shell, so you can write a complete command
with arguments in the <code>command</code> option. Like the <code>include</code>
widget, it has a <code>parse</code> option that includes the output verbatim
if set to <code>false</code>.
</p>
<p>Simple example: page generation timestamp.</p>
<pre>
[widgets.generated-on]
  widget = "exec"
  selector = "#generated-on"
  command = "date -R"
</pre>

<h5>Environment variables</h5>
<p>The following environment variables are passed to the external program:</p>
<dl>
  <dt><code>PAGE_FILE</code></dt>
  <dd>Path to the page source file, relative to the current working directory
      (e.g. <code>site/index.html</code></dd>
</dl>
<p>This is how you can include page's own source into a page, on a UNIX-like system:</p>
<pre>
[widgets.page-source]
  widget = "exec"
  selector = "#page-source"
  parse = false
  command = "cat $PAGE_FILE"
</pre>
<p>
If you store your pages in git, you can get a page timestamp from the git log
with a similar method (note that it's not a very fast operation for long commit
histories):
<pre>
[widgets.last-modified]
  widget = "exec"
  selector = "#git-timestamp"
  command = "git log -n 1 --pretty=format:%ad --date=format:%Y-%m-%d -- $PAGE_FILE"
</pre>
<p>
The <code>PAGE_FILE</code> variable can be used in many different ways,
for example, you can use it to fetch the page author and modification date
from a revision control system like git or mercurial.
</p>

<h3>Content widgets</h3>
<h4 id="widgets-title">title</h4>
<p>
The <code>title</code> widget sets the page title, that is, the <code>&lt;title&gt;</code>
from an element with a certain selector. If there is no <code>&lt;title&gt;</code> element
in the page, this widget assumes you don't want it and does nothing.
</p>
<p>Example:</p>
<pre>
[widgets.page-title]
  widget = "title"
  selector = "h1"
  default = "My Website"
  append = " on My Website"
  prepend = "Page named "
</pre>
<p>
If <code>selector</code> is not specified, it uses the first <code>&lt;h1&gt;</code>
as the title source element by default.
</p>
<p>
Optional <code>prepend</code> and <code>append</code> parameters allow you to insert some text
before and after the title.
</p>
<p>
If there is no element matching the <code>selector</code> in the page, it will use the
title specified in <code>default</code> option. In that case the <code>prepend</code>
and <code>append</code> options are ignored.
</p>
<p>
If the title source element is absent and default title is not set, the title is left empty.
</p>

<h4 id="widgets-footnotes">footnotes</h4>
<p>
The <code>footnotes</code> widgets finds all elements matching a selector,
moves them to another element, and replaces them with numbered links.
As usual, the container element can be anywhere in the page.
</p>
<pre>
[widgets.footnotes]
  # Required: Where to move the footnotes
  selector = "#footnotes"

  # Required: What elements to consider footnotes
  footnote_selector = ".footnote"

  # Optional: Element to wrap footnotes in, default is &lt;p&gt;
  footnote_template = "&lt;p&gt; &lt;/p&gt;"

  # Optional: Element to wrap the footnote number in, default is &lt;sup&gt;
  ref_template = "&lt;sup&gt; &lt;/sup&gt;"

  # Optional: Class for footnote links, default is none
  footnote_link_class = "footnote"
</pre>
<p>Suppose you have this page:</p>
<pre>
&lt;div id=&quot;content&quot;&gt;
  &lt;p&gt;The footnotes widget is easy to use.&lt;span class=&quot;footnote&quot;&gt;According to its author.&lt;/span&gt;&lt;/p&gt;
&lt;/div&gt;
&lt;div id=&quot;content&quot;&gt;&lt;/div&gt;
</pre>
<p>Using configuration from this example, this is what the footnotes widet will make from it:</p>
<pre>
&lt;div id=&quot;content&quot;&gt;
  &lt;p&gt;The footnotes widget is easy to use.&lt;a href=&quot;#footnote-1&quot; class=&quot;footnote&quot;&gt;&lt;sup&gt;1&lt;/sup&gt;&lt;/a&gt;&lt;/p&gt;
&lt;/div&gt;
&lt;div id=&quot;content&quot;&gt;
  &lt;p id=&quot;footnote-1&quot;&gt;According to its author.&lt;/p&gt;
&lt;/div&gt;
</pre>

<h4 id="widgets-toc">toc</h4>
<p>The <code>toc</code> widget generates a table of contents for your page.</p>
<p>
Table of contents is generated from the heading tags from <code>&lt;h1&gt;</code>
to <code>&lt;h6&gt;</code>. 
</p>
<p>Here is the ToC configuration from this website:</p>
<pre>
[widgets.table-of-contents]
  widget = "toc"

  # Required: where to insert the ToC
  selector = "#generated-toc"

  # Optional: minimum and maximum levels, defaults are 1 and 6 respectively
  min_level = 2
  max_level = 6

  # Optional: use &lt;ol&gt; instead of &lt;ul&gt; for ToC lists
  # Default is false
  numbered_list = false

  # Optional: Class for the ToC list element, default is none
  toc_list_class = "toc"

  # Optional: append the heading level to the ToC list class
  # In this example list for level 2 would be "toc-2"
  toc_class_levels = false

  # Optional: Insert "link to this section" links next to headings
  heading_links = true

  # Optional: text for the section links
  # Default is "#"
  heading_link_text = "→ " 

  # Optional: class for the section links
  # Default is none
  heading_link_class = "here"

  # Optional: insert the section link after the header text rather than before
  # Default is false
  heading_links_append = false

  # Optional: use header text slugs for anchors
  # Default is false
  use_header_slug = true

  # Optional: use unchanged header text for anchors
  # Default is false
  use_header_text = false
</pre>
<h5>Choosing the heading anchor options</h5>
<p>
By default, if a heading has no <code>id</code>, soupault will generate
a unique numeric identifier for each heading. This is safe, but not very
good for readers (links are non-indicative) and people who want to share
direct links to sections (they will change if page structure changes).
</p>
<p>
If a heading has an <code>id</code> attribute, it will be used for the anchor.
It's fine to mix this with any other option: anchors for the remaining headings with no <code>id</code>
will be generated automatically.
</p>
<p>
If you want to find a balance between readability, permanence, and ease of maintenance
there are a few ways to do that and the choice is yours.
</p>
<p>
The <code>use_header_slug = true</code> option converts the heading text
to a valid HTML identifier. Right now, however, it's very aggressive
and replaces everything other than ASCII letters and digits with hyphens.
This is obviously a no go for non-ASCII languages, that is, pretty much
all languages in the world. It may be implemented more sensibly in the future.
</p>
<p>
The <code>use_header_text</code> option uses unmodified header text
for anchors, with whitespace and all. This is against the rules of HTML,
but seems to work well in practice.
</p>
<p>
Note that <code>use_header_slug</code> and <code>use_header_text</code>
do not enforce uniqueness.
</p>
<p>
All in all, for best link permanence you should give every heading a unique id
by hand, and for best readability you may want to do with <code>use_header_text = true</code>.
</p>

<h4 id="widgets-breadcrumbs">breadcrumbs</h4>
<p>The <code>breadcrumbs</code> widget generates breadcrumbs for the page.</p>
<p>The only required parameter is <code>selector</code>, the rest is optional.</p>
<pre>
[widgets.breadcrumbs]
  widget = "breadcrumbs"
  selector = "#breadcrumbs"
  prepend = ".. / "
  append = " /"
  between = " / "
  breadcrumb_template = "<a class=\"nav\"></a>"
  min_depth = 1
</pre>
<p>
The <code>breadcrumb_template</code> is an HTML snippet that can be used
for styling your breadcrumbs.
It <strong>must</strong> have an <code>&lt;a&gt;</code> element in it.
By default, a simple unstyled link is used.
</p>
<p>
The <code>min_depth</code> sets the minimum nesting depth where breadcrumbs
appear. That's the length of the logical navigation path rather than directory path.
</p>
<p>
There is a fixup that decrements the path for section index pages, that is, pages named<code>index</code>
by default, or whatever is specified in the <code>index_page</code> option.
Their navigation path is considered one level shorter than any other page in the section,
when clean URLs are used. This is to prevent section index pages from having links
to themselves.
</p>
<ul>
  <li><code>site/index.html</code> → 0</li>
  <li><code>site/foo/index.html</code> → 0 (sic!)</li>
  <li><code>site/foo/bar.html</code> → 1</li>
</ul>

<h3>HTML manipulation widgets</h3>

<h4>delete_element</h4>

<p>The opposite of <code>insert_html</code>. Deletes an element with given selector. It can be useful in two situations:</p>
<ul>
  <li>Another widget may leave an element empty and you want to clean it up.</li>
  <li>Your pages are generated with another tool and it inserts something you don't want.</li>
</ul>
<pre>
# Who reads footers anyway?
[widgets.delete_footer]
  widget = "delete_element"
  selector = "#footer"
</pre>
<p>
You can limit it to deleting only empty elements with <code>only_if_empty = true</code> option.
Element is considered empty if there's nothing but whitespace inside it.
</p>

<h2>Future plans</h2>
<ul>
  <li>Per-section and per-page template overrides</li>
  <li>Lists as page/section options</li>
  <li>Table of contents widget</li>
  <li>Footnotes widget</li>
  <li>Forced widget processing</li>
</ul>

<h3>Extensibility</h3>
<p>
Right now the only way to extend soupault is to make it run external programs.
You can do quite a lot with it, especially when the set of passed environment
variables is extended.
</p>
<p>Possible future methods may be:</p>
<ul>
  <li>Dynlinking plugins. Full access to internal datastructures, but plugins are not easy to distribute (need to be compiled).</p>
  <li>Embedded Scheme/Lua/a DSL. Heavyweight option, but plugins are easy to distribute.</li>
</ul>

<div id="last-modified">This page was last modified: <span id="git-timestamp"> </span> </div>
