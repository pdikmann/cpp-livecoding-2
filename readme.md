# Getting started

- `make` and `make run` the application
- in another terminal, run `compile-libs.sh`
- edit `src/DynamicLibs/Libraries/Stuff.cpp` - e.g. the `draw` function - and save it to see *live updates!*

# Pop the hood

check out `diagrams/overview.svg` to understand the basic architecture:

- application and libraries communicate via the interfaces found in `src/DynamicLibs/Interfaces`.
- the libraries implement these interfaces (and add extra stuff as needed).
- library source files found in `src/DynamicLibs/Libraries` are automatically compiled into
  the `bin/data` folder by the `compile-libs.sh` shell script.
- that same script signals the application to reload the libraries on successful compilation.
  the `DynamicLibs` class takes care of this for you.

# DIY

peek at `diagrams/diy.svg` to add your own libraries:

- write a suitable interface header file in `src/DynamicLibs/Interfaces`
- add a member variable of the interface type to the `MyLibs` class found in `src/DynamicLibs` 
  and initialize it inside the `MyLibs::initLibs` function.
- write the implementation in `src/DynamicLibs/Libraries`; implement the interface 
  and provide `create` and `destroy` functions with `extern "C"` linkage.

# Details

- `src/DynamicLibs/Libraries` is excluded from compilation inside `config.make`
- to make this run on Linux, you will need to recompile oF with the `-fPIC` compiler flag.
  

