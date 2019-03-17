{{% set date 2019 March 16 %}}

<h1>Old webmaster's guide to mobile-friendly websites</h1>

<div id="generated-toc"></div>

<p>
Suppose you have a website that was designed in a past era,
or with a mindset from a past era. Or it's just made to look as if it was
made in a past era for aesthetic reasons.
You do it because you think information you provide is useful, or because you want
to have a place to share things without relying on cloud services that sell your data,
or you just miss the time when everyone was making homepages.
</p>
<p>
Then someone comes to you and says: <strong>your website looks bad on my phone!</strong>
You ask them if they tried to use a computer, but it turns out they think phones
are the new computers.
</p>
<p>
It becomes known. Your collegues laugh at you. A popular search engine excludes your website from search results
due to its &ldquo;presentation over usefulness&rdquo; policy.
Your parliament starts discussing fines for authors of mobile-unfriendly websites.
You become known as the anti-mobile person in your town and a local newpaper picks up the story.
Convicted felons start reading it to feel better about themselves.
</p>
<p>
You start reading about mobile-friendly websites and see articles by professional
webmasters telling you how to make a hamburger menu and degrade the UI to the level
of a device with 640x480px output and 1x1 inch input resolution.
</p>
<p>
You take inspiration from Wikipedia and other websites that have a normal version for real computers
and a special version for limited devices.
You think making a mobile version is easy, since there's even a special CSS media query for it
(<code>@media handheld</code>), but then you discover that all devices other than old cellphones and PDAs ignore it.
</p>
<p>
But not all is lost! As a professional non-webmaster, I have a unique expertise that I can share with you
to help your website survive in the hostile world filled with phones mistaken for computers.
I'll focus on techniques that do not make your website worse for desktop users.
</p>

<h2>General considerations</h2>

<p>
All advice below assumes you already define the layout in CSS and got rid of table-based layouts.
If not, you should do it first. Separation of the logical and physical layout is good for you since
you can change the website look by editing just the CSS, and it also makes websites more friendly
to people with special needs, e.g. screen reader users.
</p>

<h2>The viewport settings</h2>

<p>
This is said everywhere, but I'll add it for the sake of completeness, and because I cannot refrain from ranting about it.
The first thing to ensure your website will render properly on mobile devices is to set the <em>viewport</em> meta tag.
The <a href="https://drafts.csswg.org/css2/visuren.html#x1">viewport</a> is the screen area where
a web page is displayed. If you don't set it, your website will look unusably small.
You can read the details in <a href="https://developer.mozilla.org/en-US/docs/Mozilla/Mobile/Viewport_meta_tag">MDN</a>.
</p>
<p>
The default, widespread, world standard viewport meta tag settings that mean &ldquo;do what a sensible
browser would be expected to do by default&rdquo; are:
</p>
<pre>
&lt;meta name="viewport" content="width=device-width, initial-scale=1.0"&gt;
</pre>

<p>
To me, it seems absurd that not even pure markup, <a href="http://contemporary-home-computing.org/prof-dr-style/">Prof. Dr.</a>
style websites render in a readable way on mobile devices withut it. I see no valid reason to set the initial to anything else
than the device width on systems that have no resizable windows, and I haven't seen anyone do it. Moreover, the additional
options such as <code>maximum-scale</code> and <code>user-scalable</code> that prevent users from zooming in are outright accessibility nightmares.
But that's the way it is. We've got what we've got.
</p>

<h2>Fitting elements to the screen width</h2>

<p>
Some elements can end up wider than screen, or wider than their parent element, which causes an annoying horizontal
scroll. This problem is not unique to mobile browsers, it's just more apparent on narrow screens, so I think it's
an improvement for all users.
</p>
<p>
Images, and, in some browsers, tables, are common examples of such elements. Setting <code>max-width: 100%</code>
will limit their width to the parent element. For images, you also need to set <code>display: inline-block</code>.
Other elements may also be suspectible, for example form inputs, HTML5 video, and object/embed tags.
</p>

<pre>
table {
    max-width: 100%;
}

img {
    display: inline-block;
    max-width: 100%;
}
</pre>

<p>
Preformatted text can also end up wider than screen, but the solution is different and it's a bit of a compromise.
Since text cannot be automatically scaled down, the only solution is to allow the lines to wrap at whitespace characters
but preserve whitespace otherwise, using the <code>white-space: pre-wrap</code> property.
</p>
<p>
For code in whitespace-sensitive languages such as Haskell and Python this can be a visual nuisance, but copying and pasting
line-wrapped text works as expected, so the problem is purely visual at least.
</p>
<pre>
pre, code, sample {
    white-space: pre-wrap;
    hyphens: none;
}
</pre>

<h2>Detecting mobile devices</h2>

