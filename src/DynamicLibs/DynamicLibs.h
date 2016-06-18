#pragma once

#include <string>
#include <map>
#include <vector>
#include <csignal>
#include "ofFileUtils.h"
#include "DynamicBinding.h"
#include "ofLog.h"
#include "ofEvents.h"

using namespace std;

class DynamicLibs
{
public:
    // functions
    void setup( );                                   /// register signal handler, call loadLibs
    template < typename T >
    void initLib( string name, T& out_interface )    /// template for comfortable casting
    {
        bool ok = reloadLib( name );
        if( ok ){
            out_interface = static_cast< T >( libs.at( name ).object );
            ofLogVerbose( "DynamicLibs" ) << "casting " << name << " to " << out_interface;
        }
    }
    // variables
    static volatile std::sig_atomic_t sigSwap;             /// reload flag
    std::map< std::string, DynamicBinding< void > > libs;  /// all loaded libraries

private:
    // functions
    void update( ofEventArgs & eargs );              /// check if re-load was signalled. called in ofApp::update
    //
    bool reloadLib( std::string name );              /// check if lib is loaded, load or rebind accordingly
    DynamicBinding< void > & loadLib( ofFile file ); /// load a specific library (and store a dynamicBinding in `libs`). assumes this is the first time the lib is loaded!
    // void loadAllLibs( );                          /// load all .dylibs from ./bin/data (and store dynamicBindings in `libs`)
    // void swapLib( std::string name );             /// re-load specific library
    // void swapLibs( );                             /// re-load all previously loaded libs
    bool bindLib( std::string name,
                  std::string path,
                  DynamicBinding< void >& out );     /// invoke the <dlfcn> magic on a library binding
    void destroyOldLib( DynamicBinding< void >& out,
                        void*( &persistentData ));   /// call to `destroy` and `getData` of lib
    bool closeOldLib( DynamicBinding< void >& out ); /// dlclose
    bool openNewLib( std::string name,
                     std::string path,
                     DynamicBinding< void >& out );  /// dlopen
    bool bindNewLib( DynamicBinding< void >& out );  /// dlsym
    void createNewLib( DynamicBinding< void >& out,
                       void* persistentData );       /// call to `create` and `setData` of lib
    //
    virtual void initLibs( ) = 0;                    /// project-specific casting
};
