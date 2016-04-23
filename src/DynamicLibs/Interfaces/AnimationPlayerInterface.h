#pragma once
#include <vector>
#include <string>
#include "ofColor.h"

///
/// \brief The AnimationPlayerInterface class is completely abstract
/// (has only pure virtual functions) and is used as an interface
/// to the dynamic libraries loaded at runtime.
///
class AnimationPlayerInterface {
  public:
    virtual void update( float deltaTime )=0;
    virtual void play( std::string name )=0;
    virtual void stop( )=0;
    virtual bool isPlaying( )=0;
    virtual void scrub( std::string name, float position )=0;
    virtual std::vector< ofColor > & getColors()=0;
};

// typedef DynamicLibInterface * ( *dynamicConstructor )();
// typedef void ( *dynamicDeconstructor )( DynamicLibInterface * );
