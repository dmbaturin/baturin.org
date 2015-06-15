{% set date 2015 June 10 %}

<h2>Standalone software</h2>

<h3>VyOS</h3>

<p>A Linux distribution for routers/firewalls/VPN concentrators.</p>
<p>Unlike GUI-oriented projects like pfSense or IPcop, we belive in the
power of the command line interface, so VyOS provides access to all
system functions through a unified CLI somewhat similar to that of JunOS.</p>
<p>Forked from now-abandoned Vyatta Core.</p>
<ul>
  <li> Website: <a href="http://www.vyos.net">vyos.net</a> </li>
  <li> Source code: it's a bit complicated, in a nutshell,
       <a href="http://github.com/vyos/build-iso/">github.com/vyos/build-iso</a>
       and all its submodules.</li>
  <li>Distrowatch: <a href="http://distrowatch.com/table.php?distribution=vyos">distrowatch.com/table.php?distribution=vyos</a> </li>
</ul>

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
   high level compiled language rather than a mix of C and shell.</p>
<ul>
  <li>Source code: <a href="https://github.com/dmbaturin/hvinfo">github.com/dmbaturin/hvinfo</a> </li>
</ul>

<h2>Libraries and plugins</h2>

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

<h3>mw-confirmedit-subnet-captcha</h3>
<p>MediaWiki ConfirmEdit CAPCTHA plugin that makes users do simple subnet calculations.</p>
<p>MediaWiki is probably the only software I like enough to write anything in PHP.</p>
<ul>
  <li> Source code: <a href="https://github.com/dmbaturin/mw-confirmedit-subnet-captcha">github.com/dmbaturin/mw-confirmedit-subnet-captcha</a> </li>
</ul>

<h2>Web tools</h2>

<h3>encapcalc</h3>
<p>A protocol overhead and maximum transmission unit calculator.</p>
<ul>
  <li>The tool: <a href="/tools/encapcalc">tools/encapcalc</a> </li>
  <li>The git repo: <a href="https://github.com/dmbaturin/encapcalc">github.com/dmbaturin/encapcalc</a> </li>
</ul>

<h2>All other software projects</h2>
<p>Look here: <a href="https://github.com/dmbaturin">github.com/dmbaturin</a>. </p>

<h2>Misc</h2>
<h3>FluxCDE</h3>
<p>CDE/Motif-inspired theme for Fluxbox window manager.</p>
<p>Download from <a href="http://box-look.org/content/show.php?content=115816">box-look</a>.</p>
<p>Works best with Motif-like themes for GTK/Qt/whatever else widget toolkits you use.
   And no, this is not a joke, I actually like Motif look and feel and it's been my default
   theme for a good while. Fluxbox is not very good at reproducing the feel though.</p>
