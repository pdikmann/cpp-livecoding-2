#pragma once

#include <string>

template< typename Interface >
struct DynamicBinding {
    std::string name;                 /// library basename
    std::string path;                 /// library path (absolute)
    void * handle = nullptr;          /// handle for dlopen/dlsym/dlclose
    Interface * object = nullptr;     /// payload
                                      //
    Interface * ( *create )( void );  /// constructor proxy
    void ( *destroy )( Interface * ); /// destructor proxy
  //
  template< typename Other >
  operator DynamicBinding< Other >() {
    DynamicBinding< Other > other;
    other.name = name;
    other.path = path;
    other.handle = handle;
    other.object = (Other*)object;
    other.create = (Other * (*)( ))create;
    other.destroy = (void (*)( Other* ))destroy;
    return other;
  }
};
