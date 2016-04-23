#pragma once

#include <string>
#include <map>
#include <vector>
#include <csignal>
#include "ofFileUtils.h"
#include "DynamicBinding.h"
#include "Interfaces/GenericInterface.h"

class DynamicLibs
{
public:
    // functions
    void setup( );                    /// register signal handler, call loadLibs
    DynamicBinding< void > & loadLib( ofFile file );      /// load a specific library (and store a dynamicBinding in `libs`)
    void loadLibs( );                 /// load all .dylibs from ./bin/data (and store dynamicBindings in `libs`)
    void update( );                   /// check if re-load was signalled. called in ofApp::update
    void swapLib( std::string name ); /// re-load specific library
    void swapLibs( );                 /// re-load all libs
    // variables
    static volatile std::sig_atomic_t sigSwap;  /// reload flag
    DynamicBinding< GenericInterface > stuff;

private:
    // functions
    bool bindLib( std::string name, std::string path, DynamicBinding< void >& out );
    // variables
    std::map< std::string, DynamicBinding< void > > libs;  /// all loaded libraries
};
