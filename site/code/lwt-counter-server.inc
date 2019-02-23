{% set date 2019 February 23 %}

<h1>Simple multi-client server in OCaml with Lwt</h1>

<p>Lwt is a popular cooperative multitasking library for OCaml.</p>
<p>There are a number of Lwt tutorials and examples around, but for some reason I couldn't find any that would explain
   how to write a server that serves multiple connections concurrently (as opposed to sending a reply and terminating
   the connection, like HTTP servers do).</p>
<p>While it seems obvious once you figured it out, it took me a while to figure it out, so I wrote a simple example
   and a walkthrough.</p>
<p>The server from the example allows clients to increment a shared counter and read its value.</p>

<p>Download the file: <a href="/code/files/counter-server.ml">counter-server.ml</a>. </p>

<h3>Prerequisites</h3>

<p>We will need the Lwt package and also the Logs package for logging.</p>
<pre>
opam install lwt logs
</pre>

<h3>Code walkthrough</h3>

<p>We start with opening the Lwt module so we have easy access to bind, return, and other essential functions.</p>
<pre>
open Lwt
</pre>

<p>The server will keep a shared mutable counter. Clients will be able to read or increment it.</p>
<pre>
let counter = ref 0
</pre>

<p>We'll make the server listen on 127.0.0.1:9000.</p>
<pre>
let listen_address = Unix.inet_addr_loopback
let port = 9000
let backlog = 10
</pre>

<p>We accepts two commands from clients: <tt>read</tt>, that returns the current value
   of the counter; and <tt>inc</tt>, that increments it and sends a confirmation.
   Any other command will trigger <tt>Unknown command</tt> reply.</p>
<p>All context switches occur between Lwt binds, so we don't need to care about
   locking the counter explicitly.</p>
<pre>
let handle_message msg =
    match msg with
    | "read" -&gt; string_of_int !counter
    | "inc"  -&gt; counter := !counter + 1; "Counter has been incremented"
    | _      -&gt; "Unknown command"
</pre>

<p>Threads that serve client connections need to read from the socket,
   process commands, and then read again. We'll make it a tail-recursive loop,
   essentially <tt>read &gt;&gt;= process &gt;&gt;= write &gt;&gt;= read</tt>.</p>
<p>We need to take care of closed connections though. Reading from a closed connection
   will raise an <tt>End_of_file</tt> exception. We could handle it, or we can use the
   <tt>Lwt_io.read_line_opt : input_channel -&gt; string option Lwt.t</tt> function that returns <tt>None</tt>
   rather than raising an exception if the channel is closed.</p>
<p>Then we can terminate the thread  if it returns <tt>None</tt>, and proceed otherwise.</p>
<pre>
let rec handle_connection ic oc () =
    Lwt_io.read_line_opt ic >>=
    (fun msg ->
        match msg with
        | Some msg -> 
            let reply = handle_message msg in
            Lwt_io.write_line oc reply >>= handle_connection ic oc
        | None -> Logs_lwt.info (fun m -> m "Connection closed") >>= return)
</pre>

<p>When a connection comes in, we need to start a <tt>handle_connection</tt> thread.
   We don't care about its result, but we should do something about errors that may occur,
   so we start it with <tt>Lwt.on_failure</tt> function that also takes a callback
   that is executed when exception occurs. We'll just make it log the exceptions.</p>
<pre>
let accept_connection conn =
    let fd, _ = conn in
    let ic = Lwt_io.of_fd Lwt_io.Input fd in
    let oc = Lwt_io.of_fd Lwt_io.Output fd in
    Lwt.on_failure (handle_connection ic oc ()) (fun e -> Logs.err (fun m -> m "%s" (Printexc.to_string e) ));
    Logs_lwt.info (fun m -> m "New connection") >>= return
</pre>

<p>We also need a thread that accepts connections in a loop. We'll store the socket
   in a closure. When a client connects, it will pass the file descriptor to the <tt>accept_connection</tt>
   function will start a thread that serves that client.</p>
<pre>
let create_server sock =
    let rec serve () =
        Lwt_unix.accept sock &gt;&gt;= accept_connection &gt;&gt;= serve
    in serve
</pre>

<p>Socket setup is straightforward.</p>
<pre>
let create_socket () =
    let open Lwt_unix in
    let sock = socket PF_INET SOCK_STREAM 0 in
    bind sock @@ ADDR_INET(listen_address, port);
    listen sock backlog;
    sock
</pre>
<p>
Note: <code>Lwt_unix.bind</code> return type is <code>unit Lwt.t</code> rather than <code>unit</code>,
so this is a bit sloppy and will cause a compiler warning.
</p>

<p>Now we have everything we need. All we need to start working is to start the server thread with
   <tt>Lwt_main.run</tt>.</p>
<pre>
let () =
    let sock = create_socket () in
    let serve = create_server sock in
    Lwt_main.run @@ serve ()
</pre>

<p>Now we can build it with <kbd>ocamlfind ocamlopt -package lwt,lwt.unix,logs,logs.lwt -linkpkg -o counter-server ./counter-server.ml</kbd>,
   start the executable, and start a few telnet sessions to 127.0.0.1:9000 to test it.</p>
<p>Example of a telnet session:</p>
<pre>
$ telnet localhost 9000
Trying 127.0.0.1...                                                                                                                                                                                                Connected to localhost.                                                                                                                                                                                            
Escape character is '^]'.
read
0
inc
Counter has been incremented
read
1
</pre>
