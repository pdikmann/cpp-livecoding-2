#pragma once
#include "CameraInterface.h"

class GenericInterface
{
public:
    virtual void update( ) = 0;
    virtual void draw( ) = 0;
    virtual void linkCamLib( CameraInterface* cam ) = 0;
};
