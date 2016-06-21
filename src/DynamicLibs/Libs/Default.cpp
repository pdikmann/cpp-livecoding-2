#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "ofEvents.h"
#include <vector>
#include "../Interfaces/DefaultInterface.h"

struct Particle {
    uint64_t age;
    float angle;
    Particle(){
	age = 0;
	angle = 0;
    };
};

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
private:
    std::vector< Particle > particles;
    int count = 100;
    uint64_t wrap_time = 3000;
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
    uint64_t real_time = ofGetElapsedTimeMillis();
    uint64_t time = real_time;
    // uint64_t time = ( 1 + sin( real_time / 1000.f )) / 2.f * wrap_time; // fluctuating time :D
    particles.resize( count );
    uint64_t step_time = wrap_time / count;
    ofSeedRandom( 123 );
    int i = 0;
    for( Particle & p : particles ){
	p.age = ( time + step_time * i ) % wrap_time;
	p.angle = ofRandom( 0, TWO_PI );
	i++;
    }
}

void Default::draw( ofEventArgs & eargs ) {
    ofPushMatrix();
    ofPushStyle();
    //
    ofFill();
    ofSetColor( 255, 0, 0 );
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 4 );
    int boxSize = 10;
    float drop_div = 160.f;
    float pos_div = 1000.f;
    float pos_dist = 400.f;
    for( Particle & p : particles ){
	float drop = pow( p.age / drop_div, 2 );
	float pos_factor = ( -1 * pow( 2, -( p.age / pos_div ))) + 1;
	ofPoint pos = { cos( p.angle ) * pos_dist * pos_factor,
	    		sin( p.angle ) * pos_dist * pos_factor };
	uint8_t alpha = 255 - ( 255 * p.age / (float)wrap_time );
	ofPushMatrix();
	ofTranslate( pos );
	ofTranslate( 0, drop );
	ofSetColor( 255, 0, 0, alpha );
    	ofDrawBox( boxSize, boxSize, boxSize );
	ofPopMatrix();
    }
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
