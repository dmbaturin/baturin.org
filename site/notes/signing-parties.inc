<h1>Key signing parties considered useless</h1>

<h2>Overview</h2>

<p>
At this time, public key cryptography is used by just about everyone indirectly (e.g. in HTTPS), but it is only used directly by
a <s>bunch of nerds</s> a tiny minority.
</p>
<p>
Could it be something more, for example, a basis for a truly decentralized, serverless authentication system
(think OpenID, where is it by the way?). While the primary obstacle for its wider adoption is lack of tools
usable by non-specialists, I believe if these tools are created and see widespread use, the definitions
of identities and trust and the role of key signing parties will need to be reconsidered.
</p>

<h2>In a nutshell</h2>

<p>I believe that for most practical purposes the following conditions are sufficient for signing a person's GPG key:</p>
<ul>
  <li>They can receive a message sent to the email address associated with the key.</li>
  <li>They can decrypt the message.</li>
  <li>They can reply to the message and confirm via a side channel that they did.</li>
</ul>

<p>
This allows the basic &ldquo;that's the same person who generated the key&rdquo; level of trust validation, and I believe that's the only
thing the key alone is good for. Any higher level of trust can only be gained through personal interaction with people and, if needed,
independent identity verification.
</p>

<h2>What's being questioned?</h2>

<p>
In a web of trust, the idea is that if you see that Bob's key is signed by Alice, and you trust Alice, then
you can trust Bob too. Real trust, of course, takes time to develop.
</p>
<p>
Since, in the wild, it's relatively rare to find a key signed by someone you already know personally,
and many keys do not have any signatures at all, key signing parties try to solve that problem by having
random people sign each other's keys and substitute actual trust with id document checks.
</p>
<p>
The questions are what level of trust it automatically grants Bob, and what it means for a person to be Bob.
</p>

<h2>What kind of trust are we after?</h2>

