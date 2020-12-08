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
Currently there are only structs, unions and enums, but a trait based system similar to C++ 20's
concepts is planned.

## Getting started

If you just want to try the language, simply download the latest [release](https://github.com/Funkschy/kantan-lang/releases) for you operating system (sorry, no windows support yet).

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

## Building from scratch
Compiling the compiler from scratch is a bit more complicated. To compile the Kantan compiler you need to have the gcc/g++, llvm, gpp(preprocessor, not c++) and make installed. After installing those, you need to execute
```make
make self KANTAN_STABLE="path to your stable kantan" LLVM_PATH="path to your llvm-10 installation"
```
This will produce a proper Kantan executable called 'self' and a debug build called 'compiler'.
You probably want to do
```bash
rm compiler
mv self kantan
```

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
    import "io" as _; // it is also possible to perform a non qualified import

    let fmt = "%s, lives in %s in house number %s\n";
    let p = make_person("Peter", "Berlin", 20);
    // copy
    let hans = p;
    hans.address.city = "Neuss";
    hans.address.hnr = "200";
    hans.name = "Hans";

    // printf can now be used without the "io."
    printf(fmt, p.name, p.address.city, p.address.hnr);
    printf(fmt, hans.name, hans.address.city, hans.address.hnr);
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


#### Methods
```
import "io";

type Struct struct {
    name: string
}

// the same file can have multiple methods with the same name. As long as they have different
// receiver parameters
def (s: Struct) method(i: i32) {
    io.printf("%s\n", s.name);
    io.printf("%d\n", i);
}

type Other struct {}

def (s: *Other) method(i: i32) {
    io.printf("%d\n", i);
}

def constructor(): Struct {
    io.printf("...\n");
    return Struct {name: "constructed"};
}

def main() {
    Struct{name: "hello"}.method(1);
    constructor().method(2);
    let s = constructor();
    s.method(3);

    // variables are upgraded to pointers automatically
    // they will not be automatically dereferenced though, since that may trigger a segfault
    Other{}.method(4);
}
```


#### Memory Management
```
// the canonical way to free memory is by using the defer statement.
// defer moves the statement after it at the end of the scope.
// it will be executed when:
//   1) the scope ends '}'
//   2) a return statement is executed
//   3) if the defer was in a loop, when break/continue is executed
type MyStruct struct {
    buffer: *usize // needs to be freed
}

def my_struct(): MyStruct {
    return MyStruct {
        buffer: new 42 // allocate a word on the heap
    };
}

def (ms: *MyStruct) free() {
    delete ms.buffer;
}

def main() {
    let ms = my_struct();
    defer ms.free();

    if 1 == 1 {
        // ms.free() will be called here
        return;
    }

    // it would have been freed here if we didn't return inside the if
}
```
