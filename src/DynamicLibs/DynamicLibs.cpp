#include <dlfcn.h>
#include "DynamicLibs.h"
#include "ofUtils.h"
#include "ofFileUtils.h"

volatile std::sig_atomic_t DynamicLibs::sigSwap;

void signalHandler( int sig )
{
    DynamicLibs::sigSwap = 1;
}

void DynamicLibs::setup( )
{
    std::signal( SIGINT, signalHandler );  // register signal handler
    loadLibs( );
    castLibs( );
}

void DynamicLibs::update( )
{
    if ( sigSwap == 1 )
    {
        sigSwap = 0;
        swapLibs( );
        castLibs( );
    }
}

DynamicBinding< void >& DynamicLibs::loadLib( ofFile file )
{
    // ofFile file = std::filesystem::path( apath );
    std::string name = file.getBaseName( );
    std::string path = file.getAbsolutePath( );
    ofLogNotice( "DynamicLibs" ) << "Looking for dynamic library " << name;
    DynamicBinding< void > binding;
    binding.handle = nullptr;
    binding.object = nullptr;
    bool ok = bindLib( name, path, binding );
    if ( ok )
    {
        libs.insert( std::pair< std::string, DynamicBinding< void > >( name, binding ) );
    }
    return libs.at( name );
}

void DynamicLibs::loadLibs() {
    ofDirectory dir( "." ); // resolves to DataPath
    ofLogVerbose( "DynamicLibs" ) << "Looking for dynamic libraries in " << dir.getAbsolutePath();
    dir.allowExt( "dylib" );
    dir.allowExt( "so" );   
    dir.listDir();
    for( int i = 0; i < dir.size(); ++i ) {
        ofFile file = dir.getFile( i );
        loadLib( file );
    }
}

// void DynamicLibs::swapLib( std::string name ) {
//     ofLogVerbose( "DynamicLibs" ) << "Hot-swapping single library " << name;
//     auto it = libs.find( name );
//     if( it == libs.end() ) {
//         ofLogError( "DynamicLibs" ) << "no such library loaded: " << name;
//         return;
//     }
//     DynamicBinding< void > & binding = ( *it ).second;
//     // reload library
//     bool ok = bindLib( binding.name,
//                        binding.path,
//                        binding );
//     if( !ok ) ofLogError( "DynamicLibs" ) << "re-binding failed for " << binding.name;
// }

void DynamicLibs::swapLibs( )
{
    ofLogVerbose( "DynamicLibs" ) << "Hot-swapping all libraries...";
    for ( auto it = libs.begin( ); it != libs.end( ); ++it )
    {
        DynamicBinding< void >& binding = ( *it ).second;
        // reload library
        bool ok = bindLib( binding.name,
                           binding.path,
                           binding );
        if ( !ok )
            ofLogError( "DynamicLibs" ) << "re-binding failed for " << binding.name;
    }
}

bool DynamicLibs::bindLib( std::string name, std::string path, DynamicBinding< void >& out )
{
    // destroy old object
    if ( out.object != nullptr )
    {
        ofLogVerbose( "DynamicLibs" ) << "destroying " << out.object << " ...";
        out.destroy( out.object );
        out.object = nullptr;
    }
    // closing
    if ( out.handle != nullptr )
    {
        ofLogVerbose( "DynamicLibs" ) << "closing " << out.name << " ...";
        int retval = dlclose( out.handle );
        if ( retval != 0 )
        {
            ofLogError( "DynamicLibs" ) << "failed to close lib " << out.name;
            ofLogError( "DynamicLibs" ) << dlerror( );
            return false;
        }
        out.handle = nullptr;
    }
    // open
    ofLogVerbose( "DynamicLibs" ) << "opening lib " << name << " ...";
    out.name = name;
    out.path = path;
    out.handle = dlopen( path.c_str( ), RTLD_LAZY );
    if ( out.handle == nullptr )
    {
        ofLogError( "DynamicLibs" ) << "failed to open lib " << name << " at " << path;
        ofLogError( "DynamicLibs" ) << dlerror( );
        return false;
    }
    // binding
    out.create = (void* (*)( ))dlsym( out.handle, "create" );
    out.destroy = (void (*)( void* ))dlsym( out.handle, "destroy" );
    if ( ( out.create == nullptr ) || ( out.destroy == nullptr ) )
    {
        ofLogError( "DynamicLibs" ) << "failed to bind symbols for " << name << " at " << path;
        ofLogError( "DynamicLibs" ) << dlerror( );
        return false;
    }
    // object creation
    out.object = out.create( );
    ofLogVerbose( "DynamicLibs" ) << "creating object " << out.object << " ...";
    ofLogVerbose( "DynamicLibs" ) << "opened lib " << name << " OK!";
    return true;
}
