#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "ofVec3f.h"
#include "ofLog.h"
#include "../Interfaces/GenericInterface.h"
#include "../Interfaces/CameraInterface.h"

struct Data {
    float counter;
};

class Stuff : public GenericInterface {
  public:
    Stuff();
    void update( );
    void draw( );
    void linkCamLib( CameraInterface * aCamLib ) {
        camLib = aCamLib;
    }
    Data data;

  private:
    CameraInterface * camLib;
};

Stuff::Stuff()
{
    data.counter = 0;
}

void Stuff::update( ) {
    data.counter += .2f;
    ( data.counter >= 90 ) && ( data.counter -= 90 );
    // roll camera
    ofVec3f camOrientation = camLib->cam.getOrientationEuler();
    camOrientation.z = sin( data.counter / 90.f * 2 * PI ) * 30;
    camLib->cam.setOrientation( camOrientation );
}

void Stuff::draw( ) {
    ofClear( 0 );
    ofDrawAxis( 3 );
    //
    ofPushStyle();
    //ofNoFill();
    ofFill();
    ofSetColor( 255, 255, 0 );
    ofRotateZ( data.counter );
    ofDrawBox( 1, 1, 1 );
    //ofDrawSphere( 0.5 );
    ofPopStyle();
}

// -------------------------------------------------------------
extern "C" {
    Stuff * create( ) {
        return new Stuff;
    }
    void destroy( Stuff * obj ) {
        delete ( obj );
    }
    void* getData( Stuff* obj )
    {
        Data* data = new Data;
        *data = obj->data;
        return (void*)data;
    }
    void setData( Stuff* obj, void* data )
    {
        obj->data = *( (Data*)data );
        delete ( (Data*)data );
    }
}
