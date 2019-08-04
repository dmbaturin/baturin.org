<h1>Static Site Generators</h1>

<div id="generated-toc"> </div>

<p>
Static website generators were always obvious to me, but at some point
I realized that it's only because I hang out in circles where they are
commonly used. Lately I've realized that a number of people even in the web development field
are still unaware of them, or were told wrong things about them.
</p>
<p>
To make things worse, most website generator websites
(including <a href="/projects/soupault">my own</a>) are written as if
the reader already knows what's going on and only wants to know
if they want to use that exact generator or not.
</p>
<p>
Here I'm saying the things that usually get omitted.
</p>

<h2>Brief history of web architectures</h2>
<p>
I start from the beginning of times, but bear with me.
</p>
<p>
In the beginning, a website was just a bunch of HTML files
in a directory. Every page was handcrafted. It's simple
and has very low performance requirements, but it's obvious that
managing any site with more than a few pages that way is a massive headache.
</p>

<img src="/images/ssg/static_site.png" alt="" />

<p>
That is why people got an idea to separate the content from presentation
and generate pages dynamically. Page content would be stored in a database.
When a request comes in, a content management system or another web application
extracts page content from the database, assembles a page from it using
a template/theme, and serves it to the user.
</p>

<img src="/images/ssg/cms.png" alt="" />

<p>
This approach makes it easy to manage large sites. If you are changing the visual design,
you just change the templates. It also enables interaction with visitors,
which gave rise to Web 2.0.
</p>
<p>
However, performance requirements are much higher and servers become sensitive
to traffic surges. This gave rise to the infamous &ldquo;slashdot effect&rdquo;
when a sudden popularity spike makes a website completely inaccessible because
the server can't handle the load, and adding server resources in a timely manner is not always trivial.
</p>
<p>
Something had to be done about it. The key observation is that even on websites with interactive features,
most of the content stays unchanged most of the time. The CMS just wastes CPU cycles
rendering the same content for every request.
</p>
<p>
That's why people created caching proxies and content delivery networks that serve end users
a static copy of the page unless its content has changed.
</p>

<img src="/images/ssg/cdn.png" alt="" />

<p>
Now, the last step to static website generators. Notice that over time, the caching server
builds a complete static copy of your website. If you don't have interactive features like
comments or you outsource them to external services, the CMS is now a dead weight:
it does nothing useful at that point, but it still listens to requests, exposing potential security
vulnerabilities.
</p>

<h2>Enter static website generators</h2>
<p>
Instead of having a CMS generate pages on demand, you can use a program that generates
all your pages at once, and then you can serve it as a static website.
</p>
<p>
This way you still free from having to write every page by hand, but you are also
free from having to keep the CMS running and up to date. 
</p>

<img src="/images/ssg/ssg.png" alt="" />

<h2>What can static generators do</h2>
<p>
Everything that a CMS can do, except interaction with visitors.
</p>
<p>These things are possible and there are generators that support them:</p>
<ul>
  <li>Customizeable content models and taxonomies.</li>
  <li>Asset management and collections (galleries etc.)</li>
  <li>Multilingual websites.</li>
  <li>Graphical frontends for website management.</li>
</ul>
<p>
The hardest part is to choose which one to use because there are so many of them.
I've just made my own.
</p>

<div id="last-modified">This page was last modified: <time id="git-timestamp"> </time> </div>
