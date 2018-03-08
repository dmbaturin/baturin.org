{% set date 2018 March 02 %}

<p>
This is my home page. I know home pages is not a popular genre these days and it probably makes me
look old fashioned.
</p>

<h2>Whose home page?</h2>

<p>
My name is <strong>Daniil Baturin</strong>. On the Internet I usually go by <strong>dmbaturin</strong>,
the &ldquo;m&rdquo; is from my middle name, <a href="https://en.wikipedia.org/wiki/Patronymic">of sorts</a>.
The good thing about it is that it's easy to write in ASCII. The bad thing is that just about everyone asks how they should
pronounce it and how many i's are there. I don't really care, you can call me Dan and avoid the issue altogether.
<br/>
If you really insist, there are two i's and it's pronounced [Danʲiʔil Baturʲin] (Dah-nee-ee-l Bah-too-ree-n, , with a glottal stop between the i's like in &ldquo;uh-oh&rdquo;.
</p>

<p>
I write programs and build networks. Sometimes those programs have something to do with networks,
sometimes they don't.
</p>

<p>You may be interested in my <a href="/projects">projects</a> or the
   <a href="/docs">documentation</a> I wrote. At least that's what I consider most important.</p>
<p>
The ones that are used widely enough that you might have already stumbled upon them are
<a href="http://www.vyos.io">VyOS</a> (a Linux distribution for routers), <a href="/tools/encapcalc">encapcalc</a>
the MTU/MSS calculator, and the <a href="/docs/iproute2">task-centered iproute2 user guide</a>
(formerly &ldquo;iproute2 cheatsheet&rdquo;).
</p>

<p>You can also find me in other places:</p>
<ul>
  <li> Github: <a href="http://github.com/dmbaturin">github.com/dmbaturin</a> </li>
  <li> Blog: <a href="http://blog.baturin.org">blog.baturin.org</a> </li>
  <li> Twitter: <a href="http://twitter.com/dmbaturin">@dmbaturin</a> </li>
  <li> dmbaturin on <a href="http://freenode.net">Freenode</a> and <a href="http://esper.net">Esper</a>
       IRC networks.</li>
</ul>

<p>You can contact me by email &lt;daniil (at) baturin (dot) org&gt;
(use <a href="http://pool.sks-keyservers.net:11371/pks/lookup?op=vindex&amp;search=0x4726DF9511E0AF1F">PGP key</a>
if you wish).</p>

<h2>Privacy notice</h2

<p>
I use self-hosted <a href="http://piwik.org">Piwik</a> for statistics to satisfy my
curiosity regarding the visitors. It doesn't store complete IP addresses and respects the Do Not Track header,
so if you have a problem with it, just enable DNT and you should be fine.</p>

<p>This website does not use cookies (should I have made this a popup message?).</p>

<h2>Copyrights</h2>

<p>
Everything you see here is distributed under the terms of the
<a href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution</a> license, unless otherwise specified.
</p>

<h2>News</h2>
<p>
The usual problem with news sections on static websites is that no one ever updates them. For this reason I made a dirty hack
for generating them from the git commit log.
</p>
<dl>
{% cmd git log -n 10 --date=short --pretty="format:<dt>%ad</dt> <dd>%s</dd>" | perl -ne '$_=~s/\[(.*)\]/<a href="$1">$1<\/a>/;print' %}
</dl>
