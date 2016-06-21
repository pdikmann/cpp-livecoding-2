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
