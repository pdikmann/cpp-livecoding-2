#pragma once

#include "DynamicLibs.h"
#include "Interfaces/GenericInterface.h"
#include "Interfaces/CameraInterface.h"

class MyLibs : public DynamicLibs
{
public:
    // project-specific libs
    GenericInterface* stuff;
    CameraInterface* cam;

private:
    void castLibs( )
    {
        // cast member variables into action
        stuff = static_cast< GenericInterface* >( libs.at( "Stuff"  ).object );
        cam   = static_cast< CameraInterface*  >( libs.at( "Camera" ).object );
    }
};
