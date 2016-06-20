#include "../DynamicLibs.h"
#include "../DynamicLibs.cpp"
#include "../Interfaces/LibLoaderInterface.h"

// project-specific interfaces used for communication with your libraries
#include "../Interfaces/GenericInterface.h"
#include "../Interfaces/CameraInterface.h"

// avoid multiple inheritance confusion, use composition instead
class LibLoaderHelper : public DynamicLibs
{
public:
    CameraInterface* cam;
    GenericInterface* stuff;
private:
    void initLibs( )
    {
        // initLib( "LibLoader", libloader );	
	initLib( "Stuff", stuff );
	initLib( "Camera", cam );
	stuff->linkCamLib( cam );
	// stuff->setup( );
    }
};

class LibLoader : public LibLoaderInterface
{
public:
    void setup( );
    void update( );
    LibLoaderHelper llh;
};

void LibLoader::setup( )
{
    llh.setup( false );
}

void LibLoader::update( )
{
    /// @todo fix this workaround: register & un-register event listeners properly
    llh.stuff->update( );
    llh.stuff->draw( );
}

// -------------------------------------------------------------
extern "C" {
    LibLoader * create( ) {
        return new LibLoader;
    }
    void destroy( LibLoader * obj ) {
        delete ( obj );
    }
    void* getData( LibLoader* obj )
    {
        std::map< std::string, DynamicBinding< void > > * data = new std::map< std::string, DynamicBinding< void > >;
	*data = obj->llh.libs;
        // Data* data = new Data;
        // *data = obj->data;
        return (void*)data;
    }
    void setData( LibLoader* obj, void* data )
    {
        obj->llh.libs = *( (std::map< std::string, DynamicBinding< void > > *)data );
        // obj->data = *( (Data*)data );
        // delete ( (Data*)data );
	delete( (std::map< std::string, DynamicBinding< void > > *)data );
    }
}
