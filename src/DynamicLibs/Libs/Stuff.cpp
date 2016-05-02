#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "../Interfaces/GenericInterface.h"

class Stuff : public GenericInterface
{
public:
    Stuff();
    void update( );
    void draw( );
private:
    float counter;
};

Stuff::Stuff()
  : counter( 0 ){}

void Stuff::update( )
{
    counter += .2f;
}

void Stuff::draw( )
{
    ofClear( 0 );
    ofSetColor( 0, 255, 0 );
    ofRotateZ( counter );
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
