#include "ofCamera.h"
#include "../Interfaces/CameraInterface.h"

class Camera : public CameraInterface {
  public:
    Camera( );
    void begin( );
    void end( );
};

// -------------------------------------------------------------
Camera::Camera( ) {
    cam.setupPerspective( true, 90, 0.001, 100 );
    cam.setPosition( 0, 1, 2 );
    cam.lookAt( ofVec3f( 0, 0, .5 ) );
}

void Camera::begin( ) {
    cam.begin( );
}

void Camera::end( ) {
    cam.end( );
}

// -------------------------------------------------------------
extern "C" {
    Camera * create( ) {
        return new Camera;
    }
    void destroy( Camera * obj ) {
        delete ( obj );
    }
}

