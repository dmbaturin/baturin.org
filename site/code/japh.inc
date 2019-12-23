<h1>JAPHs and other fun with obfuscation</h1>

<p>Category: <span id="category">Fun</span></p>

<div id="generated-toc"></div>

<p>
I like recreational obfuscation. Not that I'm really good at it, but here are some examples of what I wrote during inspiration attacks.
</p>

<h3>Christmas tree 2.0</h3>

<pre class="language-python">
#!/usr/bin/env python3
# Happy winter holidays!
m = ['\0', '\0', '\\', '/', '|', '\n', '*', ' ']
n=1427204133903092885193970416731911941327299733
Y =  lambda f : lambda x, y : f( Y( f ) ) (x, y)
s = Y( lambda f : lambda  x, y : y if not x else
f(x >> 3, y + m[x & 7]))(n, "") ; print(s[::-1])
</pre>

<p>Should print an ASCII christmas tree:</p>
<pre>
    *    
   /|\   
  //|\\  
 ///|\\\ 
////|\\\\
</pre>

<p>
...eight years later. Another take on the custom ASCII art encoding theme, this time
every character is encoded by three bits. Since we only need six characters, while
three bits can encode 2<sup>3</sup>=8 distinct values, two unused values are mapped
to the null character. A loop would be too obvious, so I used recursion with accumulator,
implemented using the Y combinator instead of a direct recursive definition.
</p>
<p>Date: December 2018</p>

<h3>Fermat numbers</h3>

<pre class="language-perl">
#!/usr/bin/env perl
s3:n(q{Just });s5:n("another ");s17:n(q{Perl });s257:n("hacker");
s65537:n("\n");$p=1;sub n{$p++; $w=2**(2**$p)+1;print@_;goto"s$w"
unless grep/^1$/,map{1 if $w%$_==0}(2..int(sqrt($w)));exit;};n;1;
</pre>

<p>
Should print &ldquo;Just another Perl hacker&rdquo;. Labels contain <a href="https://en.wikipedia.org/wiki/Fermat_number">Fermat numbers</a>,
the n() subroutine generates a Fermat number and jumps to its label as long as the next number after it is prime. Since only the first five
are prime, it terminates as expected.
</p>
<p>The primality test is awfully inefficient but for very small numbers it doesn't matter.</p>
<p>Date: some 2013 I guess.</p>

<h3>Email is the key</h3>

<pre class="language-perl">
#!/usr/bin/env perl
@a=split(//, "daniil @ baturin  . org" ); # Daniil Baturin
@b=split(//,q/Px%!+o0Q6lh*7dp$.@8#t!nr/);while($i&lt;24){$_.=
chr((ord(@b[$i])-ord(@a[$i])+62)%94+32);$i++};print"$_\n"#
</pre>

<p>Should print traditional &ldquo;Just another Perl hacker&rdquo;.</p>
<p>
The idea was to make the email address an essential part of the program that cannot be modified without understanding.
</p>
<p>
So the email address was used as a key for the running key variant (that is, where the key length is equal to the plaintext length) of the Vigenere cipher
that uses the entire printable ASCII range as its alphabet.
</p>
<p>Date: late 2012.</p>

<h3>Three primes</h3>
<pre class="language-perl">
#!/usr/bin/env perl
use bigint;$i=
8311 * 11506699 * 5456897772732206286791603681159406921396401
;while($i){$_.=chr($i&amp;0x7f);$i&gt;&gt;=7;}print
</pre>

<p>Should print traditional &ldquo;Just another Perl hacker&rdquo;.</p>
<p>Simply obtains character codes from the string converted to number and then factorized.</p>
<p>Date: September 2012</p>

<h3>Seven points of Python</h3>

<pre class="language-python">
#!/usr/bin/env python3
from math import log, exp
print ("".join(map(lambda
x:chr((int((round((29*exp
((log(x)*6))/720-(283*exp
(log(x)*5))/240+(1657*exp
(log(x)*4))/144-(2209*exp
(log(x)*3))/48+(21403*exp
(log(x)*2))/360+(231*x)/5
+10.4)))))),range(1,7))))
</pre>

<p>Should print &ldquo;Python&rdquo;. Last I checked, it works with either Python 2.7 or Python 3.</p>
<p>The function that produces the ASCII codes of the &ldquo;Python\n&rdquo; string was constructed with
   Lagrange polynomial interpolation.</p>
<p>Date: October 2012</p>

<h3>Perl is in my heart (or vice versa)</h3>

<pre class="language-perl">
#!/usr/bin/env perl
use bigint;$h=0x3eff8fe0f0047c5fd;
while($h){$_.=$h&amp;1?' ':'*';$h/=2};
$_=join("\n",$_=~/.{11}/g);print;1
</pre>

<p>Should print an ASCII heart:</p>
<pre>
 *       * 
***     ***
 ********* 
   *****   
    ***    
     *     
</pre>

<p>Obviously, written for the Valentine's day. Specifically, for a Valentine's day contest my ISP once held
   where users submitted their postcards, videos, and the like. To their credit, they ran
   it in some test VM and posted a screenshot of the terminal.</p>
<p>Spaces are encoded with zeroes and stars with 1's, packed into a single hexadecimal number.</p>
<p>Date: February 2012 perhaps?</p>

<h3>Glider</h3>

<pre class="language-python">
#!/usr/bin/env python
glider = [[  0, 1, 0  ],
          [  0, 0, 1  ],
          [  1, 1, 1  ]]

hacker = [[ 300, 300, 175 ],
          [ 390, 270, 255 ],
          [ 400, 265, 260 ],
          [ 365, 325, 260 ],
          [ 350, 345, 225 ],
          [ 400, 320, 265 ]]

from functools import reduce; nsqr = lambda x: sp(x, x)
sp=lambda u,v: reduce(lambda x,y:x+y,list(map(lambda z:z[0]*z[1],zip(u,v))),0)
transform = lambda m,v:list(map(lambda mr: sp(mr,v),m))
print("".join(map(chr,map(lambda x:nsqr(transform(glider,x))//10**4,hacker))))
</pre>

<p>Should print the word &ldquo;Hacker&rdquo;.</p>
<p>
I generally support Eric Raymond's idea to use the glider as a technology neutral hacker subculture emblem
(even if I don't agree with a lot of his opinions).
</p>
<p>
Here the glider is used as a linear transformation matrix that is applied to a bunch of vectors
constructed so that the squares of their euclidian norms are equal to the ASCII character codes of the word &ldquo;Hacker&rdquo;
(rather, their first two or three decimal digits are).
The original version used floating point numbers, but I multiplied everything by 100 to avoid it
and use only precise and portable integer arithmetics. Works in both Python 3 and Python 2.7.
</p>
<p>This is an updated version that uses Python and integer arithmetics.</p>
<p>Date: November 2012, Python rewrite in December 2018</p>

<h3>Christmas tree</h3>

<pre class="language-perl">
#!/usr/bin/perl
$_=16;$j=1;$r.=chr($_*2)x($_+1)."*"."\n";for
($i=$_;$i&gt;$_&gt;&gt;1;$i--){;$r.=chr(2*$_)x$i.chr(
$_*3-1)x$j.chr($_*($_&gt;&gt;1)-sqrt($_)).chr($_*6
-sqrt($_))x$j.chr($_-6);$j++}$l=@a;print($r)
</pre>

<p>Early attempt, written in the late 2010 for the winter holidays.</p>

<div id="revision">This page was last modified: 2018-12-16</div>
