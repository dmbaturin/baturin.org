# Hello from a compiler-free x86-64 world

<time id="last-modified">2018-04-07</time>
<tags>programming, assembly</p>

<p id="summary">
I've stumbled upon <a href="https://blogs.oracle.com/ksplice/hello-from-a-libc-free-world-part-1">two</a>
<a href="https://blogs.oracle.com/ksplice/hello-from-a-libc-free-world-part-2">posts</a> where Jessica McKellar
demonstrates how to make a C program on Linux without using libc. They were written in 2010 when
32-bit x86 was far from extinct though, and use the old 32-bit ABI—a problem most examples
of low level programming on UNIX-like systems suffer from, even if it's not their fault.
</p>

I'll take it to the next level and show you how to live wild and free of libc, compilers, and common sense,
all in the native x86-64 way. Both assembly programming and ABI conventions are of little practical use whenever
you are not writing a compiler, a kernel, or a runtime library, but I like the premise of those posts:
making a program whose every instruction can be easily explained.

This is more of post about x86-64 than assembly of ELF, so I assume some familiarity with the x86 assembly language, although I tried
to annotate the examples with pseudocode and avoid some common tricks to make it more beginner-friendly. If you want to learn about x86 assembly,
there's an excellent (even if old) [Programming from the Ground Up](https://savannah.nongnu.org/projects/pgubook/) book
by Jonathan Bartlett.

We'll use the GNU assembler and linker for this example.

## The task

Simply printing a &ldquo;hello world&rdquo; line on screen is too trivial. I wanted something that can
demonstrate more than one concept, preferrably most of the important concepts, but still isn't overly complex
for a first example.

We'll write a program that accepts unlimited number of names as command line arguments and prints greetings for
them all:

```
$ greet Dennis Brian Ken
Hello Dennis!
Hello Brian!
Hello Ken!
```

To do this, we need to know at least:

* How to find command line arguments in the process' memory
* How to use system calls
* How to implement and call functions

While in a pure assembly program one can do many things in an ad hoc way and still end up with a working program,
we'll make our implementation more or less compliant with the system's conventions.

## The conventions

System ABI is not only specific to the OS, but also to the CPU architecture. Most UNIX-like systems use a variant of the System V ABI,
that is divided into a part applicable to all systems (it describes the ELF file format among other things) and architecture-specific
supplements.

