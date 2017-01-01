{% set date 2015 June 08 %}

<h1>JAPHs and other fun with obfuscation</h1>

<div id="generated-toc"></div>

<p>
I like recreational obfuscation. Not that I'm really good at it, but here's some examples of what I wrote during inspiration attacks.
</p>

<h3>Fermat numbers</h3>

<pre>
#!/usr/bin/env perl
s3:n(q{Just });s5:n("another ");s17:n(q{Perl });s257:n("hacker");
s65537:n("\n");$p=1;sub n{$p++; $w=2**(2**$p)+1;print@_;goto"s$w"
unless grep/^1$/,map{1 if $w%$_==0}(2..int(sqrt($w)));exit;};n;1;
</pre>

<p>Should print &ldquo;Just another Perl hacker&rdquo;. Labels contain Fermat numbers, n() subroutine jumps to them as long as the next number is prime.</p>

<h3>Email is the key</h3>

<pre>
#!/usr/bin/env perl
@a=split(//, "daniil @ baturin  . org" ); # Daniil Baturin
@b=split(//,q/Px%!+o0Q6lh*7dp$.@8#t!nr/);while($i&lt;24){$_.=
chr((ord(@b[$i])-ord(@a[$i])+62)%94+32);$i++};print"$_\n"#
</pre>

<p>Should print traditional &ldquo;Just another Perl hacker&rdquo;.</p>
<p>The idea was to make the email address not easily removable or replaceable to use it as a signature block.</p>
<p>So the email address was used as a key for running key variant of Viginere cipher modified to work on the
   whole range of printable ASCII characters instead of its traditional letter only form.</p>

<h3>Three primes</h3>
<pre>
#!/usr/bin/env perl
use bigint;$i=
8311 * 11506699 * 5456897772732206286791603681159406921396401
;while($i){$_.=chr($i&amp;0x7f);$i*gt;&gt;=7;}print
</pre>

<p>Should print traditional &ldquo;Just another Perl hacker&rdquo;.</p>
<p>Simply obtains character codes from the string converted to number and then factorized.</p>

<h3>Seven points of Python</h3>

<pre>
#!/usr/bin/python
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

<h3>Perl is in my heart (or vice versa)</h3>

<pre>
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

<p>Obviously, written for Feb 14th. Specifically, for a valentines day contest my ISP once held
   where users submitted their postcards, videos, and the like. To their credit, they ran
   it in some test VM and posted a screenshot of the terminal.</p>
<p>Spaces are encoded with zeroes and stars with 1's, packed into single hexadecimal number.</p>

<h3>Glider</h3>

<pre>
#!/usr/bin/env perl
@glider= ([  0, 1, 0  ],
          [  0, 0, 1  ],
          [  1, 1, 1  ]);

@hacker= ([ 3.00, 3.00, 2.45 ],
          [ 4.00, 2.90, 2.85 ],
          [ 4.00, 2.90, 2.88 ],
          [ 3.90, 3.80, 2.95 ],
          [ 3.90, 3.60, 2.85 ],
          [ 4.00, 3.95, 3.04 ],
          [ 1.00, 1.00, 0.95 ]);

sub mul(\@\@){@r=();for($i=0;$i&lt;3;$i++){for($j=0;$j&lt;3;$j++){;$r
[$i]+=$_[0][$i][$j]*$_[1][$i]}}return\@r}sub ip(\@){$w=0;for($k
=0;$k&lt;3;$k++){$w+=$_[0][$k]*$_[0][$k];}return chr(int($w));}for
($n=0;$n&lt;=6;$n++){$_.=ip(@{mul(@glider,@{$hacker[$n]})});}print
</pre>

<p>I generally support Eric Raymond's idea to use the glider as technology neutral hacker subculture emblem.</p>
<p> Here the glider is represented as a 3x3 matrix and the ASCII codes or the &ldquo;Hacker&rdquo; word characters are calculated
    as (squares of) euclidian norms of constructed vectors after applying the glider operator matrix to them.</p>

<h3>Christmas tree</h3>

<pre>
#!/usr/bin/perl
$_=16;$j=1;$r.=chr($_*2)x($_+1)."*"."\n";for
($i=$_;$i&gt;$_&gt;&gt;1;$i--){;$r.=chr(2*$_)x$i.chr(
$_*3-1)x$j.chr($_*($_&gt;&gt;1)-sqrt($_)).chr($_*6
-sqrt($_))x$j.chr($_-6);$j++}$l=@a;print($r)
</pre>

<p>Early attempt, made for 2010 winter holidays.</p>