<p>
There are, obviously, multiple level of trust. The biggest problem, however, is that in different situations the
trust requirements tend to grow exponentially rather than linearly.
</p>
<p>
For example, if I come to a book club and introduce
myself as Eugene (if it's not yet obvious, my name is <strong>not</strong> Eugene), people probably won't question it,
and if the truth comes to light, they won't be shocked to learn they've discussed Shakespeare with someone who's
actually not Eugene (though they will likely think I'm a weirdo). In that case it's just a handle, the impact of
this &ldquo;impersonation&rdquo; is very low, if it exists.
</p>
<p>
At the next level, for example starting an employment, the standards suddenly get a lot higher: for tax reporting purposes,
background checks and whatever else, they need the name that is written in my government id document, and they need to see
the document itself to verify that I haven't lied.
</p>
<p>
At the highest level, for example, opening a bank account for my company, it's not only needed to verify my id document,
it's also important to verify that I'm actually authorized to do that.
</p>

<p>
Most situations tend to fall under the first level, in the real world and on the Internet alike. It's handy to know that
Alice on the Foo mailing list and the Bar mailing list is really the same person. This is the equivalent of seeing the same
face at a book club and a grocery store in the real world. Otherwise, the key alone doesn't grant one any more trust;
if you consider allowing someone to login to your servers, or lend them money, you get to rely on your prior interaction with them.
You definitely would like to know that it's not someone who claims to be Alice trying to defraud you, but if the key is the same,
you can be sure that it's either really the same person, or an elaborate case of identity theft (and if you have any doubt,
you'd rely on non-cryptographical ways of authentication to verify it anyway, such as asking a question only Alice would know the
answer to).
</p>

<p>
Curiously, in the most sensitive application of public keys on the Internet, namely operating system image and software package
signing, people typically go for side channel checks right away if they have any doubts, rather than rely on signatures. 
</p>

<h2>A disjoint web</h2>

<p>
The most effective trust indicator (whatever trust is) is a signature of someone you ever communicated with, or at least
have an idea who it is. This is a relatively rare occurence globally. People from geographically distant regions do not
get a chance to meet often, and even if the six degrees of separation hypothesis is true, that alone is enough to create
a situation when most keys are only signed by people you have no idea about.
</p>

<p>
A rather more sensitive issue is that even within the same country, not all people have equal opportunities to participate
in face to face key signing parties, and those excluded are often the most vulnerable groups such as people in remote areas or people for whom
the cost of transportation can be prohibitive for other reasons, or people with mobility impairments. Should we be punishing those groups for the sake
of security, real or perceived? And is security improvement from id document check it real or perceived?
</p>

<h2>The security of id documents and the risk of identity theft</h2>

<p>
How exactly secure are the papers? Unlike cryptographical protocols, they cannot possibly offer information theoretical security.
The local government can simply issue a &ldquo;genuine&rdquo; id document to any name to begin with. Still, with modern 
anti-counterfeiting measures, they are usually secure enough to make it very hard to fool a trained professional.
</p>
<p>
However, most of the people at key signing events are not trained professionals, so the real question is how secure are they
when checked by common people. Are you familiar with all security devices used in your country's id documents? Do you know
how hard is each of them to replicate? Do you know how hard is it to get a convincing fake from a dark web marketplace?
At the latest key signing party, did you bother to check even the most obvious ones such as holograms or watermarks?
</p>
<p>
It gets worse if we try to close the gaps in the web. If you live in Albania, do you know what constitutes a valid id document
in Zambia (which is geographically as well as alphabetically far away from Albania) and what it may look like?
</p>
<p>
My feeling is that a person with an average quality fake can easily pass the check at a typical party. A bigger question
is if this is a relevant risk though.
</p>
<p>
Getting a brand new (or simply different) key signed to someone else's name would be a very impractical way to commit
identity theft. First, sudden and unannounced key change is suspicious enough on its own. Second, if the identity theft target
still has access to the original key, they can easily and securely notify everyone that the new key owner is an impostor.
If I make a key with Linus Torvalds or Theo de Raadt written in it, and try to impersonate the Linux kernel or OpenBSD
maintainers respectively with it, my chances of success are very slim whether I get it signed by someone else or not.
</p>
<p>
A much &ldquo;better&rdquo; way to commit identity theft would be to gain control of the private key and the email account
itself. With even intelligent and knowledgeable people sometimes ignoring security precautions, it's also rather more likely to happen,
and the incidents with the servers of a few major open source projects getting compromised by using stolen SSH keys sadly confirm this.
</p>
<p>
The worst part is that the initial identity verification does nothing to prevent or make it easier to detect this kind of
identity theft. This isn't too different from the real world though, if someone forges your signature convincingly,
you can have a hard time proving it wasn't you.
</p>
<p>But then, how important is it that the name in the key matches the name in my id document?</p>

<h2>Real names</h2>

<p>
What's a &ldquo;real name&rdquo; anyway and how important is it that the name in the key is &ldquo;real&rdquo;.
Moreover, is the web of trust an extension of existing governmental identification systems?
</p>
<p>
Since PGP keys cannot be used as substitutes for government id documents in situations where one would be required,
is there any point in tying one with the other? Where governments use public key cryptography for authentication, they
use their own certificate authorities anyway.
</p>
<p>
Since in most jurisdictions you can change your legal name, and you often get to choose the name when you receive your first id,
what the government id document really certifies in general case is that you are the one who initiated the creation of that authentication device.
</p>
Could the PGP infrastructure instead be an alternative to that system that is backed by actual trust of individual people for authenticity and
cryptographical algorithms for security? I believe in practice all one needs to know is that the key was created by the person using it to
establish their identity, and from this point of view, the ultimate form of trust would come from witnessing the key creation. I'm still to hear
of a PGP setup party event though.
</p>
<p>
The biggest difference from the government id is that it's easy to establish multiple identities. I don't think this is inherently harmful,
it's relatively common, for example, for writers to use different pen names for different genres. Likewise, in some religious traditions it's
customary to assign religious names to monks and/or clergy, I can't see why they cannot use their religious name in a theology mailing list but
keep using their secular names in other contexts. While multiple identities can be used for less than constructive purposes, they are not unique in that,
anything can be used that way. Systems that do need to enforce uniqueness of the person/account pair will have to resort to non-cryptographic means of
verification, and if they have a strong uniqueness requirement, will not trust any web of trust in any case (more on that later).
</p>
<p>
If we take this further, there seems to be a valid use case for intentionally anonymous but signed keys. Suppose a whistleblower communicates
with a reporter from the Foo Daily using an anonymous mail service.
A reporter from the Quux Herald Tribune wants to communicate with them as well, but wants to make sure it's the same person. The reporter from the Foo Daily,
who has seen the key used for sending the original message can sign the whistleblower's key to let all other reporters know they can trust that it belongs
to the person in question.
</p>

<h2>Conclusion</h2>

<p>
A PGP key that is tied to a government id is not inherently more trustworthy for the purposes of &ldquo;casual trust&rdquo; and doesn't make the owner
automatically more trustworthy for higher trust levels. Most people are acting in good faith, and for those who aren't, a pseudonymous key is unlikely
to be a good attack vector.
</p>
<p>
Rather than try to emulate the governments, we can value the keys for what they are and what they are actually good for.
</p>

<time id="last-modified">2018-03-21</time>
