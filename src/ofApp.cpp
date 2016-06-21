#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup( )
{
    ofSetLogLevel( "DynamicLibs", OF_LOG_VERBOSE );
    ofSetLogLevel( "MyLibs", OF_LOG_VERBOSE );
    ofSetLogLevel( "ofApp", OF_LOG_ERROR );
    libs.setup( );
    cam.setPosition( 0, 0, 10 );
    cam.setFov( 90 );
    cam.setNearClip( 0.01 );
    cam.setFarClip( 100 );
}

//--------------------------------------------------------------
void ofApp::update( )
{
    ofLogVerbose( "ofApp" ) << "updating";
    libs.particles->update( );
}

//--------------------------------------------------------------
void ofApp::draw( )
{
    ofLogVerbose( "ofApp" ) << "drawing";
    cam.begin();
    libs.particles->draw( );
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h )
{
}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg )
{
}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo )
{
}
