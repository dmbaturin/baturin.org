# Declarative parse error reporting with Menhir

<time id="last-modified">2019-08-24</time>
<tags>programming, parsing, ocaml</tags>

<p id="summary">
Many parsers for custom formats aren't very user friendly when it comes to error handling.
At best you get the line and column where the error is, at worst a &ldquo;Parse error&rdquo;
message is all you get. Can we do better? With right tools, we definitely can and it's not that hard.
</p>

I doubt anyone is making unfriendly parsers intentionally. It's just that many tools make it hard to do.
Making parsers is a well-understood problem with a variety of ready to use solutions,
but few of them make handling errors easy.

When I started working on a new version of [bnfgen](https://github.com/dmbaturin/bnfgen/),
my main goal was to make it usable for non-programmers (writers, linguists, and anyone
who may be interested in formal languages). After all, a programmer can make an equivalent tool
in an evening—if ease of use for anyone else isn't a concern.

It's been using [Menhir](http://gallium.inria.fr/~fpottier/menhir/)—the de facto standard
LR(1) parser generator for OCaml from the start, but error handling was rudimentary at best.
The traditional approach with an error token is there, like in any other yacc, but I never
found it very appealing, so I started looking for alternatives and found that recent Menhir
versions indeed offer one—they allow declarative error messages.

However, the new way of error handling doesn't work with the classic yacc-like &ldquo;monolithic&rdquo;
API, so I had to learn about the new incremental API first.

## The incremental API

Menhir has two parser API options: monolithic and incremental. The monolithic approach
is typical for yacc-like tools: it produces a blackbox module with no user-serviceable
parts inside. There's one function of type `(Lexing.lexbuf -> token) -> Lexing.lexbuf -> 'a`
that takes a lexer produced by ocamllex or a compatible tool and gives you an AST.

The incremental API is much more flexible. It makes no assumptions about the lexer API,
you just give it tokens, and it gives you the parser state that you can inspect and act upon.
The downside is that you are responsible for driving it, there's no &ldquo;just give me the AST&rdquo;
function.

Let's look at an example of driving the parser.

The grammar in question is BNF-like with two notable additions: rules are semicolon-separated,
and you can add a &ldquo;weight&rdquo; before tokens to influence how often an alternative is taken.
For example, in this rule, the recursive alternative is ten times more likely to be taken:

```
<start> ::= 10 <start> "foo" | "foo" ;
```

The parser is at [src/bnf_parser.mly](https://github.com/dmbaturin/bnfgen/blob/master/src/bnf_parser.mly).
There is no need to change anything in the parser to make use of the incremental API, instead we need to add
one more module for its driver.

Here is the code that you can use as a blueprint for your own driver, after adjusting the module and type names:

```ocaml
open Util
open Lexing

module I = Bnf_parser.MenhirInterpreter

let get_parse_error env =
    match I.stack env with
    | lazy Nil -> "Invalid syntax"
    | lazy (Cons (I.Element (state, _, _, _), _)) ->
        try (Bnf_parser_messages.message (I.number state)) with
        | Not_found -> "invalid syntax (no specific message for this eror)"

let rec parse lexbuf (checkpoint : Grammar.grammar I.checkpoint) =
  match checkpoint with
  | I.InputNeeded _env ->
      let token = Bnf_lexer.token lexbuf in
      let startp = lexbuf.lex_start_p
      and endp = lexbuf.lex_curr_p in
      let checkpoint = I.offer checkpoint (token, startp, endp) in
      parse lexbuf checkpoint
  | I.Shifting _
  | I.AboutToReduce _ ->
      let checkpoint = I.resume checkpoint in
      parse lexbuf checkpoint
  | I.HandlingError _env ->
      let line, pos = Util.get_lexing_position lexbuf in
      let err = get_parse_error _env in
      raise (Syntax_error (Some (line, pos), err))
  | I.Accepted v -> v
  | I.Rejected ->
       raise (Syntax_error (None, "invalid syntax (parser rejected the input)"))
```

The `Syntax_error` exception is defined in the [Util](https://github.com/dmbaturin/bnfgen/blob/master/src/util.ml)
module, it's so that the lexer and the parser can easily use the same exception and it could be caught in the same way.

The top	level type of the parser is `Grammar.grammar`, so we define a `parse` function that will have
`lexbuf -> Grammar.grammar I.checkpoint -> Grammar.grammar` type. The `'a I.checkpoint` is the type of the parser state.

In the `I.InputNeeded` state we feed the parser a token. The `I.offer` function takes a tuple of the token and its
starting and ending positions in the input file. The `lexbuf` record produced by ocamllex exposes them as
`lex_start_p` and `lex_curr_p` fields—a somewhat obscure fact that I never needed to know when using the monolithic API.

The state most interesting to us for this post is `I.HandlingError`. It comes with a value that contains information
about the error state that we pass to the `get_parse_error` function. That function calls `Bnf_parser_messages.message`
on the state number to get an error message.

Where the `Bnf_parser_messages` module comes from is the most interesting part. It's autogenerated from declarative
descriptions of error messages.

The workflow is:
* Run `menhir --list-errors` on the parser to get a template file with error states (`bnf_parser.messages`).
* Edit the `.messages` file and add error message descriptions.
* Run `menhir --compile-errors bnf_parser.messages bnf_parser.mly` to generate the error handling module.

Let's inspect these steps in detail.

## Getting the error state data

Menhir can produce a file that lists all token sequences that ends in error states.
You just need to call it with `--list-errors` option and it will write the data to stdout.
In our case, the command is `menhir --list-errors src/bnf_parser.mly`.

```
menhir --list-errors src/bnf_parser.mly > src/bnf_parser.messages
```

In BNFGen, that file is at [src/bnf_parser.messages](https://github.com/dmbaturin/bnfgen/blob/master/src/bnf_parser.messages).

A typical entry looks like:

```
grammar: IDENTIFIER DEF NUMBER SEMI
##
## Ends in an error in state: 8.
##
## rule_rhs_part -> NUMBER . rule_rhs_symbols [ SEMI OR EOF ]
##
## The known suffix of the stack is as follows:
## NUMBER
##

<YOUR SYNTAX ERROR MESSAGE HERE>
```

States are defined by token sequences, so they will stay the same if state numbers
in the automaton change.

## Adding error messages

All you need to do is to replace the error message placeholders with actual error messages.
That's also the tricky part because error states is all you get, and a state doesn't always
equal one and only one incorrect input.

Menhir is still an LR(1) generator so the lookahead depth is just one token. Less ambiguous
languages allow much better error messages, so if you are in position to design a language
from scratch, you should take it into account.

My attempt to apply this approach to another project that deals with another language
failed because that language is so ambiguous that even a much deeper lookahead wouldn't help much.

The main danger is to underestimate the number of ways a parser can end up in a particular state.
Overlooking a state can led to a confusing error message unrelated to the actual mistake.
Optional tokens that can be empty can make it especially challenging. 

BNFGen's grammar is relatively unambiguous though, so the errors are more or less descriptive.
There still may be missed cases when the same error is displayed for two unrelated states,
but it's still better than nothing.

In the example above, it's obvious what may cause that state: a rule with weight specified but tokens
missing, like `<start> ::= 10 ;`.

So, the complete entry is:

```
grammar: IDENTIFIER DEF NUMBER SEMI

Weight number must be followed by a symbol identifier or a string.
Example of a valid rule: <start> ::= 10 <foo> | <bar> ;

```

Here's an example of a tricky state that I in fact caught while writing this post.

```
grammar: IDENTIFIER DEF NUMBER STRING NUMBER
##
## Ends in an error in state: 11.
##
## rule_rhs_part -> NUMBER rule_rhs_symbols . [ SEMI OR EOF ]
## rule_rhs_symbols -> rule_rhs_symbols . symbol [ STRING SEMI OR IDENTIFIER EOF ]
##
## The known suffix of the stack is as follows:
## NUMBER rule_rhs_symbols
##

Unterminated rule (missing semicolon somewhere?)
Example of a valid rule: <start> ::= 10 <foo> | 5 <bar> | "baz" ;
```

Looking at the token sequence, it seems like the bad input it catches is like `<foo> ::= 10 "bar" 20 "baz" ;`,
and the error message should tell the user about a missing pipe (vertical line).
However, since weight is optional, it's broader and also catches inputs like `<foo> ::= 10 "bar"  <bar> ::=  "bar"`
where the error is caused by a missing semicolon. Perhaps it could be fixed by better grammar rules.

## Generating the error handling module

That's the simplest part. Once you have the messages file ready, you can produce a module from it with:

```
menhir --compile-errors src/bnf_parser.messages src/bnf_parser.mly
```

It needs both a messages file and a parser file so that it can map those abstract state descriptions
to concrete automaton state numbers from the current parser code.

The function is very simple internally, it takes the state number that we receive from the incremental API
and maps it to a message. The complexity is all inside the Menhir library.

If you have missed any error states, it will raise a `Not_found` exception, which is why we have to handle it
in the `get_parse_error` function discussed earlier.

```ocaml
let message =
  fun s ->
    match s with
    | 0 ->
        "Invalid syntax.\n"
    ...
    | _ ->
        raise Not_found
```

## Dune integration

Dune integration is _mostly_ straightforward, except for the `--compile-errors` step made complicated by
Menhir's insistence of writing the code to stdout. I wish it had an option to save it to file (maybe I should make a patch).

Dune's `with-stdout-to` macro allows to work around it. I thought there's no way to make it track the source file,
but thanks to Armael I've learnt I just needed to specify `(deps ...)` to make it work.

You also need to remember to add `--table` to Menhir flags, and to link `menhirLib` into the executable, since incremental API
parsers are not standalone.

Here's my file:

```
; Produce the lexer from src/bnf_lexer.mll

(ocamllex bnf_lexer)

; --table enables the incremental API
; src/bnf_parser.mly will be turned into bnf_parser.ml and bnf_parser.mli
(menhir
 (flags --table)
 (modules bnf_parser))

; The target for saving menhir's stdout to bnf_parser_messages.ml
(rule
 (targets bnf_parser_messages.ml)
 (deps bnf_parser.messages bnf_parser.mly)
 (action  (with-stdout-to %{targets} (run menhir --compile-errors %{deps}))))

(executable
 (name bnfgen)
 (public_name bnfgen)

 ; Important: incremenetal API requires linking menhirLib
 (libraries menhirLib))

```

## Conclusions

The declarative approach definitely seems promising.

As of now, there are some sharp edges:

* Output to stdout makes build integration harder than it should be.
* Small changes in the grammar sometimes cause big changes in the error states.
* I couldn't get the `--compare-errors` option to work. It's is supposed to help with updating errors on grammar changes, but its syntax is unusually fragile.

Since it's quite new (based on a 2016 [paper](http://gallium.inria.fr/~fpottier/publis/fpottier-reachability.pdf), I hope it will be improved soon.
