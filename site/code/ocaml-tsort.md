# User-friendly topological sort in OCaml

Topological sorting is a relatively common task, but it's also relatively hard to generalize to for everyone's needs
_and_ keep it user-friendly. Wherever there are dependencies between tasks, whether it's a build automation tool like Make
or something else, you need to sort them, and you also need to tell users what they are doing wrong.

We have many libraries that are great at generic graph manipulation but require developers
to convert their data to a format that is designed to be able to be usable with any possible graph algorithm.
At the same time they may be unable to represent data with non-existent dependencies, or produce an easily printable
cyclic dependency error.

We also have programs that either produce nondescript error messages or just resolve cycles as they please, like systemd.

Which is why I made a module that you can easily steal and adapt for your project. It's licensed under Creative Commons Zero
(i.e. public domain equivalent), you can get it from here: [tsort.ml](/code/files/tsort.ml).

I tried to make it as easy to use as the classic UNIX `tsort(1)`. Here's a usage example:

```
utop # sort [("foundation", []); ("walls", ["foundation"]); ("roof", ["walls"])] ;;
- : (string list, string) result = Ok ["foundation"; "walls"; "roof"]
```

It also produces distinct errors for non-existent and circular dependencies:

```
utop # sort [("foundation", ["roof"]); ("walls", ["foundation"]); ("roof", ["walls"])] ;;
- : (string list, string) result =
Error "Found a circular dependency between nodes: roof foundation walls"


utop # sort [("foundation", ["construction permit"]); ("walls", ["foundation"]); ("roof", ["walls"])] ;;
- : (string list, string) result =
Error "Found dependencies on non-existent nodes: construction permit"
```

It uses the classic Kahn's algorithm from 1962. The input is a `(string * string list) list`, but internally
I convert it to a `Hashtbl` for ease of manipulation.

The idea is simple:
* Create an empty list for the sorted nodes.
* Find a node that has no dependencies, remove it from the graph and move it to that list.
* Repeat until the graph is empty.
* If there are nodes that can't be removed, there's a dependency cycle.

That's assuming there's actually a graph. When the graph is constructed from user-defined statements like `Requires = foo.service` or `foo: bar.c baz.c`,
the natural representation is an associative array of node names as keys and their dependency lists as values.
And there's no guarantee that `foo.service` or `bar.c` actually actually exists in the set of its keys.

However, If we sacrifice efficiency, we can work with that format directly. First make a helper that find nodes whose dependency lists are empty:

```
let find_isolated_nodes hash =
  let aux id deps acc =
    match deps with
    | [] -> id :: acc
    | _  -> acc
  in Hashtbl.fold aux hash []
```

Then a helper for removing all nodes with given names from the hash (those names are later moved to the list of sorted nodes):

```
let remove_nodes nodes hash =
  List.iter (Hashtbl.remove hash) nodes
```

And then we also need a way to delete a node name from every dependency list:

```
let remove_dependency hash dep =
  let aux dep hash id =
    let deps = Hashtbl.find hash id in
    let deps =
      if List.exists ((=) dep) deps then
        CCList.remove ~eq:(=) ~key:dep deps
      else deps
    in
    begin
      Hashtbl.remove hash id;
      Hashtbl.add hash id deps
    end
  in
  let ids = CCHashtbl.keys_list hash in
  List.iter (aux dep hash) ids
```

We make an assumption that every node mentioned in dependency lists exists in the hashtable keys,
but if the graph turns out to be unsortable, we need to consider that possibility and have a function
for checking it ready:

```
let find_nonexistent_nodes nodes =
  let keys = List.fold_left (fun acc (k, _) -> k :: acc) [] nodes in
  let rec find_aux ns nonexistent =
    match ns with
    | n :: ns ->
      if List.exists ((=) n) keys then find_aux ns nonexistent
      else find_aux ns (n :: nonexistent)
    | [] -> nonexistent
  in
  let nonexistent = List.fold_left (fun acc (_, vs) -> List.append acc (find_aux vs [])) [] nodes in
  CCList.uniq ~eq:(=) nonexistent
```

Now we can wrap it up. First we find nodes that have no dependencies at all (&ldquo;base nodes&rdquo;), remove them from the hash,
and create an initial list of sorted nodes from them. Then we start a loop that has three parameters: a list of nodes
that are safe to remove from dependencies (`deps`), a graph hash, and an accumulator list for storing sorted nodes.

After ininial identification and removal of base nodes, the list of nodes safe to remove and the list of already sorted nodes
are the same. Then it gets funny because the recursion is not structural and that list can either shrink or grow.

If there are more nodes become isolated after removal of already sorted dependencies, it's a good thing because they will
be removed at the next step. If everything is fine, then eventually both the list of nodes safe to remove from dependency lists
and the hash itself become empty.

If the list of nodes that are safe to remove becomes empty but the hash still has items, then there are two possibilities:
something depends on a node that is not in the hash key set, or two nodes become on each other. Two tell those conditions
apart, we run the check for non-existent nodes on the original data. If none are found, then there's actually a cycle.

```
let sort nodes =
  let rec sorting_loop deps hash acc =
    match deps with
    | [] -> acc
    | dep :: deps ->
      let () = remove_dependency hash dep in
      let isolated_nodes = find_isolated_nodes hash in
      let () = remove_nodes isolated_nodes hash in
      sorting_loop (List.append deps isolated_nodes) hash (List.append acc isolated_nodes)
  in
  let nodes_hash = CCHashtbl.of_list nodes in
  (* Find and remove nodes that have no dependencies *)
  let base_nodes = find_isolated_nodes nodes_hash in 
  let () = remove_nodes base_nodes nodes_hash in
  let sorted_node_ids = sorting_loop base_nodes nodes_hash [] in
  let sorted_node_ids = List.append base_nodes sorted_node_ids in
  let remaining_ids = CCHashtbl.keys_list nodes_hash in
  match remaining_ids with
  | [] -> Ok sorted_node_ids
  | _  ->
    let nonexistent_nodes = find_nonexistent_nodes nodes in
    begin
      match nonexistent_nodes with
      | [] -> Error (Printf.sprintf "Found a circular dependency between nodes: %s" (String.concat " " remaining_ids))
      | _  -> Error (Printf.sprintf "Found dependencies on non-existent nodes: %s" (String.concat " " nonexistent_nodes))
    end
```