<p>
For some websites, switching to a &ldquo;responsive web design&rdquo; is simply not an option.
If complex positioning of elements is a part of your artistic vision and you are not going to sacrifice it,
best you can do is to give a completely different layout to mobile device users so that they can get
the raw information from the website at least.
</p>
<p>
In a nutshell, there's no easy way to do it without server side checks or complex JS, but there's hope.
First, a word of warning:
</p>

<h3>Don't check the screen size</h3>
<p>
There are many pages around that offer ready-made CSS media queries for detecting mobile devices by their screen size.
It's better to ignore them because by now mobile devices screen	size in pixels often equals or     
exceeds	the size of common desktop screens, and even the ratio may be the same,	so this	criterion
is useless for detecting if it's a mobile device or not. It's only useful for defining breakpoints for switching
layouts between wide and narrow screens.
</p>
<p>
So, what can you use instead?
</p>

<h3>Check interaction media features</h3>
<p>
While the <code>handheld</code> media type is not supported by anyone, there's still hope for detecting mobile devices with CSS alone.
</p>
<p>
The mantra of responsive design has always been to test for capabilities, not specific devices. The defining characteristic of modern mobile devices is lack
of a pointing device other than a touchscreen, which is their biggest limitation due to low input resolution of capacitive touchscreens.
The irony is that there was no media query for a high precision pointing device capability.
Now there is, it's called &ldquo;<a href="https://developer.mozilla.org/en-US/docs/Web/CSS/Media_Queries/Using_media_queries#Media_features">media features</a>&rdquo;
and provides two queries with slightly different semantics: <code>pointer</code> and <code>any-pointer</code>. Possible values are <code>coarse</code> and <code>fine</code>.
The difference is that the <code>pointer</code> checks capabilities of the <em>primary</em> pointing device (whatever it means) while
<code>any-pointer</code> checks if <em>any</em> device with given capability is available.
</p>

<pre>
@media (pointer: fine) {
  /* Desktop browser layout rules */
}

@media (pointer: coarse) {
  /* Mobile device layout rules */
}
</pre>

<p>
If you want to make it work in browsers that don't support that media query, you may take advantage of the fact that CSS rules are processed top to bottom
and the last ones take effect, for example:
</p>

<pre>
/* Red background in all desktop browsers */
body { background-color: red; }

/* Blue background in all mobile browsers that support the pointer query */
@media (pointer: coarse) {
  body { background-color: blue; }
}
</pre>

<p>
Browser support for it only got <a href="https://caniuse.com/#search=any-pointer">close to universal</a> in the late 2018,
but some mobile browsers still don't support it so it depends on the audience you want to serve.
</p>
<p>
Behaviour of those queries on devices that have both a proper pointing device and a touchscreen is an interesting question,
and I'm not in position to test it personally.
Presumably, <code>any-pointer: coarse</code> should return true in that case, which is problematic if you are using the second
approach to support old desktop browsers. If old desktop browser support is not an issue, <code>any-pointer: fine</code>
ought to work fine on all devices with a high presicion pointing device whether they also have a touchscreen or not.
</p>

<h2>Layout</h2>

<p>
This is the tricky part. The widely adopted solution is so-called &ldquo;fluid design&rdquo;, where layout changes depending
on screen width. There are two approaches: either using media queries for screen width to define &ldquo;breakpoints&rdquo;
where the layout changes, or use the CSS <a href="https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Flexible_Box_Layout">flexbox</a>
or <a href="https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Grid_Layout">grid</a> layouts to allow elements to automatically
re-flow.
</p>
<p>
Re-flowing without fixed breakpoints is the cleanest solution imposes visual design limitations, since some kids of decorations
like borders and per-element backgrounds can look ugly if what supposed to be a narrow column expands to the full screen width.
Depending on your visual design preferences, you may be able to get away with it. Otherwise you will need to decide how it should
look on smal screens and what screen size will be considered small.
</p>
<p>
If your layout is purely vertical and doesn't involve columns at all, you may only need minimal modifications to make it work
on mobile.
</p>

<h3>Get rid of width values in absolute units</h3>

<p>
It's common to see something like <code>width: 1000px</code> for the main block, or sometimes width of every block defined
in pixels. In the time when the world standardized on 1024x768 for a while, it might have been justified, but even then it
would be problematic for people stuck with 800x600 displays.
</p>
<p>
Whenever you want to limit the width of an element, you should use the <code>max-width</code> property instead,
and the physical layout should be defined in relative units (e.g. percents).
</p>

<h3>Screen orientation</h3>

<p>
Using a completely different style for mobile devices is the most radical option, but you may also try to make it better
for people who have it worst: those using smartphones in portrait mode.
</p>

<pre>
/* Rules for desktop and mobile in landscape mode */

@media (orientation: portrait) {
  /* Rules for portrait mode */
}
</pre>

<h2>Conclusion</h2>

<p>
I hope the suggestions I collected will be useful for making old websites accessible to the mobile audience
without hurting desktop users. If you have more suggestions, please share them with me.
</p>
