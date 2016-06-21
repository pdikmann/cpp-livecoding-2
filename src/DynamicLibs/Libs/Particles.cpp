#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "ofVec3f.h"
#include "ofLog.h"
#include "../Interfaces/ParticlesInterface.h"

struct Data {
    float counter;
};

class Particles : public ParticlesInterface {
  public:
    Particles();
    void update( );
    void draw( );
    Data data;
};

Particles::Particles()
{
    data.counter = 0;
}

void Particles::update( ) {
    data.counter += .2f;
    ( data.counter >= 90 ) && ( data.counter -= 90 );
    // roll camera
    ofVec3f camOrientation = camLib->cam.getOrientationEuler();
    camOrientation.z = sin( data.counter / 90.f * 2 * PI ) * 30;
    camLib->cam.setOrientation( camOrientation );
}

void Particles::draw( ) {
    ofClear( 0 );
    ofDrawAxis( 3 );
    //
    ofPushStyle();
    ofNoFill();
    //ofFill();
    ofSetColor( 255, 0, 0 );
    ofRotateZ( data.counter );
    ofDrawBox( 1, 1, 1 );
    //ofDrawSphere( 0.5 );
    ofPopStyle();
}

// -------------------------------------------------------------
extern "C" {
    Particles * create( ) {
        return new Particles;
    }
    void destroy( Particles * obj ) {
        delete ( obj );
    }
    void* getData( Particles* obj )
    {
        Data* data = new Data;
        *data = obj->data;
        return (void*)data;
    }
    void setData( Particles* obj, void* data )
    {
        obj->data = *( (Data*)data );
        delete ( (Data*)data );
    }
}
