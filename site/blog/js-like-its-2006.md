# Bundle NPM modules into static JS libraries like it's 2006

<time id="last-modified">2020-08-19</time>
<tags>js, web, programming</tags>

<p id="summary">
A good thing about browser implementations of JavaScript is compatibility. The NPM ecosystem, however, is infamous for its fragility.
As a professional &ldquo;not a frontend developer&rdquo; I try to opt out of it as much as possible.
Luckily, if you just need a library from NPM, it's easy to package it into an &ldquo;eternal&rdquo; blob that will work forever.
In this post I'll share my procedure for creating such static JS blobs from NPM modules.
</p>

Classic JavaScript libraries usually come in a single file package and &ldquo;export&rdquo; their functions by extending the `window` object.
This approach is foolproof as has no moving parts, as long as libraries don't use conflicting names.
My own website had been using a [ToC library from 2007](https://www.kryogenix.org/code/browser/generated-toc/) for a while. A snowflake script
from 1999 is just as likely to work. jQuery [1.0.0](https://code.jquery.com/jquery-1.0.js) from 2006 is still usable 15 years later.

The problem with using NPM modules in browsers is that they are written in a JS variant that has something of a module system.
JavaScript of the browsers doesn't know anything about modules, so developers bundle their applications with all required libraries
and ship it as a single `.js` file.

It may be fine for applications under constant development, but it's bad for pages [designed to last](https://jeffhuang.com/designed_to_last/).
If you have a script that happens to use jQuery 1.0.0, you can independently a) edit the script b) edit the page that uses it c) upgrade jQuery.
You will not lose your ability to do any of those things in the future. NPM-based workflows have more potential to become lost technology
if you are not doing it all the time and not adjusting for library and tooling changes.

For an upcoming project, I need to manipulate TOML in browser. To my surprise, I couldn't find a library that would do both parsing and printing. 
So, I decided to assemble a jQuery-style library for that, from two packages: [toml](https://www.npmjs.com/package/toml) for parsing,
and [json2toml](https://www.npmjs.com/package/json2toml) for printing.

The key idea is that while we cannot use `require()` in browser, nothing prevents us from adding values to the `window` object _from within_
a node.js-style module. Then we can use a bundling tool to produce a browser-friendly blob from it.

First we'll install the required libraries: `npm install toml json2toml`.

Now we'll create an entry point file, say `toml_browser.js`. It will load the libraries, create a `window.toml` object, and add
public functions to its fields.

```js
var toml = require('toml');
var json2toml = require('json2toml');

window.toml = Object();
window.toml.parse = toml.parse;
window.toml.print = json2toml;
```

Now it's time to bundle it all together. There are a few tools for that, but I stick with [browserify](http://browserify.org/).
They also provide a plugin for JS minification, named &ldquo;tinyify&rdquo;.

Let's install them: `npm install browserify tinyify`. If you use `npm -g` for a global install, then `browserify` command will become available
However, you can as well run it directly from the module dir.

```
$ ./node_modules/browserify/bin/cmd.js -o toml.js -p tinyify ./toml_browser.js
```

Now the file is ready to use. Everything you add to the `window` object becomes visible in the main namespace,
so once the script is loaded, you can call `toml.parse()` and `toml.print()`.

```html
<html>
  <head>
    <script src="/toml.js"> </script>
    <script>
      console.log(toml.parse("[foo]\n bar=42"))
    </script>
  </head>
</html>
```

I don't know if this procedure is the best one, but it successfully helps me stay a JavaScript luddite and I'm happy with it.
