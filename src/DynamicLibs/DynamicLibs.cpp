#include <dlfcn.h>
#include "DynamicLibs.h"
#include "ofUtils.h"
#include "ofFileUtils.h"

volatile std::sig_atomic_t DynamicLibs::sigSwap;

void signalHandler( int sig )
{
    ofLogVerbose( "DynamicLibs" ) << "Signal handled!";
    DynamicLibs::sigSwap = 1;
}

void DynamicLibs::setup( )
{
    std::signal( SIGINT, signalHandler );  // register signal handler
    loadLibs( );
    initLibs( );
    ofAddListener( ofEvents().update, this, &DynamicLibs::update, OF_EVENT_ORDER_BEFORE_APP );
}

void DynamicLibs::update( ofEventArgs & eargs )
{
    if ( sigSwap == 1 )
    {
        ofLogVerbose( "DynamicLibs" ) << "Swapping libs";
        sigSwap = 0;
        swapLibs( );
        initLibs( );
    }
}

DynamicBinding< void >& DynamicLibs::loadLib( ofFile file )
{
    // ofFile file = std::filesystem::path( apath );
    std::string name = file.getBaseName( );
    std::string path = file.getAbsolutePath( );
    ofLogVerbose( "DynamicLibs" ) << "Looking for dynamic library " << name;
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

enum BindLibException {
    LIB_WONT_CLOSE,
    LIB_WONT_OPEN,
    LIB_WONT_BIND
};

bool DynamicLibs::bindLib( std::string name, std::string path, DynamicBinding< void >& out )
{
    bool ok = true;
    void* persistentData = nullptr;
    try
    {
        destroyOldLib( out, persistentData );
        ok = closeOldLib( out );
        // if ( !ok )
        //     return ok;
        ok = openNewLib( name, path, out );
        // if ( !ok )
        //     return ok;
        ok = bindNewLib( out );
        // if ( !ok )
        //     return ok;
        createNewLib( out, persistentData );
    }
    catch ( BindLibException e )
    {
        return false;
    }
    ofLogVerbose( "DynamicLibs" ) << "opened lib " << name << " OK!";
    return true;
}

void DynamicLibs::destroyOldLib( DynamicBinding< void >& out, void* persistentData )
{
    if ( out.object != nullptr )
    {
        if ( out.getData != nullptr )
        {
            ofLogVerbose( "DynamicLibs" ) << "retrieving persistent data from " << out.name;
            persistentData = out.getData( out.object );
        }
        ofLogVerbose( "DynamicLibs" ) << "destroying " << out.object << " ...";
        out.destroy( out.object );
        out.object = nullptr;
    }
}

bool DynamicLibs::closeOldLib( DynamicBinding< void >& out )
{
    if ( out.handle != nullptr )
    {
        ofLogVerbose( "DynamicLibs" ) << "closing " << out.name << " ...";
        int retval = dlclose( out.handle );
        if ( retval != 0 )
        {
            ofLogError( "DynamicLibs" ) << "failed to close lib " << out.name;
            ofLogError( "DynamicLibs" ) << dlerror( );
            throw( LIB_WONT_CLOSE );
            return false;
        }
        out.handle = nullptr;
    }
    return true;
}

bool DynamicLibs::openNewLib( std::string name, std::string path, DynamicBinding< void >& out )
{
    ofLogVerbose( "DynamicLibs" ) << "opening lib " << name << " ...";
    out.name = name;
    out.path = path;
    out.handle = dlopen( path.c_str( ), RTLD_LAZY );
    if ( out.handle == nullptr )
    {
        ofLogError( "DynamicLibs" ) << "failed to open lib " << out.name << " at " << out.path;
        ofLogError( "DynamicLibs" ) << dlerror( );
        throw( LIB_WONT_OPEN );
        return false;
    }
    return true;
}

bool DynamicLibs::bindNewLib( DynamicBinding< void >& out )
{
    out.create = (void* (*)( ))dlsym( out.handle, "create" );
    out.destroy = (void (*)( void* ))dlsym( out.handle, "destroy" );
    out.getData = (void* (*)( void* ))dlsym( out.handle, "getData" );  // optional, can be null
    out.setData = (void (*)( void*, void* ))dlsym( out.handle, "setData" );  // optional, can be null
    if ( ( out.create == nullptr ) || ( out.destroy == nullptr ) )
    {
        ofLogError( "DynamicLibs" ) << "failed to bind essential symbols for "
                                    << out.name << " at " << out.path;
        ofLogError( "DynamicLibs" ) << dlerror( );
        throw( LIB_WONT_BIND );
        return false;
    }
    if ( ( out.getData == nullptr ) || ( out.setData == nullptr ) )
    {
        ofLogNotice( "DynamicLibs" ) << "failed to bind optional symbols for "
                                     << out.name << " at " << out.path;
        ofLogNotice( "DynamicLibs" ) << dlerror( );
    }
    return true;
}

void DynamicLibs::createNewLib( DynamicBinding< void >& out, void* persistentData )
{
    out.object = out.create( );
    if ( ( out.setData != nullptr ) && ( persistentData != nullptr ) )
    {
        out.setData( out.object, persistentData );
    }
    ofLogVerbose( "DynamicLibs" ) << "creating object " << out.object << " ...";
}
