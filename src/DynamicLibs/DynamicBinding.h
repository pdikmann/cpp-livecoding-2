#pragma once

#include <string>

template < typename Interface >
struct DynamicBinding
{
    std::string name;                        /// library basename
    std::string path;                        /// library path (absolute)
    void* handle = nullptr;                  /// handle for dlopen/dlsym/dlclose
    Interface* object = nullptr;             /// payload
                                             //
    Interface* ( *create )( void );          /// constructor proxy
    void ( *destroy )( Interface* );         /// destructor proxy
    void* ( *getData )( Interface* );        /// (optional) persistent data getter
    void ( *setData )( Interface*, void* );  /// (optional) persistent data setter
    //
    template < typename Other >
    operator DynamicBinding< Other >( )  /// cast operator, not really needed
    {
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
