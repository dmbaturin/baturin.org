<inlineStyle>
table.mutations {margin-left: auto; margin-right: auto; }
td, th { text-align: center; }
th {background-color: white;}
.lenition {background-color: #bcf3f4;}
.eclipsis {background-color: #f4debc;}
</inlineStyle>

# Irish initial mutations

For people who start learning Irish (and other Celtic languages), <wikipedia page="Consonant_mutation#Celtic_languages">initial consonant mutations</wikipedia>
often become stumbling blocks.
They were certainly confusing for me, and _still_ are: knowing how they work and how to use them is one thing,
but to be able to understand the language and speak fluently, you need to train yourself to mutate and un-mutate words without thinking.
There is only one way to do that — constant practice and repetition.
From that point of view _just_ memorizing mutation tables and practicing them (e.g., with help from spaced repetition software) 
is a perfectly good approach.

If you have a student memorize the mutation tables up front, you can tell them that the past tense of many verbs
is formed by leniting them, for example. It is just that memorizing mysterious tables without any context can drive any student insane. 
But not telling students that the processes that are used to form the past tense, possessives, relative clauses,
and many other constructs are the same processes and that they are predictable also deprives them of an important learning tool.

Most books and courses use a mixed approach: show that the patterns exist and then gradually teach them by example
("táim i mo <em>ch</em>onaí i <strong>g</strong>Corcaigh, tá siad ina <strong>g</strong>conaí i <strong>n</strong>Gallimh...").
However, the phonetic nature of mutations is very rarely highlighted. The main reason is that it may take a while to explain
to people without any prior training in phonology. But it may in fact be an excellent opportunity for students
to learn something about human speech that they did not realize before, and for some people it may be easier
to memorize mutations when they see that those mutations are not random at all.
So, I made an attempt to present information about mutations in a way that emphasizes the phonetic nature
of what is going on.

For people who are not phonology nerds yet, I tried to annotate all terms and <wikipedia page="International Phonetic Alphabet">IPA</wikipedia>
symbols with Wikipedia links for further reading.

Initial consonant mutations in Goidelic languages tend to preserve the <wikipedia>place of articulation</wikipedia>
but change its <wikipedia page="Manner of articulation">manner</wikipedia>.
For example, the <wikipedia>voiceless velar plosive</wikipedia> [k] becomes [x] (<wikipedia>voiceless velar fricative</wikipedia>) after lenition
and [g] (<wikipedia>voiced velar plosive</wikipedia>) after eclipsis.
Notice how its place of articulation — <wikipedia page="Velar_consonant">velar</wikipedia>
<fn id="velar">Produced with the back of the tongue against the soft palate. The soft palate is called velum in Latin, hence the term.</fn> — remains unchanged.

It is not always so simple. The extinction of <wikipedia>dental fricative</wikipedia> sounds in Modern Irish made the situation with lenited
[d] and [t] quite complicated, for example: in Old Irish, their lenited versions were always [<wikipedia page="Voiced dental fricative">ð</wikipedia>]
and [<wikipedia page="Voiceless dental fricative">θ</wikipedia>],
but in Modern Irish, [t] is lenited to [<wikipedia page="Voiceless glottal fricative">h</wikipedia>],
and [d] can be either [<wikipedia page="Voiced velar fricative">ɣ</wikipedia>] (broad) or [<wikipedia page="Voiced palatal fricative">ʝ</wikipedia>] (slender).
And that is not counting "dh", "th", and "gh" in non-initial positions where they can be either completely silent
or just cause <wikipedia page="Compensatory lengthening">compensatory vowel lengthening</wikipedia>.

Still, the phenomenon has clear patterns:

<table class="mutations">
  <colgroup>
    <col />
    <col class="lenition"/>
    <col class="eclipsis"/> 
  </colgroup>
  <tr>
    <th>Unmutated </th>
    <th class="lenition">Séimhiú (lenition)</th>
    <th class="eclipsis">Urú (eclipsis)</th>
  </tr>
  <tr>
    <td>voiceless fricative</td>
    <td>silent or glottal fricative</td>
    <td>voiced fricative or unchanged</td>
  </tr>
  <tr>
    <td colspan="3">voiced fricative — never occurs in initial positions in native stems</td>
  </tr>
  <tr>
    <td>voiceless stop</td>
    <td>voiceless fricative</td>
    <td>voiced stop</td>
  </tr>
  <tr>
    <td>voiced stop</td>
    <td>voiced fricative</td>
    <td>nasal occlusive</td>
  </tr>
  <tr>
    <td>nasal occlusive</td>
    <td>voiced fricative (when possible)</td>
    <td>unchanged</td>
  </tr>
</table>

More specifically, lenition turns every occlusive sound into a fricative, when it is possible.
Eclipsis makes voiceless fricatives and plosives into their voiced counterparts, and voiced plosives into nasal occlusives.

We can present all mutations in a table, grouped by the place of articulation.

<table class="mutations">
  <colgroup>
    <col />
    <col span="2" class="lenition"/>
    <col span="2" class="eclipsis"/> 
  </colgroup>
  <tr>
    <th>Unmutated</th>
    <th colspan="2" class="lenition">Séimhiú (lenition)</th>
    <th colspan="2" class="eclipsis">Urú (eclipsis)</th>
  </tr>
  <tr>
    <th> </th>
    <th class="lenition">Spelling</th>
    <th class="lenition">Pronunciations</th>
    <th class="eclipsis">Spelling</th>
    <th class="eclipsis">Pronunciations</th>
  </tr>
  <tr>
    <th colspan="5">
      <wikipedia page="Bilabial consonant">Bilabial</wikipedia> and
      <wikipedia page="Labiodental consonant">labiodental</wikipedia> consonants<fn id="bilabial">In Modern Irish, lenited versions of bilabial occlusives (bh/mh)
        are often realized as labiodental rather than bilabial fricatives. Generally, [v]/[β]/[w] and [f]/[ɸ] are <wikipedia>allophones</wikipedia>
        in Modern Irish.</fn>
    </th>
  </tr>
  <tr>
    <td>
      <wikipedia page="Voiceless labiodental fricative">f</wikipedia><fn id="f">F is usually labiodental in Irish but in some words and in some speakers
      it may be bilabial — one common example where it often happens is <em>faoi</em> (under).</fn>
    </td>
    <td>fh</td>
    <td>Silent</td>
    <td>bhf</td>
    <td>[<wikipedia page="Voiceled labiodental fricative">v</wikipedia>] (only slender), [<wikipedia page="Voiced bilabial fricative">β</wikipedia>],
        [<a href="https://en.wikipedia.org/wiki/Voiced_labial%E2%80%93velar_approximant">w</a>] (broad)</td>
  </tr>
  <tr>
    <td><wikipedia page="Voiceless bilabial plosive">p</wikipedia></td>
    <td>ph</td>
    <td>[<wikipedia page="Voiceless bilabial fricative">ɸ</wikipedia>], [<wikipedia page="Voiceless labiodental fricative">f</wikipedia>]</td>
    <td>bp</td>
    <td>[<wikipedia page="Voiced bilabial plosive">b</wikipedia>]</td>
  </tr>
  <tr>
    <td><wikipedia page="Voiced bilabial plosive">b</wikipedia></td>
    <td>bh</td>
    <td>[<wikipedia page="Voiceled labiodental fricative">v</wikipedia>] (only slender), [<wikipedia page="Voiced bilabial fricative">β</wikipedia>],
        [<a href="https://en.wikipedia.org/wiki/Voiced_labial%E2%80%93velar_approximant">w</a>]
    </td>
    <td>mb</td>
    <td>[<wikipedia page="Voiced bilabial nasal">m</wikipedia>]</td>
  </tr>
  <tr>
    <td><wikipedia page="Voiced bilabial nasal">m</wikipedia></td>
    <td>mh</td>
    <td>[<wikipedia page="Voiceled labiodental fricative">v</wikipedia>] (only slender), [<wikipedia page="Voiced bilabial fricative">β</wikipedia>],
        [<a href="https://en.wikipedia.org/wiki/Voiced_labial%E2%80%93velar_approximant">w</a>]
    </td>
    <td colspan="2">Cannot change — already nasal</td>
  </tr>
  <tr>
    <th colspan="5">
      <wikipedia page="Dental consonant">Dental</wikipedia> and <wikipedia page="Alveolar consonant">alveolar</wikipedia> consonants
    </th>
  </tr>
  <tr>
    <td><wikipedia page="Voiceless alveolar plosive">t</wikipedia></td>
    <td>th</td>
    <td>Old Irish: [<wikipedia page="Voiceless dental fricative">θ</wikipedia>]<br> Modern Irish: [<wikipedia page="Voiceless glottal fricative">h</wikipedia>]</td>
    <td>dt</td>
    <td>[<wikipedia page="Voiced alveolar plosive">d</wikipedia>]</td>
  </tr>
  <tr>
    <td><wikipedia page="Voiced alveolar plosive">d</wikipedia></td>
    <td>dh</td>
    <td>Old Irish: [<wikipedia page="Voiced dental fricative">ð</wikipedia>]<br> Modern Irish: [<wikipedia page="Voiced velar fricative">ɣ</wikipedia>] (broad),
      [<wikipedia page="Voiced palatal fricative">ʝ</wikipedia>] (slender)</td>
    <td>nd</td>
    <td>[<wikipedia page="Voiced alveolar nasal">n</wikipedia>]</td>
  </tr>
  <tr>
    <td><wikipedia page="Voiced alveolar nasal">n</wikipedia></td>
    <td colspan="2">Does not change</td>
    <td colspan="2">Cannot change — already nasal</td>
  </tr>
  <tr>
    <th colspan="5">
      <wikipedia page="Velar consonant">Velar</wikipedia> consonants
    </th>
  </tr>
  <tr>
    <td><wikipedia page="Voiceless velar plosive">c</wikipedia></td>
    <td>ch</td>
    <td>[<wikipedia page="Voiceless velar fricative">x</wikipedia>]</td>
    <td>gc</td>
    <td>[<wikipedia page="Voiced velar plosive">g</wikipedia>]</td>
  </tr>
  <tr>
    <td><wikipedia page="Voiced velar plosive">g</wikipedia></td>
    <td>gh</td>
    <td>[<wikipedia page="Voiced velar fricative">ɣ</wikipedia>] (broad), [j] (slender)</td>
    <td>ng</td>
    <td>[<wikipedia page="Voiced velar nasal">ŋ</wikipedia>]</td>
  </tr>
  <tr>
    <th colspan="5">
      <wikipedia>Sibilants</wikipedia>, <wikipedia>approximants</wikipedia>, and <wikipedia page="Tap consonants">taps</wikipedia> 
    </th>
  </tr>
  <tr>
    <td>s</td>
    <td>sh</td>
    <td>[<wikipedia page="Voiceless glottal fricative">h</wikipedia>]</td>
    <td colspan="2">
      Does not change<fn id="s">Could become [z] if Irish had voiced sibilants.
      Reportedly, that was the case in the Clare Island dialect and it was spelled &ldquo;ds&rdquo; there,
      e.g., &ldquo;ár dsagart&rdquo; /a:r zagart/.</fn>
     </td>
  </tr>
  <tr>
    <td>l</td>
    <td colspan="2">
      Does not change<fn id="l">L could become [<a href="https://en.wikipedia.org/wiki/Voiceless_dental_and_alveolar_lateral_fricatives">ɬ</a>] in an alternative universe. Welsh has that sound
      and there are pairs of cognates that have [l] in Irish but [ɬ] in Welsh:
      lámh vs llaw (hand), leabhar vs llyfr...
      The likely reason is that initial [l] tended to be realized as [<a href="https://en.wikipedia.org/wiki/Voiced_labial%E2%80%93velar_approximant">w</a>]
      in many Irish dialects for a long time and was lenited to [l] in phrases like &ldquo;mo lámh&rdquo;.</fn>
    </td>
    <td colspan="2">Cannot change<fn id="nl">A &ldquo;nasalized L&rdquo; sound is technically possible to articulate
      but nasalized approximants do not seem to occur in any human language, for better or worse.</fn>
    </td>
  </tr>
  <tr>
    <td>r<fn id="r">R in Modern Irish can be either a trill or an approximant, speakers usually consider them allophones.</fn></td>
    <td colspan="2">Does not change<fn id="rh">Welsh has a voiceless counterpart of the alveolar trill ([<a href="https://en.wikipedia.org/wiki/Voiceless_alveolar_trill">r̥</a>]),
    but that sound does not occur in Irish.</fn>
    </td>
    <td colspan="2">Does not change</td>
  </tr>
</table>

## Acknowledgements

Many thanks to [Emma Nic Cárthaigh](http://research.ucc.ie/profiles/A007/emmaniccarthaigh) for beta reading, suggestions, and corrections.

<hr>
<div id="footnotes"> </div>
