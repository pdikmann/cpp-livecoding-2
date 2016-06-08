#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "ofVec3f.h"
#include "ofLog.h"
#include "../Interfaces/GenericInterface.h"
#include "../Interfaces/CameraInterface.h"

class Stuff : public GenericInterface {
  public:
    Stuff();
    void update( );
    void draw( );
    void linkCamLib( CameraInterface * aCamLib ) {
        camLib = aCamLib;
    }

  private:
    float counter;
    CameraInterface * camLib;
};

Stuff::Stuff()
    : counter( 0 ) {}

void Stuff::update( ) {
    counter += .2f;
    ( counter >= 90 ) && ( counter -= 90 );
    // roll camera
    ofVec3f camOrientation = camLib->cam.getOrientationEuler();
    camOrientation.z = sin( counter / 90.f * 2 * PI ) * 30;
    camLib->cam.setOrientation( camOrientation );
}

void Stuff::draw( ) {
    ofClear( 0 );
    ofDrawAxis( 3 );
    //
    ofPushStyle();
    ofNoFill();
    //ofFill();
    ofSetColor( 255, 0, 0 );
    ofRotateZ( counter );
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
}


