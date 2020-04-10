<script>
function generate()
{
    var grammarBNF = document.getElementById("grammar").value;
    var startSymbol = document.getElementById("start-symbol").value;
    var separator = document.getElementById("separator").value;

    var output_container = document.getElementById("output");

    var output = "";

    try
    {
        var grammar = bnfgen.loadGrammar(grammarBNF);
        output = bnfgen.generate(grammar, startSymbol, separator);
    }
    catch(err)
    {
        output = err[1];
    }

    var output_text = document.createTextNode(output);
    output_container.innerHTML = '';
    output_container.appendChild(output_text);
}

function download_grammar(name)
{
    var url = window.location.protocol + '//' + window.location.host + window.location.pathname + name;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
           // Typical action to be performed when the document is ready:
           document.getElementById("grammar").value = xhttp.responseText;
           document.getElementById("output").innerHTML = "";
        }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
}

</script>


<h1>BNFGen</h1>
<p>
This is an online version of <a href="https://github.com/dmbaturin/bnfgen">BNFGen</a>,
yet another random text generator based on context-free grammars. Here are the highlights:
</p>
<ul>
  <li>Familiar BNF input and examples.</li>
  <li>Allows you to choose how often each alternative is taken by adding a &ldquo;weight&rdquo; and tweak your grammars for more realistic output.
      For example, <code>&lt;x_or_y&gt; ::= 10 "x" | "y" ;</code> will make <code>x</code> appear ten times more often than <code>y</code>.
  </li>
  <li>Supports comments in grammar files (shell style).</li>
  <li>Good syntax error reporting.</li>
