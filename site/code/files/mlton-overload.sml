(** MLton function overloading demonstration.
    This is MLton-specific and totally not standard StandardML!

    Build with: mlton -default-ann 'allowOverload true' mlton-overload.sml

 *)

signature BIZZAREOP_INT = sig
    val ^+ : int * int -> int
end

structure BizzareOpInt : BIZZAREOP_INT = struct
    val ^+ = fn (x, y) => x + y
end

signature BIZZAREOP_STRING = sig
    val ^+ : string * string -> string
end

structure BizzareOpString : BIZZAREOP_STRING = struct
    val ^+ = fn (x, y) => x ^ y
end

(* Magic begins here *)

_overload ^+ : ('a * 'a -> 'a) as BizzareOpInt.^+ and BizzareOpString.^+

(* Magic ends here *)

infix 6 ^+

val () = print ("hello " ^+ "world\n")
val () = print ((Int.toString (3 ^+ 4)) ^ "\n")
