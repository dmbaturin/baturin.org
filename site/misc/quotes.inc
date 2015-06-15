<blockquote>
Our biggest mistake was using the scary term &ldquo;monad&rdquo; rather than &ldquo;warm fuzzy thing&rdquo;.
</blockquote>
<div class="quote-source">&mdash; Symon Peyton Jones</div>

<blockquote>
I remember being impressed with Ada because you could write an
infinite loop without a faked up condition.  The idea being that in Ada
the typical infinite loop would be normally be terminated by detonation.
</blockquote>
<div class="quote-source"> &mdash; Larry Wall<a href="#infinite-loop"><sup>1</sup></a> </div>

<blockquote>
Ada language is a voice from thermonuclear hell, a curse to the humankind.
</blockquote>
<div class="quote-source"> &mdash; Melor Sturua<a href="#izvestia"><sup>2</sup></a> </div>

<blockquote>
Objects are state data with attached behavior; closures are behaviors with attached state data and without the overhead of classes.
</blockquote>
<div class="quote-source">&mdash; Peter Norvig</div>

<blockquote>
Given enough bugs, all eyes are shallow.
</blockquote>
<div class="quote-source">&mdash; Sad but true.</div>

<blockquote>
A programming language is low level when its programs require attention to the irrelevant. 
</blockquote>
<div class="quote-source">&mdash; Alan Perlis</div>

<blockquote>
Under no circumstances confuse the mesh with the interleave operator, except under confusing circumstances.
</blockquote>
<div class="quote-source">&mdash; D. Woods and J. Lyon, the INTERCAL reference manual</div>

<blockquote>
Formal logical proofs, and therefore programs&nbsp;&mdash;&nbsp;formal logical proofs that particular computations
are possible, expressed in a formal system called a programming language&nbsp;&mdash;&nbsp;are utterly meaningless.
To write a computer program you have to come to terms with this, to accept that whatever you
might want the program to mean, the machine will blindly follow its meaningless rules and come to
some meaningless conclusion.
</blockquote>
<div class="quote-source">&mdash; S. Dehnadi and R. Bornat</div>

<hr />

<a name="infinite-loop"> </a>
<p><sup>1</sup>
This can give a wrong impression that you can't escape a loop in Ada.
You can, either with &ldquo;exit [when &lt;condition&gt;]&rdquo; statement
or by raising an exception. Otherwise it's true, &ldquo;loop ... end loop;&rdquo;
creates an infinite loop, finite loops are created by adding a condition before the
&ldquo;loop&rdquo; keyword.
</p>

<a name="izvestia"> </a>
<p> <sup>2</sup>
This is from an <a href="http://www.orwell.ru/a_life/Melor_Sturua/russian/ms_01">article</a>
in a soviet newspaper that described the USA as an antiutopia.
The author compared it to Orwell's Newspeak as if it was a constructed language,
which suggests that he had little idea what a programming language is. Neither he knew that
Ada was used in the USSR too.
</p>

