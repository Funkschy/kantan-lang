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
Currently there are only structs, but a interface system similar to Rusts traits is planned.

## Getting started

To compile the Kantan compiler you need to have the gcc, gpp(preprocessor, not c++) and make installed. 
After installing these just clone the repo and run `make`. <br/>
This will create a `compiler` binary in your current directory.

There currently is no way to link the resulting object files into an executable directly.<br/>
You can just run `kantan <your-file>.kan; gcc out.o -o out; rm out.o` to get an executable.

## Syntax

Just a few examples. The test suite in tests/files contains way more examples.

#### Hello World
```
import "io"

def main(): void {
    io.printf("Hello world\n");
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
import "io"

type Person struct {
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
