{% set date 2018 August 06 %}

<h1>The Zen of Ada</h1>

<p>This observation was first published in my old blog back in 2014.</p>
<p>
When I got interested in the <a href="http://www.adaic.org/">Ada language</a>, I inevitably learnt of the
<a href="https://www.dwheeler.com/steelman/steelman.htm">Steelman document</a> &mdash;
a set of requirements for the new programming language for mission critical applications.
Ada was designed specifically to meet those requirements, and its designers did an excellent
work.
</p>
<p>
When I started learning Ada, I already knew Python, and of course I also knew of the
<a href="https://legacy.python.org/dev/peps/pep-0020/">Zen of Python</a>, which is available
in the REPL as an easter egg (type &rdquo;import this&rdquo;), and I noticed that many points from it
seem to echo the Steelman requirements, at least in its part that defines general language design
and not specific requirements for a compiled, systems level, realtime language.
</p>
<p>
I don't know if Zen of Python was directly influenced by the Steelman document, or it's a result
of convergent evolution, but they bear a striking similarity in their spirit. Perhaps it's simply because
they should be requirements for any well designed language!
</p>
<p>For convenience I put the matching paragraphs side by side.</p>

<table>
<tr>
<th>Zen of Python</th>
<th>Steelman requirements</th>
</tr>
<tr>
<td>
Explicit is better than implicit.
</td>
<td>
The language should encourage user documentation of programs. It shall require explicit specification of
programmer decisions and shall provide defaults only for instances where the default is stated in 
the language definition, is always meaningful, reflects the most frequent usage in programs, and may be explicitly overridden.
</td>
</tr>
<tr>
<td>
Simple is better than complex. Complex is better than complicated.
</td>
<td>
The language should not contain unnecessary complexity. It should have a consistent semantic structure that minimizes the number of underlying concepts.
</td>
</tr>
<tr>
<td>
Readability counts.
</td>
<td>
The language should promote ease of program maintenance. It should emphasize program readability (i.e., clarity, understandability, and modifiability of programs).
</td>
</tr>
<tr>
<td>
Special cases aren’t special enough to break the rules.
</td>
<td>
It should have few special cases and should be composed from features that are individually simple in their semantics.
</td>
</tr>
<tr>
<td>
Errors should never pass silently.
</td>
<td>
The language shall be designed to avoid error prone features and to maximize automatic detection of programming errors. [...]
There shall be an exception handling mechanism for responding to unplanned error situations detected in declarations and statements during execution. 
</td>
</tr>
<tr>
<td>
Unless explicitly silenced.
</td>
<td>
It shall be possible during translation to suppress individually the execution time detection of exceptions within a given scope.
</td>
</tr>
<tr>
<td>
In the face of ambiguity, refuse the temptation to guess.
</td>
<td>
Translators shall produce explanatory diagnostic and warning messages, but shall not attempt to correct programming errors.
</td>
</tr>
<tr>
<td>
There should be one– and preferably only one –obvious way to do it.
</td>
<td>
The language should have uniform syntactic conventions and should not provide several notations for the same concept.
</td>
</tr>
<tr>
<td>
Namespaces are one honking great idea – let’s do more of those!
</td>
<td>
The language shall distinguish between open scopes (i.e., those that are automatically included in the scope of more globally declared variables)
and closed scopes (i.e., those in which nonlocal variables must be explicitly Imported).
Bodies of functions, procedures, and processes shall be closed scopes. Bodies of classical control structures shall be open scopes.
</td>
</tr>
</table>
