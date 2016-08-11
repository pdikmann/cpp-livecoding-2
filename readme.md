# Hint

This is WIP as `gcc 5` might have changed linking behavior.
In particular, it might now be necessary to pass the `--export-dynamic` flag to the linker by adding `PROJECT_LDFLAGS += -Wl,--export-dynamic` to `config.make`. This might cause errors with the `clang` compiler, so further investigation is due.

# Getting started

- run `compile-libs.sh -f`
- in another terminal, `make` and `make run` the application
- edit `src/DynamicLibs/Libs/Stuff.cpp` - e.g. the `draw` function - and save it to see *live updates!*

# Pop the hood

check out `diagrams/overview.svg` to understand the basic architecture:

- application and libraries communicate via the interfaces found in `src/DynamicLibs/Interfaces`.
- the libraries implement these interfaces (and add extra stuff as needed).
- library source files found in `src/DynamicLibs/Libs` are automatically compiled into
  the `bin/data` folder by the `compile-libs.sh` shell script.
- that same script signals the application to reload the libraries on successful compilation.
  the `DynamicLibs` class takes care of this for you.

# DIY

peek at `diagrams/diy.svg` to add your own libraries:

- write a suitable interface header file in `src/DynamicLibs/Interfaces`
- add a member variable of the interface type to the `MyLibs` class found in `src/DynamicLibs` 
  and initialize it inside the `MyLibs::initLibs` function.
- write the implementation in `src/DynamicLibs/Libs`; implement the interface 
  and provide `create` and `destroy` functions with `extern "C"` linkage.

# Details

- the `compile-libs.sh` script assumes either a 64-bit linux running `gcc` (tested with 4.9.2) or mac os x (tested on 10.11) running `clang`.
- `src/DynamicLibs/Libs` needs to be excluded from compilation inside `config.make`
- to make this run on Linux, you will need to recompile oF with the `-fPIC` compiler flag;
  apply the `0001-add-fPIC-flag-to-compile.core.mk.patch` to the openFrameworks-repository to set the flag,
  then `make clean` and `make` your openFrameworks (from inside `libs/openFrameworksCompiled/project`)
  

