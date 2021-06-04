# Why not Hugo

<time id="last-modified">2021-06-04</time>
<tags>web</tags>

<p id="summary">
People sometimes ask me why I wrote my own <a href="https://soupault.app">static site generator</a> instead of using an existing one.
A lof of time their suggestion is Hugo. Well, a lot of the time I use Hugo as an example of what <em>not to do</em>.
It suffers from creeping compatibility breakage and I find it highly limiting in many areas. I'm not planning to use it and not recommend it to anyone else.
For the details, read on.
</p>

## Format support

The Hugo website says that "with its amazing speed and flexibility, it makes building websites fun again".

The interesting part is that depending on the content formats you want to use, you may have to choose between speed and flexibility.

If you want to use reStructuredText or AsciiDoc, that is possible, but it [requires external helpers](https://web.archive.org/web/20210202024351/https://gohugo.io/content-management/formats/).
That's an obvious speed/flexibility tradeoff already. It can be a wortwhile tradeoff is there's enough flexibility. Is there?

Not quite. First, the set of available external helpers is hardcoded and you cannot add your own without modifying Hugo itself.
Second, Hugo doesn't let you pass custom arguments to the helpers. For AsciiDoctor you get a
[bunch of options](https://web.archive.org/web/20210202024351/https://gohugo.io/content-management/formats/#external-helper-asciidoctor)
that Hugo will [translate](https://github.com/gohugoio/hugo/blob/ee733085b7f5d3f2aef1667901ab6ecb8041d699/markup/asciidocext/convert.go#L162-L166) to `asciidoctor` arguments for you.

For RST, you get hardcoded [`--leave-comments --initial-header-level=2`](https://github.com/gohugoio/hugo/blob/ee733085b7f5d3f2aef1667901ab6ecb8041d699/markup/rst/convert.go#L82) and that's it.

You can also use [Pandoc](https://pandoc.org) as a convertor. Pandoc is famous for its flexibility. However, Hugo will just pass `--mathjax` to it,
and there's [no way to pass any other options](https://github.com/gohugoio/hugo/blob/ee733085b7f5d3f2aef1667901ab6ecb8041d699/markup/pandoc/convert.go#L63-L64).

### What about soupault?

It's possible to use any external executable as a [page preprocessor](https://soupault.app/reference-manual/#page-preprocessors) and pass any options to it.
You can take your tools with you if you migrate from something else.

## Markdown support

There are people who claim that Markdown is the only format you will ever need. Which Markdown though?

Hugo bundles _three_ different Markdown libraries: Goldmark, BlackFriday, and MMark. MMark is deprecated and BlackFriday "will eventually be deprecated".

Why bundle different libraries? Because they have [different features and behaviour](https://web.archive.org/web/20210206131957/https://gohugo.io/getting-started/configuration-markup/).
For example, Goldmark is CommonMark compliant, while BlackFriday isn't.
End users and theme authors may have to take that into account, and sometimes [stick with an old default](https://themes.gohugo.io/hugo-octopress/#goldmark-vs-blackfriday)
to enable some features.

### What about soupault?

As already shown in previous section, there's no such problem. You are free to use any Markdown conversion tool of your choise and pass any options to it.

## Feature inconsistenties

This all leads to the next point: feature inconsistency. Let's examine table of contents generation, for example.
Table of contents is an opaque feature of the convertor. Exact way it works will change when you switch Markdown backends,
and you cannot have consistent ToC between different source formats. Same with footnotes, cross-references etc.

Another example is syntax highlighting. Built-in Markdown files will use Chroma, other files will use the highlighting mechanism
of the external helper, if there's any. There's no way to keep it consistent.

### What about soupault?

Soupault works on the HTML level. Even when external helpers are involved, it first runs the page source through the helper, then parses its
HTML output and runs widgets and plugins on it. That way ToC and footnotes can work and look exactly the same for all pages, no matter the source format.

## Creeping incompatibility and theme breakage

My personal pet peeve is projects that stay at 0.x for years to avoid having to maintain a stable API and answer to their users when it breaks (despite bragging about the size of their community
measured via [proxy metrics](https://gohugo.io/news/0.61.0-relnotes/)<fn id="40k-stars">Exact words from that release announcement: &ldquo;40K stars on GitHub is a good enough reason to release a new version of Hugo!&rdquo;</fn>).

It's especially annoying for end users when they don't even use the API in question _directly_.
A major selling point of CMSes and CMS-like static site generators is "themes". The idea is that you can just install a different theme
if you want a different website look. In practice it's not always that simple because a theme isn't just a "HTML with placeholders".
Many themes have a lot of logic in their templates and come with a lot of assumptions, so switching from one theme to another isn't trivial.
That can be a sensible tradeoff by itself.

I want to raise a different point. The problem with Hugo specifically is that it breaks compatibility with old themes all the time.
For example, let's look at [hugo-icarus](https://themes.gohugo.io/hugo-icarus/). This is what its maintainer says:
"The original Hugo Icarus theme has been unmaintained for years, and as Hugo upgraded, some features broke (such as front-page post listing, recent posts, and post counts). This version fixes such issues."

It's far from an isolated incident. A quick look at the issues reveals multiple entries along the same lines:
[#867](https://github.com/gohugoio/hugoThemes/issues/867), [#810](https://github.com/gohugoio/hugoThemes/issues/810), [#669](https://github.com/gohugoio/hugoThemes/issues/669).
In all such cases their approach is to notify authors and remove themes from the catalog if they aren't fixed.

If a theme is abandoned by its original authors or if you are using a custom theme, your only choice is to dive into its internals and fix it yourself.
Even post that recommend Hugo sometimes suggest to [choose your theme wisely](https://masalmon.eu/2020/02/29/hugo-maintenance/).

### What about soupault?

Soupault doesn't have themes in the same sense. You _configure_ it to work with your HTML and specify where to insert the page content,
what data to extract and put in the site index etc., all using CSS3 selectors. It's comparable to the old idea of [microformats](https://microformats.org),
except you can make up your own microformats as you go. Thus a combination of an HTML layout and a soupault config will not break
from upgrading soupault, unless soupault config format changes.

When config format _does_ change, soupault considers it a breaking change and a reason to bump the major version and to go through a proper.
It happened exactly once so far, and a convertor to the new option format was provided to help users migrate (even though the change didn't affect all users).

## It actually parses HTML internally

When I was doing static site generator "market research", that was what convinced me that I should write soupault. In many cases,
people do resort to parsing HTML to get things working the way they like and plug an HTML parser into their SSG.

Funnily enough, Hugo isn't consistent about that. To inject a generator meta tag, it [parses HTML with regular expressions](https://github.com/gohugoio/hugo/blob/ee733085b7f5d3f2aef1667901ab6ecb8041d699/transform/metainject/hugogenerator.go#L27).<fn id="zalgo"><a href="https://stackoverflow.com/questions/1732348/regex-match-open-tags-except-xhtml-self-contained-tags/1732454#1732454">It's too late, too late, we cannot be saved!</a></fn>
At the same time, it bundles an HTML parser and uses it in the AsciiDoc helper to [yank the ToC generated by `asciidoctor` out of the document](https://github.com/gohugoio/hugo/blob/ee733085b7f5d3f2aef1667901ab6ecb8041d699/markup/asciidocext/convert.go#L220-L225)
and provide some semblance of consistency with Markdown file handling and make the ToC available in a variable.<fn id="toc-datastructure">Built-in Markdown backends also return an opaque HTML ToC, not a ToC datastructure.</fn>

There's no way you can use that HTML parser in your own ways though.

### What about soupault?

Soupault provides an HTML manipulation [API](https://soupault.app/reference-manual/#HTML) for its Lua plugins, similar to browsers. You are free to use all its features yourself.
It also provides a [ToC datastructure](https://soupault.app/reference-manual/#HTML.get_headings_tree) if you want to write a custom ToC renderer.
All built-in widgets could be reimplemented as Lua plugins, in fact some of them started their life as plugins and than ascended to built-ins.

## Conclusion

For me, the list above is enough of a reason to never touch Hugo and never recommend it to anyone else.
That said, soupault may not suite your site's needs and isn't a perfect solution for everyone either.

<hr>
<div id="footnotes"> </div>
