#pragma once

#include "DynamicLibs.h"

// project-specific interfaces used for communication with your libraries
#include "Interfaces/LibLoaderInterface.h"

class MyLibs : public DynamicLibs
{
public:
    // pointers to the objects (implementing your interfaces) obtained from your libraries
    LibLoaderInterface* libloader;

private:
    void initLibs( )
    {
        // cast objects into action
        // use library filenames, e.g. Stuff.so -> "Stuff"
        initLib( "LibLoader", libloader );	
        ofLog() << "post-init in mylibs";
	libloader->setup( );
    }
};
