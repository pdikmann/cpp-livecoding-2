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
    // pointers to the objects (implementing your interfaces) obtained from your libraries
    CameraInterface* cam;
    GenericInterface* stuff;

private:
    void initLibs( )
    {
        // cast objects into action
        // use library filenames, e.g. Stuff.so -> "Stuff"
	initLib( "Stuff", stuff );
	initLib( "Camera", cam );
	stuff->linkCamLib( cam );
	stuff->setup( );
    }
};

class LibLoader : public LibLoaderInterface
{
public:
    void setup( );
    LibLoaderHelper llh;
};

void LibLoader::setup( )
{
    llh.setup( false );
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
        BindingsMap* data = new BindingsMap;
	*data = obj->llh.libs;
        return (void*)data;
    }
    void setData( LibLoader* obj, void* data )
    {
        obj->llh.libs = *( (BindingsMap*)data );
	delete( (BindingsMap*)data );
    }
}
