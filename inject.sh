#!/bin/bash

# injects all sourcefiles and helpers needed for livecoding into the working directory.
# usage example:
# 
# > cd openFrameworks/apps/myApps
# > projectgenerator -o../.. newApp
# > cd newApp
# > git clone git@github.com:pdikmann/ofLive.git
# > ofLive/inject.sh

targetdir=`pwd`
scriptdir=`dirname "$0"`

# echo script-dr: $targetdir
# echo target-dir: $scriptdir

cd $scriptdir

# copy scripts
cp -t "$targetdir/" \
    compile-libs.sh \
    0001-add-fPIC-flag-to-compile.core.mk.patch 

# copy target files
cp -r -t "$targetdir/src/" \
    src/DynamicLibs

# integrate 
cd $targetdir/src

# echo including
sed -i 's=\(#include "ofMain.h"\)=\1\n#include "DynamicLibs/MyLibs.h"=' ofApp.h
# echo composing
sed -i 's/\(};\)/\t\tMyLibs libs;\n\1/' ofApp.h
# echo setup
sed -i 's/\(void ofApp::setup(){\)/\1\n\tlibs.setup( true );/' ofApp.cpp

cd $targetdir

# echo exclude from compilation
gawk -i inplace '/^[# ]*PROJECT_EXCLUSIONS/ { print; print "PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/DynamicLibs/Libs%"; next }1' config.make 

./compile-libs.sh -f -o

