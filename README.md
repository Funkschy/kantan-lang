# Kantan (簡単)

A Kantan compiler written in Kantan.

## Elevator pitch

Kantan is a system programming language with the goal of being a modernized version of C.
The language is primarily designed for C programmers who want the advancements in
programming language design of the last few decades in their language.

However unlike other modern system programming languages, Kantan has a relatively
C like approach to issues like memory management and types.

Memory management works like in C, but if you want to, you can also use the "new" and "delete"
keywords, which are just a thin wrapper around malloc and free (no vtables i promise).
Currently there are only structs, but a interface based system similar to Rusts traits or C++ 20's concepts is planned.

## Getting started

If you just want to try the language, simply dowload the latest [release](https://github.com/Funkschy/kantan-lang/releases) for you operating system (sorry, no windows support yet).

After downloading, just run ./kantan --help to view you options.

If you want to use the io module, you also need to create the file ~/.kantan/std/io.kan and put the following content inside:
```
extern def printf(fmt: string, ...): i32;
extern def puts(s: string): i32;
extern def putchar(c: i32): i32;
```
Don't worry, in the future the stdlib will be installed alongside the compiler, but currently there is no real stdlib, so this is good enough.

There is also a [vscode plugin](https://github.com/Funkschy/vscode-kantan), which adds support for syntax highlighting and graphical debugging.
Alternatively, install the [vim plugin](https://github.com/Funkschy/kantan.vim), which you can use to get syntax highlighting in vim/nvim.

Compiling the compiler from scratch is a bit more complicated. To compile the Kantan compiler you need to have the gcc/g++, llvm, gpp(preprocessor, not c++) and make installed. You will probably need to adjust the Makefile quite a bit.


## Syntax

Just a few examples. The test suite in test/runnable/ contains way more examples.

#### Hello World
```
import "io";

def main() {
    io.printf("Hello world\n");
}
```

#### Loops
```
import "io" as stdout; // you can also alias imports

def main() {
    for let i = 0; i < 3; i = i + 1 {
        stdout.printf("Hello from for\n");
    }

    let i = 0;
    while i < 3 {
        stdout.printf("Hello from while\n");
        i = i + 1;
    }

    let i = "It is actually possible to redeclare variables";
    stdout.printf("%s\n", i);
}
```

#### Variables
```
def main(): i32 {
    let i = 42; // Type inference
    let ptr = &i;
    let a = *ptr + 5;
    return a; // 47
}
```

#### Structs
```
import "io";

type Person struct {
    // strings are currently just pointers to null terminated char arrays, however, this will likely
    // change in the future
    name: string,
    age: i32,
    address: Address
}

type Address struct {
    city: string,
    hnr: string // number of the house
}

def make_person(name: string, city: string, age: i32): Person {
    return Person {
        name: name,
        age: age,
        address: Address { city: city, hnr: "100" }
    };
}

// the ': void' is optional
def main(): void {
    let fmt = "%s, lives in %s in house number %s\n";
    let p = make_person("Peter", "Berlin", 20);
    // copy
    let hans = p;
    hans.address.city = "Neuss";
    hans.address.hnr = "200";
    hans.name = "Hans";
    io.printf(fmt, p.name, p.address.city, p.address.hnr);
    io.printf(fmt, hans.name, hans.address.city, hans.address.hnr);
}
```


#### Enums
```
import "io";

type MyEnum enum {
    First, Second, Third
}

// Enums can also have a start value, after which they just continue to count up
type MyOtherEnum enum {
    First = 2, Second, Third
}

def main() {
    io.printf("%d\n", MyEnum.Second); // prints 1
    io.printf("%d\n", MyOtherEnum.Second); // prints 3

    let same_len = MyEnum.len == MyOtherEnum.len; // true
}
```
