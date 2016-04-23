extern "C" {
Stuff* create( );
void destroy( Stuff* );
}

struct Stuff
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

Stuff* create( )
{
    return new Stuff;
}

void destroy( Stuff* obj )
{
    delete ( obj );
}