</ul>
<h2>Grammar syntax</h2>
<p>
If you are already familiar with formal languages, here's a quick summary of the BNF variant used by BNFGen.
If not, there's a formal grammar <a href="#tutorial">tutorial</a> below the input form.
</p>
<ul>
  <li>Terminals are written in single or double quotes (<code>'foo'</code> or <code>"foo"</code>).</li>
  <li>Non-terminals are in angle brackets (<code>&lt;foo&gt;</code>, <code>&lt;_foo_bar0&gt;</code>).</li>
  <li>Rules are separated by semicolons.</li>
  <li>Empty alternatives (<code>&lt;foo&gt; ::= | "a" | "b";</code> are not allowed.</li>
  <li>The default starting symbol is <code>&lt;start&gt;</code>, but that's configurable.</li>
  <li>You can specify the symbol separator if you don't want to insert all spaces by hand.</li>
  <li>Recursion depth is not limited. You can hang your browser with infinite recursion.</li>
</ul>


<form action="">
  <div>
    <span>Sample grammars:</span>
    <input type="button" value="English" onclick='download_grammar("english.bnf")' />
    <input type="button" value="Balanced parentheses" onclick='download_grammar("parens.bnf")' />
    <input type="button" value="Serial numbers" onclick='download_grammar("serial.bnf")' />
  </div>
  <textarea id="grammar" style="width: 100%; min-height: 20em">
# A trivial subset of English.
# Generates one or more sentences.

&lt;start&gt; ::= 10 &lt;sentence&gt; &lt;line_break&gt; &lt;start&gt; | &lt;sentence&gt; ;

&lt;sentence&gt; ::= &lt;NP&gt; &lt;VP&gt; "." ;

&lt;NP&gt; ::= &lt;capitalized_article&gt; " " &lt;subject&gt; " " ;

&lt;VP&gt; ::= &lt;verb&gt; " " &lt;article&gt; " " &lt;object&gt;;

&lt;article&gt; ::= "a" | "the";
&lt;capitalized_article&gt; ::= "A" | "The" ;

&lt;subject&gt; ::=
    "customer" 
  | "system administrator"
  | "programmer"
;

&lt;object&gt; ::= "system" | "compiler" | "program";

&lt;verb&gt; ::= "used" | "installed" | "developed" ;

&lt;line_break&gt; ::= "\n"
  </textarea>
  <span>Start symbol: </span> <input id="start-symbol" type="text" value="start" />
  <span>Symbol separator: </span> <input id="separator" type="text" value="" />
  <input type="button" value="Generate" onclick="generate()" />
</form>
<span>Output:</span>
<div class="cfgen-output">
  <pre id="output"> </pre>
</div>

<h2 id="tutorial">Tutorial</h2>
<p>
If you don't know what's BNF or context-free grammar but still found this page somehow
and now you are curious, here's a quick and informal introduction that I hope will get
you interested in the subject. Don't worry, I'm not an expert myself.
</p>
<p>
Recognizing languages is much more important than generating them, so most of the time people
get introduced to formal languages by writing lexers and parsers. That's much less illustrative, and, let's admit it,
much less fun, and many of them give up or limit themselves to someone else's recipes without
learning the underlying concepts. It doesn't have to be that way&mdash;formal languages <em>are</em> fun.
</p>
<p>
BNFGen's primary purpose is to produce test data for other programs, but the same technique
can be used for generating <a href="http://www.theproofistrivial.com/">amusing texts</a>
or <a href="https://pdos.csail.mit.edu/archive/scigen/">fake research papers</a> that look plausible
enough to be accepted by unscrupulous conference committees.
</p>
<h3>Formal languages</h3>
<p>
A formal language is a language whose grammar is defined precisely. In natural languages we can have
endless arguments whether singular they or double negatives are grammatical, but in a formal language
it's known exactly what's grammatical and what isn't. All computer languages including programming languages
and file formats are examples of formal languages.
</p>
<h3>Symbols</h3>
<p>
A formal language is built upon a set of <em>symbols</em>. The meaning of that word is different from everyday use:
it can be a letter, a word, or a larger language unit such as a noun phrase or a whole sentence.
</p>
<p>
Symbols that, from a particular grammar's point of view, do not consist of anything, are called
<em>terminal symbols</em>. Compound symbols that can refer to other symbols are called <em>non-terminal</em>.
<p>
<p>
For example, in English, any infinitive looks like &ldquo;to VERB&rdquo;. &ldquo;To&rdquo; is a terminal
symbol—it always stays the same. The VERB part is a non-terminal because you can put any verb there
and it's still a valid infinitive.
</p>
<h3>Production (rewrite) rules</h3>
<p>
Grammars are defined in terms of <em>production rules</em> that describe all valid ways to reduce (rewrite)
non-terminal symbols to terminals. Every grammar has a <em>start symbol</em>&mdash;the largest
language unit it means to describe.
</p>
<p>
Consider a language of single-word sentences that can be &ldquo;Yes&rdquo; or &ldquo;No&rdquo;.
Those words are its terminal symbols, and the sentence is a non-terminal symbol.
In a blackboard notation, non-terminals are often represented by capital letters, while terminals
are represented by lowercase letters, and the sides of a rewrite rule are separated by an arrow.
</p>
<pre>
S → y (a sentence can be the terminal y, assumed to mean yes)
S → n (a sentence can also be the terminal n, assumed to mean no)
</pre>
<h3>Backus-Naur form</h3>
<p>
Programmers usually use more consice and keyboard-friendly conventions, such as BNF (Backus-Naur form)
named after two great computer scientists. They created some of the first programming languages,
Fortran and Algol respectively. BNF itself is not formally defined, it's only a convention
that says that non-terminals should be in angle brackets, terminals should be in quotes,
that the <code>|</code> character means &ldquo;or&rdquo;, and that <code>::=</code> means
&ldquo;is defined as&rdquo;
</p>
<p>
Let's define our yes/no language grammar in BNF. All examples below are valid BNFGen inputs
and you can paste them into the form.
</p>
<pre>
&lt;start&gt; ::= "Yes" | "No" ;
</pre>
<p>
Now let's try to define a grammar for &ldquo;to be or not to be&rdquo; sentences.
</p>
<pre>
&lt;start&gt; ::= &lt;infinitive&gt; "or not" &lt;infinitive&gt; ;

&lt;infinitive&gt; ::= "to" &lt;verb&gt; ;

&lt;verb&gt; ::= "be" | "do" | "compute"
</pre>
<p>
Note that &ldquo;or not&rdquo; is a single terminal symbol, since for the purpose
of this grammar we are not interested in its internal structure, much like we weren't
interested in the letters that &ldquo;yes&rdquo; and &ldquo;no&rdquo; consist of
in our previous example.
</p>
<p>
Hitting the &ldquo;generate&rdquo; button a few times proves that it's not following
the Hamlet's pattern&mdash;the verbs can be different.
</p>
<h3>Language complexity</h3>
<p>
This demonstrates the idea of different language complexity classes. The yes/no language
is <em>finite</em>&mdash;there are only two valid sentences in it.
</p>
<p>
The language of sentences like &ldquo;to be or not to be&rdquo; is <em>context-sensitive</em>—if
we require that the verb is the same. &ldquo;To be or not to do&rdquo; would not be valid.
When we pick the first verb, we create a context that limits what may come next.
</p>
<p>
It seems very simple to humans, but that grammar would be quite tricky to define in terms
of production rules, and it would take a lot more computational effort to check if a sentence
is valid or not since you need to remember what the previous symbols were.
</p>
<p>
BNFGen works with <em>context-free</em> grammars. Context-free means what came before a symbol is
not limiting what may come after it. This is why there can be only one symbol on the left-hand
side of a rule and it must be a non-terminal. A rule like <code>"a" &lt;A&gt; ::= ...</code>
would only apply to symbol A when it follows &ldquo;a&rdquo;. BNFGen doesn't remember what
came before a symbol, so it cannot do that. While it seems limiting, most computer languages
are in fact context-free&mdash;it's a complexity sweet spot where you can express
a lot, but don't run into serious trouble reading or producing it automatically.
</p>
<h3>Infinite languages and recursive rules</h3>
<p>
Our <em>actual</em> grammar for those sentences is limited in another way though&mdash; since there are only
three possible verbs, the language it defines is also finite. How to define an infinite language?
The key point is that a non-terminal symbol definition may refer to any symbol, including itself.
Some BNF extensions include special syntax for &ldquo;one or more&rdquo; and &ldquo;zero or more&rdquo;,
but they are really just shortcuts for recursive rules.
</p>
<p>
This is how we can define a language whose sentences consist of one or more letters <code>a</code>:
</p>
<pre>
&lt;start&gt; ::= "a" &lt;start&gt; | "a" ;
</pre>
<p>
If we take the second alternative, the rewriting process immediately terminates and we end up with
just one <code>a</code>. However, if we take the first alternative, we add one <code>a</code> to our
sentence, and then we have two alternatives: either to add another <code>a</code> and stop,
or take the first alternative again and repeat that process.
</p>
<p>
Of course, it's possible to accidentally create a rule will never produce anything.
This grammar will go deeper and deeper until you get a &ldquo;too much recursion&rdquo; error:
</p>
<pre>
&lt;start&gt; ::= "a" &lt;start&gt; | &lt;start&gt; ;
</pre>
<p>
BNFGen makes no attempt to check for non-terminating rules, even though it's possible and
there are grammar analysis tools that can do it for you.
</p>
<p>
In practice, a common problem with well-formed recursive rules is that if you select
alternatives randomly, they terminate too early and produce uninteresting texts.
For this reason, BNFGen allows you to specify how often each alternative should be taken.
</p>
<p>For example:</p>
<pre>
&lt;letter&gt; ::= 10 "a" | 5 "b" | "c" ;
</pre>
<p>You can combine it with recursive rules to make sure they produce longer strings:</p>
<pre>
&lt;start&gt; ::= 100 "a" &lt;start&gt; | &lt;start&gt; ;
</pre>

<h3>Further reading</h3>
<dl>
  <dt><a href="https://core.ac.uk/display/82795661">On certain formal properties of grammars</a></dt>
  <dd> Noam Chomsky's paper that started it all.</dd>
  <dt><a href="http://infolab.stanford.edu/~ullman/ialc.html">Introduction to Automata Theory, Languages, and Computation</a></dt>
  <dd>A classic textbook on the subject</dd>
  <dt><a href="http://www.kevs3d.co.uk/dev/lsystems/">L-system image generator</a></dt>
  <dd>You can make pretty pictures with grammars too.</dd>
</dl>
