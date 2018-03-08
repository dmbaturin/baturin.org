{% set date 2018 March 07 %}

<h1>On &ldquo;easter eggs&rdquo;</h1>

<p>
Easter eggs in software occasionally become a point of controversy. At the one end of the spectrum, there are video games where,
quite obviously, anything goes. At the other end, there are ECG monitors, avionics software, and other life-critical systems
that arguably must not include any code that is not directly related to their primary purpose. In the middle, the balance
can be harder to find.
</p>
<p>
I don't think the &ldquo;no fun allowed&rdquo; approach of various &ldquo;trustworthy computing&rdquo; initiatives is
an unambiguously good idea, but the concerns for security and reliability are very real and needs to be addressed. I propose the following
easter egg policy that I believe is sensible and not too restrictive.
</p>

<h2>Easter egg policy</h2>

<h3>The purpose of the code should be clearly stated</h3>
<p>
A common objection to easter eggs is that they are pieces of code of unclear purpose. And, really, they shouldn't be.
I doubt anyone will feel uncomfortable if they find something like this in the code:
</p>
<pre>
# Display a cat picture for user entertainment when Esc-Meta-Alt-Control-Shift
# key combination is pressed
display_picture("resources/cat.png")
</pre>
<p>
If I stumble upon intentionally obsfuscated code that refers to a piece of binary data in an odd location,
that would definitely raise some questions. If the code is transparent and commented, even if I find it silly,
it's not a security or reliability concern.
</p>

<h3>Development process and standards for easter eggs and all other code should be the same</h3>
<p>
Another common objection is that if someone can put an easter egg into the code, then they can put anything in there
including spyware and backdoors.
</p>
<p>
The obvious fix is to keep it open to other developers and testers and follow the usual development process.
If there's code review, make it go through the code review. If an issue tracker entry is required for a change, make one.
</p>
<p>
Sure, it makes them easier to discover for the end users, but then again, there are there to be discovered, and not many users
even read the entire list of resolved issues in release notes, so it hardly takes away much surprise.
</p>
<p>
Besides, good easter eggs probably shouldn't lose much value when they are no longer a surprise. The best ones become
an important part of the user base lore, like Python's &ldquo;import this&rdquo;.
</p>
<p>
A side effect of this rule is that it makes overly elaborate easter eggs such as flight simulators embedded in office suits
impractical, which I believe is a good thing.
</p>

<h3>It should be impossible to trigger during normal program operation</h3>
<p>
The bad thing about accidentally triggered anything is that it's not always humans who operate programs.
Command line tools are especially prone to this because they are often used from scripts, but GUI programs are not immune
since they can also be operated by other programs &mdash; for example, automated testing tools.
</p>
<p>
If your script breaks, or you spend hours debugging an intermittent fault in acceptance tests and track it down to
an easter egg, that's anything but nice. If you accidentally use an invalid combination of options and get an easter
egg instead of a clear error message, that's not too nice either.
</p>
<p>
From this perspective, time-based easter eggs should be prohibited under any circumstances, and command line/request options,
key combinations, or action sequences that trigger an easter egg should not be variations of valid options.
</p>
<p>
For example, the &ldquo;sl&rdquo; command is a lot of fun<a href="#sl"><sup>1</sup></a>, but if mistyped &ldquo;ls&rdquo; is in a script, it will neither
fail with command not found, nor produce a correct result. Same with &ldquo;ddata&rdquo;. I have both installed on my workstation
and some other machines that I'm solely responsible for, but I'm dead set against having them installed on production servers.
</p>
<p>
Likewise, if while trying to debug a server issue late at night and type something like &ldquo;foo --verbose --quiet&rdquo; or a similar
&ldquo;impossible&rdquo; option combination, I would like to see at least an error message saying they are mutually exclusive
rather than an easter egg.
</p>
<p>
Perhaps there's an exception when time-based easter eggs are fine: when it's in the part of the program interface that cannot be
accessed programatically, or has no effect for programmatic access. OpenSuSE used to have a special GRUB theme that was only active
during winter holiday season &mdash; that's fine since there's simply no way any other program can access its interface. Likewise, if a web interface
changes its look for the April fools day in a way that doesn't affect the DOM, Selenium tests and the like will not get upset.
</p>

<a name="sl"> </a>
<p>
If you are not familiar with <a href="https://github.com/mtoyoda/sl">sl</a>, it displays an animated ASCII train.
There's also one for <a href="http://r-wos.org/hacks/gti">git</a>.
</p>
