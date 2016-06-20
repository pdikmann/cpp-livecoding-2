#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup( )
{
    ofSetLogLevel( "DynamicLibs", OF_LOG_VERBOSE );
    ofSetLogLevel( "MyLibs", OF_LOG_VERBOSE );
    ofSetLogLevel( "ofApp", OF_LOG_ERROR );
    libs.setup( true );
}

//--------------------------------------------------------------
void ofApp::update( )
{
    ofLogVerbose( "ofApp" ) << "updating";
}

//--------------------------------------------------------------
void ofApp::draw( )
{
    ofLogVerbose( "ofApp" ) << "drawing";
    libs.libloader->update( );
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
