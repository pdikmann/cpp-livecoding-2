#pragma once

#include "DynamicLibs.h"

// project-specific interfaces used for communication with your libraries
#include "Interfaces/ParticlesInterface.h"

class MyLibs : public DynamicLibs
{
public:
    // pointers to the objects (implementing your interfaces) obtained from your libraries
    ParticlesInterface* particles;

private:
    void initLibs( )
    {
        // cast objects into action
        // use library filenames, e.g. Particles.so -> "Particles"
        castLib( "Particles", particles );
    }
};
