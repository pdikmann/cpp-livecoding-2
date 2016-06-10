# Livecoding using C++ and openFrameworks

This article describes an approach and a setup to integrate live coding - a fast, iterative style of programming - into C++ and the openFrameworks environment. While the environment is traditionally hampered by the constraints of batch programming and long compilation times, this approach significantly reduces the time between writing the code and seeing the result of its execution, facilitating learning and experimentation and leading to increased productivity.

## The TL;DR

Instead of compiling all our code into a single monolithic app, we are separating the different components into a set of dynamic, shared libraries that can be compiled separatedly and (re-)loaded into the main app at runtime. We're using a simple shell script that detects changes to our sourcefiles, tries to compile them and signals the main app on success. The main app then uses the (standard BSD and C libraries) `<dlfcn.h>` and `<csignal>` headers  to work the magic.

You can read up on the basics of dynamic libraries in the following paragraphs, or jump straight to the conclusions on [how to use the example code](#usage) and [how to adapt it to your needs](#adapt).

## How dynamic libraries work

Running an executable is straightforward for the operating system: it maps a piece of physical memory to a new instance of virtual memory, copies the executable into it, and starts running the code from the top (the beginning of virtual memory). If the executable allocates memory to store and retrieve values, it is taken from this same instance of virtual memory. When the machine code of the executable references variables and symbols (like the location of particular functions within the executable), these hard-coded references are also resolved in relation to the virtual memory.

This means that the executable never has to worry about the place it occupies in physical memory: all references are mapped to the virtual memory, with the executable sitting comfortably right at the top, or address 0. (Yes, it is slightly more complicated than that, but humour us for the sake of this explanation.) Most importantly, this means that the compiler building this executable, when writing the machine code, can simply assume that everything is stored in relation to address 0 and just use fixed, absolute numbers to refer to things. 

Loading a dynamic library at runtime poses a challenge, then: since the library code is going to be placed _after_ the executable, it can no longer assume that it sits at the top of virtual memory. In effect, the compiler of the library can no longer write down fixed addresses for all the references, and instead has to produce what is called [position-independent code](https://en.wikipedia.org/wiki/Position-independent_code).

Position-independent code makes no assumptions about its place in virtual memory. Instead, all the references to symbols and variables are redirected to use a so-called 'global offset table', conveniently located at a fixed spot in the beginning of the code. This allows the executable loading the library to modify this table - and adjust the addresses to match their actual location in virtual memory. This mechanism is what allows dynamic libraries to function - and for the same library to be used by completely different executables. It literally puts the 'shared' in 'shared library'.

In addition, it is possible to order the compiler to produce position-independent code for the original executable. The compiler will then write a similar global offset table into the executable as well, storing all references used in the executable in this table. Although it is not strictly necessary for the executable to function - since all its code starts at address 0 anyway - it allows for another neat trick: it exposes the symbols and variables of the executable to any dynamically loaded library. Using the executable's global offset table, the dynamic library can look up and use any of the symbols that the executable brings to the table.

Using these mechanisms, we can have our openFrameworks application hot-swap its components at runtime, modifying their behavior without having to restart or recompile. Even better: it allows us to use the entirety of openFrameworks' functionality from inside our dynamic libraries - without incurring the penalty of long compile times.

## Interface and Linkage

Two more things need to be in place to create and use a dynamic library: a common interface and the proper linkage.

### Interface

An interface - that is: a contract describing the calling conventions of a piece of software - is needed to assure the mutual cooperation between the application using the library and the library itself. Although the library is intentionally not part of the apps compilation unit, the compiler still needs to know about the functions and symbols made available by the library. On the other hand, the library needs to be aware of these obligations and ensure that all of the available functions are implemented and none are missing.

In our case, the interfaces used are so-called [abstract classes](http://devdocs.io/cpp/language/abstract_class) and can be found in the `src/DynamicLibs/Interfaces` directory. They look like this:

``` {.cpp .numberLines startFrom="4"}
class GenericInterface
{
public:
    virtual void update( ) = 0;
    virtual void draw( ) = 0;
    ...
```

The pure virtual functions assure that the compiler of our main application will not look for their actual implementation when compiling the openFrameworks executable. The dynamic library then implements a derived class of this interface, supplying the functionality. 

### Linkage

[Linkage](https://en.wikipedia.org/wiki/Linkage_%28software%29) is the word describing the way that a compiler (and the linker) hooks up all the symbols defined in the source code (like function and variable names) when writing an executable. When compiling multiple files into a single application, the compiler has to take care to keep track of identical names, making sure that they refer to the same things even when they occur in different files. To achieve this, the compiler writes them into [symbol tables](https://en.wikipedia.org/wiki/Symbol_table) (like the 'global offset table' mentioned before) for later reference.

To accomodate the complex features of C++ (like Polymorphism, where multiple derived classes overwrite the same function name with different implementations), the compiler will have to 'mangle' these names - adding additional letters and numbers to make them distinct - in effect producing unpredictable, garbled entries in the symbol tables.

Fortunately for us, the C language is more straightforward and does not require the compiler to mangle symbol names. Since many popular C++-compilers are also capable of compiling C, it is possible to expose certain names, unmangled, to the outside by declaring their linkage as `extern "C"`. Examples of this can be found in the `src/DynamicLibs/Libs/Stuff.cpp` file:

``` {.cpp .numberLines startFrom="55"}
extern "C" {
    Stuff * create( ) {
        return new Stuff;
    }
    void destroy( Stuff * obj ) {
        delete ( obj );
    }
    ...
```

Writing, loading and using a dynamic library in C++ poses an additional challenge: that of linkage. Linkage is the 

Using a dynamic library requires our executable to know two things: the capabilities of the library, and the actual names of the functions used to interact with it. Both of these pose different problems: This lays down two restrictions:

While the openFrameworks app and the library both need to agree on practical details like function signatures and return types, it is not possible for one of them to include the other during compilation. Including the library source files in the same compilation unit as the rest of the app would include its functionality into our main executable, unable to be updated without recompiling the entire app. On the other hand, including the entirety of our openFrameworks app into the compilation unit of the library would defeat the purpose of this exercise by increasing compilation times. [this is bs]
The solution is to use interface classes defining the calling conventions of the components. These interfaces are purely virtual, meaning: they don't provide any implementation details, but establish the class signature. [imprecise].

The capabilities of our components are laid down in the sourcecode of the component. However, we cannot have our executable actually compile 

## Running the code {#usage}

On Mac OS X, you're already good to go. Clone this repo into the `openFrameworks/apps/yourapps/cpp-livecoding-2` directory, `make` and `make run`. Then, in a separate terminal, run `./compile-libs.sh` to start the continuous background compilation. At this point, you can start editing the `src/DynamicLibs/Libs/Stuff.cpp` source file and see any saved changes being reflected in your app in real-time. If things don't show up, check the output of `./compile-libs.sh` for compilation errors.

On Linux, the situation is different: you will probably need to recompile your openFrameworks to use position-independent code. This is done by default on Mac OS for reasons of security and compliance with x86-64 architecture (see [this StackOverflow question](http://stackoverflow.com/questions/7216244/why-is-fpic-absolutely-necessary-on-64-and-not-on-32bit-platforms#7217082)); the Makefile for Linux does not make the same assumptions (e.g. because you might not run a 64bit system). But it can be arranged: assuming you have cloned openFrameworks from its github repo, you can apply the provided patch and recompile:

```
cp 0001-add-fPIC-flag-to-compile.core.mk.patch ../../..
cd ../../..
git apply 0001-add-fPIC-flag-to-compile.core.mk.patch
cd libs/openFrameworksCompiled/project   
make clean
make
```

After that, continue as described above for Mac OS X.

## Rolling your own {#adapt}

The architecture of this contraption is best summed up in the following picture:

![Architecture overview](../diagrams/overview.png)

All user-serviceable parts are contained within the `MyLibs` class and the `src/DynamicLibs/Libs` and `/Interfaces` directories. 

To add a component for use as a dynamic library, first create a fitting interface. Looking at the example headers in `src/DynamicLibs/Interfaces`, you'll see that these are purely virtual, meaning: all functions are declared `virtual` and assigned `= 0`. This turns the interface class into an abstract class, preventing it from being instantiated and requiring any derived classes to provide implementations to all declared functions - which fits the intended use very nicely.

``` {.cpp .numberLines startFrom="4"}
class GenericInterface
{
public:
    virtual void update( ) = 0;
    virtual void draw( ) = 0;
    ...
```

Afterwards, take the three exemplary steps laid out in the `MyLibs.h` file:
`#include` the new interface, add another public member variable (holding a pointer to the interface type) and set it up with a corresponding line in the `initLibs` function. 

Last but not least, write the library itself! This comes down to implementing all the functionality promised in the interface class, plus 4 important extras: a set of functions called `create`, `destroy`, `getData` and `setData`, all of them declared with `extern "C"` linkage. Scoop their signatures from the example in `Stuff.cpp`:

``` {.cpp .numberLines startFrom="55"}
extern "C" {
    Stuff * create( ) { ...  }
    void destroy( Stuff * obj ) { ...  }
    void* getData( Stuff* obj ) { ... }
    void setData( Stuff* obj, void* data ) { ... }
}
```

The first two are mandatory and are used to instantiate (and destroy) the new component. Since the main app has no detailed knowledge of the component, it is unable to instatiate it without help from the library; and since the linkage is constrained to C language constructs, these two functions have to do the leg work of calling the respective constructor & destructor.

The other two are optional and can be used to persist data across reloads of the component. By storing the components' data in a dedicated struct, the main application can take a hold of it, swap out the library for a freshly compiled new version, and put the data back in place afterwards. This works fine for simple structs, but will require additional legwork for more complex / nested structures.

## Wrapping up

And that's it! I hope you
