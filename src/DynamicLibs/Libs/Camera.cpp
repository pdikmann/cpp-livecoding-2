#include "ofCamera.h"
#include "../Interfaces/CameraInterface.h"

class Camera : public CameraInterface
{
public:
    Camera( );
    void begin( );
    void end( );

private:
    ofCamera cam;
};

// -------------------------------------------------------------
Camera::Camera( )
{
    cam.setupPerspective( true, 90, 0.001, 100 );
    // cam.enableOrtho();
    cam.setPosition( 0, 1, 2 );
    // cam.setOrientation( ofVec3f( -45, 0, 0 ));
    cam.lookAt( ofVec3f( 0, 0, .5 ));
}

void Camera::begin( )
{
    cam.begin( );
}

void Camera::end( )
{
    cam.end( );
}

// -------------------------------------------------------------
extern "C" {
Camera* create( );
void destroy( Camera* );
}

Camera* create( )
{
    return new Camera;
}

void destroy( Camera* obj )
{
    delete ( obj );
}
