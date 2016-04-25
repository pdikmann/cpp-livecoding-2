#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "../Interfaces/GenericInterface.h"

class Stuff : public GenericInterface
{
    void update( );
    void draw( );
};

void Stuff::update( )
{
}

void Stuff::draw( )
{
    ofClear( 0 );
    ofSetColor( 255, 0, 0 );
    ofDrawBox( 1, 1, 1 );
}

// -------------------------------------------------------------
extern "C" {
Stuff* create( );
void destroy( Stuff* );
}

Stuff* create( )
{
    return new Stuff;
}

void destroy( Stuff* obj )
{
    delete ( obj );
}
