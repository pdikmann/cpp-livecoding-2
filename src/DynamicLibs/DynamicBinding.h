#pragma once

#include <string>
#include "Interfaces/AnimationPlayerInterface.h"

struct DynamicBinding {
    std::string name; /// library basename
    std::string path; /// library path (absolute)
    void * handle = nullptr; /// handle for dlopen/dlsym/dlclose
    AnimationPlayerInterface * object = nullptr; /// payload
    //
    AnimationPlayerInterface * ( *create )( void ); /// constructor proxy
    void ( *destroy )( AnimationPlayerInterface * ); /// destructor proxy
};
