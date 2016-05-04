#pragma once

#include "ofCamera.h"

class CameraInterface
{
public:
    virtual void begin( ) = 0;
    virtual void end( ) = 0;
    ofCamera cam;
};
