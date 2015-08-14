(**
    Code from the Functional Unparsing paper by Olivier Danvy,
    translated from SML to OCaml

 **)

let (|+) f g x = f (g x)

let format_string p = p (fun s -> s) ""


(** Formatting patterns *)

(* String literal *)
let lit x k s = k (s ^ x)

(* \n *)
let eol k s = k (s ^ "\n")

(* %d *)
let int k s x = k (s ^ (string_of_int x))

(* %s *)
let str k s x = k (s ^ x)


(** Tests *)

let fact = format_string (lit "The original paper published in " |+ int |+ lit " used " |+ str |+ lit " rather than OCaml" |+ eol) 1997 "StandardML"

let () = print_string fact

(** These will cause compilation errors *)

(* 
    let _ = format_string (int |+ int) "foo" "bar"
    let _ = format_string (str |+ int) 99 "bar"
*)

