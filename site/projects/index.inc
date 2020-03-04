<h1>Projects</h1>

<div id="generated-toc"></div>

<h2>Standalone software</h2>

<h3>VyOS</h3>

<p>A GNU/Linux distribution for routers/firewalls/VPN concentrators.</p>
<p>Unlike GUI-oriented projects like pfSense or IPcop, we belive in the
power of the command line interface, so VyOS provides access to all
system functions through a unified CLI somewhat similar to that of JunOS.</p>
<p>It was forked from the abandoned Vyatta Core project in 2013.</p>
<ul>
  <li> Website: <a href="https://www.vyos.io">vyos.io</a> </li>
  <li> Source code: it's a bit complicated, in a nutshell,
       <a href="http://github.com/vyos/vyos-build/">github.com/vyos/vyos-build</a>
       and <a href="http://github.com/vyos/vyos-1x/">github.com/vyos/vyos-1x</a>
       is where active development is going on.</li>
  <li>Distrowatch: <a href="http://distrowatch.com/table.php?distribution=vyos">distrowatch.com/table.php?distribution=vyos</a> </li>
</ul>

<h3>soupault</h3>
<p>
A static website generator based on HTML rewriting. Think DOM manipulation, but without a browser.
Instead of inserting template processor directives inside pages, you write a set of HTML manipulation
rules using CSS selectors (e.g. &ldquo;insert <code>includes/menu.html</code> in the element that matches <code>div#menu</code>&rdquo;).
Extensible with Lua plugins and external helper programs.
Perfect for Web 1.0 enthusiasts since you can make all pages look different and still have an automated workflow.
</p>
<ul>
  <li>Website: <a href="https://soupault.neocities.org">soupault.neocities.org</a></li>
  <li>Git: <a href="https://github.com/dmbaturin/soupault">github.com/dmbaturin/soupault</a></li>
</ul>
<p>
If you are wondering why I put it on <a href="https://neocities.org">NeoCities</a> if I have my own server,
it's to bring soupault closer to the community that can benefit from it most, and to support an independent
and open source hosting provider.
</p>

<h3>ipaddrcheck</h3>
<p>An ipcalc for use in scripts. Allows you to do things like <tt>ipaddrcheck --is-ipv4-net 192.0.2.0/25</tt>
   to find out if the argument is a network address rather than a host address and much more.</p>
<p>Primary motivation is to allow fine-grained user input checking in configuration utilities
   and startup scripts.</p>
<p>Requires <a href="http://www.over-yonder.net/~fullermd/projects/libcidr">libcidr</a> by Matthew D. Fuller.</p>
<ul>
  <li>Source code: <a href="https://github.com/vyos/ipaddrcheck">github.com/vyos/ipaddrcheck</a> </li>
</ul>

<h3>hvinfo</h3>
<p>Yet another hypervisor detection utility, this time cross-platform (Linux and FreeBSD) and written in a
   high level compiled language (Ada 2005) rather than a mix of C and shell.</p>
<ul>
  <li>Source code: <a href="https://github.com/dmbaturin/hvinfo">github.com/dmbaturin/hvinfo</a> </li>
</ul>

<h2>Libraries and plugins</h2>

<h3>ocaml-tsort</h3>
<p>
A topological sort library that isn't annoying to use and gives clear errors about missing
dependencies and dependency cycles. See <a href="/code/ocaml-tsort">/code/ocaml-tsort</a>
for discussion.
</p>
<p>
OPAM page: <a href="https://opam.ocaml.org/packages/tsort/">opam.ocaml.org/packages/tsort</a>.
</p>

<h3>pytaf</h3>
<p>Aviation weather forecast parser and decoder. TAF stands for Terminal Aerodome Forecast.</p>
<p>The format is not very well specified and machine friendly, so the library heavily relies on nasty
   backtracking. The other problem is that the format is not exactly the same in all
   countries, but there is no indication which flavour is it in the reports.</p>
<p>Even though the library works pretty well, there is no guarantee that your report will be decoded
   correctly, so if you need it for anything serious, learn to read it yourself, it's easier than
   it looks at first.</p>
<p>You can read about the US format <a href="http://www.aviationweather.gov/static/help/taf-decode.php">here</a>.</p>
<ul>
  <li> Source code: <a href="https://github.com/dmbaturin/pytaf">github.com/dmbaturin/pytaf</a> </li>
  <li> PyPI: <a href="https://pypi.python.org/pypi/pytaf">pypi.python.org/pypi/pytaf</a> </li>
</ul>
<p><strong>Note:</strong> this package is not related to the <a href="http://tlichtenberg.github.io/pytaf/">Python Test Automation Framework</a>,
   it's just an unfortunate three-letter acronym collision.</p>
<p>
It's not very actively maintained these days, so if you are interested in it and want to be
a maintainer, let me know.
</p>

<h2>Web tools</h2>

<h3>encapcalc</h3>
<p>A protocol overhead and maximum transmission unit calculator.</p>
<p>Allows you to easily calculate the MTU and TCP MSS for tunnels with
   different combinations of encapsulating and encapsulated protocols.
   Just select protocols by clicking on their buttons and see the header size and structure.</p>
<p>To my surprise, second most popular page on this website where people come by searching for
   &ldquo;MTU calculator&rdquo; and similar.</p>
<ul>
  <li>The tool: <a href="/tools/encapcalc">tools/encapcalc</a> </li>
  <li>The git repo: <a href="https://github.com/dmbaturin/encapcalc">github.com/dmbaturin/encapcalc</a> </li>
</ul>
<p>This is what it looks like:</p>
<img src="/images/encapcalc.png" />

<h2>All other software projects</h2>
<p>Look here: <a href="https://github.com/dmbaturin">github.com/dmbaturin</a>. </p>

<h2>Misc</h2>
<h3>FluxCDE</h3>
<p>CDE/Motif-inspired theme for the FluxBox window manager.</p>
<p>Download from <a href="https://www.box-look.org/p/1016906/">box-look</a>.</p>
<p>Local copy here: <a href="/files/fluxcde.tar.gz">fluxcde.tar.gz</a> </p>
<p>Works best with Motif-like themes for GTK/Qt/whatever else widget toolkits you use.
   And no, this is not a joke, I actually like Motif's look and feel and it had been my default
   theme for a good while. Fluxbox is not very good at reproducing the feel though.
   Also, actual CDE allows changing the color of window decorations, while FluxBox doesn't,
   this theme only exist in the Solaris color scheme.</p>
<p>I  haven't used FluxBox in a long time, so it's kept here mostly for historical/sentimental reasons.
   I'm not even sure if it works anymore, if it doesn't, let me know.</p>
<p>Also, if you are a CDE fan, you should know that The Open Group finally released it
   under LGPL in 2012 and you can have <a href="http://cdesktopenv.sourceforge.net/">the real thing</a> now.</p>

<h2>Obsolete projects</h2>

<h3>EMRK</h3>
<p>EMRK was the EdgeMax Rescue Kit, a TFTP-bootable rescue image for the Ubiquiti EdgeRouter Lite, allowed the user to
   reinstall EdgeOS from scratch or mount the filesystem and do some recovery by hand.</p>
<p>Made it to the Ubiquiti documentation and its <a href="http://community.ubnt.com/t5/EdgeMAX/EdgeMax-rescue-kit-now-you-can-reinstall-EdgeOS-from-scratch/td-p/514857">forum thread</a> was alive after
   over two years from the first post.</p>
<p>
I'm no longer involved in the Ubiquiti community and don't even use their hardware myself anymore, so I have no idea if it still works.
It also never supported any hardware but the original EdgeRouterLite (er100) because Cavium never made freely available SDKs for those.
You may still try it at your own risk, or take up its maintenance.
</p>
<ul>
  <li>The image: <a href="http://packages.vyos.net/tools/emrk/">packages.vyos.net/tools/emrk/</a> </li>
  <li>Usage guide in the <a href="http://community.ubnt.com/t5/EdgeMAX-Knowledge-Base/EdgeMAX-Last-resort-recovery-of-failed-EdgeOS-device/ta-p/1103623">Ubiquiti knowledge base</a>.</li>
  <li>Source code (scripts only, the rest is from the Cavium SDK and EdgeOS itself): <a href="https://github.com/vyos/emrk">github.com/vyos/emrk</a> </li>
</ul>
