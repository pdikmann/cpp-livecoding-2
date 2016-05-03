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
    void castLib( string name, T& out_interface )    /// template for comfortable casting
    {
        out_interface = static_cast< T >( libs.at( name ).object );
        ofLogVerbose( "DynamicLibs" ) << "casting " << name << " to " << out_interface;
    }
    // variables
    static volatile std::sig_atomic_t sigSwap;             /// reload flag
    std::map< std::string, DynamicBinding< void > > libs;  /// all loaded libraries

private:
    // functions
    void update( ofEventArgs & eargs );              /// check if re-load was signalled. called in ofApp::update
    DynamicBinding< void > & loadLib( ofFile file ); /// load a specific library (and store a dynamicBinding in `libs`)
    void loadLibs( );                                /// load all .dylibs from ./bin/data (and store dynamicBindings in `libs`)
    bool bindLib( std::string name, std::string path, DynamicBinding< void >& out );
    // void swapLib( std::string name );             /// re-load specific library
    void swapLibs( );                                /// re-load all previously loaded libs
    virtual void initLibs( ) = 0;                    /// project-specific casting
};
