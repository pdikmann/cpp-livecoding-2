#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup( )
{
    // ofSetLogLevel( "DynamicLibs", OF_LOG_VERBOSE );
    libs.setup( );
}

//--------------------------------------------------------------
void ofApp::update( )
{
    libs.update( );
    libs.stuff->update( );
}

//--------------------------------------------------------------
void ofApp::draw( )
{
    // center = ofVec3f( 0, 0, 0 );
    libs.cam->begin( );
    libs.stuff->draw( );
    ofSetColor( 255, 0, 0 );
    ofDrawBox( 1, 1, 1 );
    libs.cam->end( );
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
