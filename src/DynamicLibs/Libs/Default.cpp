#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "ofEvents.h"
#include "../Interfaces/DefaultInterface.h"

struct Data {
    float counter;
};

class Default : public DefaultInterface {
public:
    Default();
    ~Default();
    void setup( );
    void update( ofEventArgs & eargs );
    void draw( ofEventArgs & eargs );
    Data data;
};

Default::Default()
{
    data.counter = 0;
}

Default::~Default()
{
    ofRemoveListener( ofEvents().update, this, &Default::update, OF_EVENT_ORDER_AFTER_APP );
    ofRemoveListener( ofEvents().draw, this, &Default::draw, OF_EVENT_ORDER_AFTER_APP );
}

void Default::setup()
{
    ofAddListener( ofEvents().update, this, &Default::update, OF_EVENT_ORDER_AFTER_APP );
    ofAddListener( ofEvents().draw, this, &Default::draw, OF_EVENT_ORDER_AFTER_APP );
}

void Default::update( ofEventArgs & eargs ) {
    data.counter += .2f;
    ( data.counter >= 90 ) && ( data.counter -= 90 );
}

void Default::draw( ofEventArgs & eargs ) {
    ofPushMatrix();
    ofPushStyle();
    //
    ofFill();
    ofSetColor( 255, 0, 0 );
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    ofRotateZ( data.counter );
    int boxSize = ofGetWidth() / 10;
    ofDrawBox( boxSize, boxSize, boxSize );
    //
    ofPopStyle();
    ofPopMatrix();
}

// -------------------------------------------------------------
extern "C" {
    Default * create( ) {
        return new Default;
    }
    void destroy( Default * obj ) {
        delete ( obj );
    }
    void* getData( Default* obj )
    {
        Data* data = new Data;
        *data = obj->data;
        return (void*)data;
    }
    void setData( Default* obj, void* data )
    {
        obj->data = *( (Data*)data );
        delete ( (Data*)data );
    }
}
