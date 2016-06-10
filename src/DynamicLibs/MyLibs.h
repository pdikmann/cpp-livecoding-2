#pragma once

#include "DynamicLibs.h"

// project-specific interfaces used for communication with your libraries
#include "Interfaces/GenericInterface.h"
#include "Interfaces/CameraInterface.h"

class MyLibs : public DynamicLibs
{
public:
    // pointers to the objects (implementing your interfaces) obtained from your libraries
    GenericInterface* stuff;
    CameraInterface* cam;

private:
    void initLibs( )
    {
        // cast objects into action
        // use library filenames, e.g. Stuff.so -> "Stuff"
        castLib( "Stuff", stuff );
        castLib( "Camera", cam );
        stuff->linkCamLib( cam ); // at this point, `stuff` is fully usable and can be called according to its interface.
    }
};
