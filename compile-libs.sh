# set -v

# ToDo: commandline options -f --force and -o --once

# ------------------------------------------------------------
# Configuration
pd_timestamp_file="$0"               # the file to compare for timestamps (this file by default)
pd_oF_dir=../../..                   # oF root directory
pd_oF_process_name=double-dragon     # the name of your project
pd_source_dir=./src/DynamicLibs/Libs # the directory containing the library sources
pd_target_dir=./bin/data/            # the directory to put the compiled libraries in
pd_timeout=2                         # interval (in seconds) to check for changed timestamps
# ------------------------------------------------------------
# OS-specifics
case $(uname)
in
    Linux)
        pd_os=linux64 
        pd_lib_extension=so
        pd_cpp_call="g++ -fPIC -shared"
        break;;
    Darwin*)
        pd_os=osx
        pd_lib_extension=dylib
        pd_cpp_call="g++ -dynamiclib -flat_namespace -undefined dynamic_lookup"
        break;;
esac
pd_oF_flags=$(cat $pd_oF_dir/libs/openFrameworksCompiled/lib/$pd_os/obj/Release/.compiler_flags)
# ------------------------------------------------------------
# Internal (hands off!)
pd_update_app=0
pd_renew_timestamp=0

# echo "Usage:"
# echo " -f, --force"
# echo "        force compilation even when timestamps are old"
# echo " -o, --once"
# echo "        don't loop forever"

# while [ $# -gt 0 ]
# do
#     case 
# done

echo "Continuously compiling files in $pd_source_dir to $pd_target_dir"
echo "Press any key to stop ..."
echo 

# if [ $# -eq 0 ] 
# then
#     echo "Usage options"
#     exit 0
# fi

function pd_compile_fast () {
    # compile without oF-headers (faster)
    set -o verbose
    $pd_cpp_call \
        $1 -o $2 
    RETVAL=$?
    set +o verbose
    return $RETVAL
}

function pd_compile_elaborate () {
    # compile with oF-headers included (takes longer)
    set -o verbose
    $pd_cpp_call \
        $pd_oF_flags \
        $1 -o $2
    RETVAL=$?
    set +o verbose
    return $RETVAL
}

function pd_check_for_of_headers () {
    # not sophisticated enough
    grep -q -e '#include ["<]of' $1
    RETVAL=$?
    return $RETVAL
}

function pd_validate_name () {
    echo $pd_sets | grep -q $1
    return $?
}

# loop forever:
# compile recently changed files,
# check for errors,
# signal app on success
# 
while true :
do
    # init
    pd_renew_timestamp=0

    # compile any recently changed files
    for f in $(find $pd_source_dir -name "*.cpp" -newer $pd_timestamp_file)
    do
        pd_renew_timestamp=1
        strip_name=$(basename $f .cpp)
        source_file=$pd_source_dir/$strip_name.cpp
        target_file=$pd_target_dir/$strip_name.$pd_lib_extension

        tput setaf 4
        echo compiling $f
        tput sgr0

        # check wether any of-sources are included
        pd_check_for_of_headers $source_file
        if [ $? -eq 0 ]
        then
            pd_compile_elaborate $source_file $target_file
        else
            pd_compile_elaborate $source_file $target_file
            # pd_compile_fast $source_file $target_file
        fi
        # check compilation result
        if [ $? -eq 0 ] 
        then 
            tput setaf 2
            echo ok
            tput sgr0
            pd_compilation_fail=0
        else
            pd_compilation_fail=1
            tput setaf 1
            echo failed
            tput sgr0
        fi
    done

    # log compilation results, signal app on success
    now=$(date +"%H:%M:%S")
    if [ $pd_renew_timestamp -eq 1 ] 
    then
        if [ $pd_compilation_fail -eq 0 ]
        then
            pd_update_app=1
            tput setaf 2
            echo done ------------------------------------------------------- $now
            tput sgr0
        else
            pd_update_app=0
            tput setaf 1
            echo fail XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX $now
            tput sgr0
        fi
    fi

    # update timestamp & send signal to app
    [ $pd_update_app      -eq 1 ] && pkill -2 $pd_oF_process_name
    [ $pd_renew_timestamp -eq 1 ] && touch $pd_timestamp_file

    # exit on keypress (single char, timeout 2 seconds)
    read -n1 -t$pd_timeout
    [ $? -eq 0 ] && {
        echo
        exit 0
    }
done
