#include "ofColor.h"

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