The ABI of Linux on x86-64 is defined by [x86-64 Architecture Processor Supplement](http://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf).

The entry point that `ld` is looking for is `_start`. To prevent `ld` from linking the prologue required for linking to libc and other fancy
things, we need to use `ld -nostdlib`.

## System call conventions

The old convention used to use interrupt `0x80` for invoking system calls. The x86-64 architecture introduced a new `syscall` instruction
specifically for this purpose. The convention for passing system call numbers and arguments also has changed.

Let's read the section A.2.1. It says that:

* System call number is passed in the `rax` register.
* System call arguments should be in registers `rdi`, `rsi`, `rdx`, `r10`, `r9`, `r8`.
* System call arguments are never passed on stack anymore (system calls are limited to six arguments).
* System calls destroy registers `rcx` and `r11`.
* The result is in `rax`, negative values indicate that it's an `errno`.

You can find system call numbers in `/usr/include/asm/unistd_64.h`. They are technically subject to change, but I don't think they ever changed.

With this knowledge we can write a minimal program that exits correctly:

```x86
.text

_start:
    mov $60, %rax # 60 = exit
    mov $0, %rdi
    syscall
.global _start
```

```
$ as -o dummy.o ./dummy.s && ld -nostdlib -o dummy ./dummy.o
$ ./dummy && echo $?
0
```

## Where are the arguments?

To get command line arguments, we need to know where they are in the program memory. Section 3.4.1 of the ABI document (Initial Stack and Register State)
has figure 3.9, which is a table describing initial stack layout. It says that at the top of the stack is argument count, followed by pointers
to argument strings.

Let's try something:

```x86
.text

_start:
    pop %rdi
    mov $60, %rax
    syscall
.global _start
```
This program pops the supposed argument count off the top of the stack and uses it as an argument to the exit system call,
so its exit code will be the number of arguments we gave it.

```
$ as -o argc.o ./argc.s && ld -nostdlib -o argc ./argc.o
$ ./argc || echo $?
1
$ ./argc foo || echo $?
2
$ ./argc foo bar baz quux || echo $?
5
```

So far so good, it appears to work as expected.

System call arguments come in the same order as we see them in the API reference for C.
We can make a couple of macros for `exit` and `write` for future use right away:

```x86
.macro write filedescr bufptr length
    mov $1, %rax # syscall = __NR_write
    mov \filedescr, %rdi
    mov \bufptr, %rsi
    mov \length, %rdx
    syscall
.endm

.macro exit status
    mov $60, %rax # syscall = __NR_exit
    mov \status, %rdi
    syscall
.endm
```

## Function call conventions

Remember that we are trying to live free of the standard library? For writing characters to stdout
we can use the `write` system call, but it needs to know the string length, and we do not have a ready-made
function for this. We'll have to make one ourselves, and for that we need to learn about function calling conventions.

One important thing is register ownership. If you call a function and it mangles your data, it's not a nice situations,
so all calling conventions specify which registers must be preserved by the callee, so that if you can be sure that
calling a function will not destroy them. In our x86-64 convention these are `rbx`, `rbp`, and `r12` to `r15`.

We can make a couple of macros that save those register on stack and then restore them to save time:

```x86
.macro save_registers
    push %rbx
    push %rbp
    push %r12
    push %r13
    push %r14
    push %r15
.endm

.macro restore_registers
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbp
    pop %rbx
.endm
```
Of course we could use a more granular approach with fewer instructions, but then we'd have to keep track of the registers we used.
I'd rather leave that to compilers since they don't get tired of tracking registers halfway through.

The other part is argument passing. It its core it's very similar to the system call convention, though it's more extensive and
obviously does allow passing arguments on stack. Since we only need pointers and less than 64-bit integers for our task, we can
pass all arguments in registers, the sequence is: `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9` as the section 3.2.3 suggests.
The return value should be in `rax`.

Let's write a simple `strlen` function. Strings are null-terminated, so to find out the length of a string we can take characters
from it one by one, compare it to zero, and increment the accumulator if it's not a null byte. The accumulator will also serve
as string index.

```x86
strlen:
    save_registers

    # r12 will be used as an accumulator and string index,
    # which is fine since maximum string index is its length
    mov $0, %r12

    strlen_loop:
        # r13b = *rdi[r12]
        mov (%rdi, %r12, 1), %r13b

        # if(r13b == 0) goto strlen_return
        cmp $0, %r13b
        je strlen_return

        inc %r12
        jmp strlen_loop

    strlen_return:
        mov %r12, %rax # save return value in rax
        restore_registers
        ret
.type strlen, @function
```

We'll also need a `puts` function, but now that we have `strlen`, it will be simple.

## The complete program

Now we are ready to put the program together.

```x86

# The ABI demands that registers rbx, rbp, and r12-r15
# must be preserved by the callee, so we push them to stack
# when a function is called and pop them back before returning
# back to the caller
.macro save_registers
    push %rbx
    push %rbp
    push %r12
    push %r13
    push %r14
    push %r15
.endm

.macro restore_registers
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbp
    pop %rbx
.endm

# Macros for system calls
# The numbers can be found in /usr/include/asm/unistd_64.h on Linux

.macro write filedescr bufptr length
    mov $1, %rax # syscall = __NR_write
    mov \filedescr, %rdi
    mov \bufptr, %rsi
    mov \length, %rdx
    syscall
.endm

.macro exit status
    mov $60, %rax # syscall = __NR_exit
    mov \status, %rdi
    syscall
.endm

.section .rodata

hello_begin:
    .ascii "Hello \0"

hello_end:
    .ascii "!\n\0"

.section .text

strlen: # (rdi = buf)
    save_registers

    # r12 will be used as an accumulator and string index,
    # which is fine since maximum string index is its length
    mov $0, %r12

    strlen_loop:
        # r13b = buf[r12]
        mov (%rdi, %r12, 1), %r13b

        # if(r13b == 0) goto strlen_return
        cmp $0, %r13b
        je strlen_return

        inc %r12
        jmp strlen_loop

    strlen_return:
        mov %r12, %rax # save the return value in rax
        restore_registers
        ret
.type strlen, @function

puts: # (rdi = filedescr, rsi = buf)
    save_registers

    # Save original arguments
    mov %rdi, %r12 # r12 = filedescr
    mov %rsi, %r13 # r13 = buf

    # r13 = strlen(buf)
    mov %r13, %rdi
    call strlen

    mov %rax, %r14 # r14 = strlen(buf)

    write %r12 %r13 %r14

    restore_registers
    ret
.type puts, @function

_start:
    # argc is the first thing on the stack when a process is launched
    # load it into r12 for using it as a loop counter
    pop %r12 # r12 = argc

    # argc is followed by pointers to argument strings
    # the first argument is the program name, for the purpose of this program
    # we do not need it

    # *argv[0] is the program name, ignore it
    pop %r13 # r13 = argv[0]
    dec %r12 # argc--

    # put the first real argument in r13
    pop %r13 # r13 = argv[1]

    main_loop:
        # if(argc == 0) goto exit
        cmp $0, %r12
        je exit

        # puts(hello_begin)
        mov $1, %rdi # rdi = STDOUT
        mov $hello_begin, %rsi
        call puts

        mov $1, %rdi # rdi = STDOUT
        mov %r13, %rsi
        call puts

        mov $1, %rdi # rdi = STDOUT
        mov $hello_end, %rsi
        call puts

        pop %r13 # fetch next argument
        dec %r12
        jmp main_loop

exit:
    exit $0

.global _start
```

Let's try greeting the ABI document authors with it:

```
$ as -o greet.o ./greet.s  && ld -nostdlib -o greet ./greet.o
$ ./greet Michael Jan Andreas Mark
Hello Michael!
Hello Jan!
Hello Andreas!
Hello Mark!

```

## Porting to FreeBSD

An interesting fact is that a number of systems adopted the same ABI on x86-64 that was first introduced in Linux,
including FreeBSD. I decided to test the portability of this program and the result is a partial success.

If you change the system call numbers to 1 for `exit` and 4 for `write` (they are in `/usr/include/sys/syscall.h`),
the program assembles fine and works for some, but has an issue that makes it segfault on some inputs, which I
didn't have time to track down:

```
$ ./greet Marshall Bill Keith
Hello Marshall!
Hello Bill!
Hello Keith!

$ ./greet foo bar baz
Hello ./greet!
Hello foo!
Hello bar!
Hello baz!
Hello Segmentation fault (core dumped)

```

If you find what the problem is, please let me know.
