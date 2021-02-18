#!/bin/sh

#set -x
START_DIR="$( cd "$( dirname "$0" )" && pwd )"
SOURCE_DIR="/src"

##################################################
#  Function
##################################################
build_items()
{
    echo ""
    echo "Entering $1"
    for i in "$1"/*; do
        if [ -f "$i" ] && [[ "$i" =~ \.cpp$ ]]; then
            INNAME="${i#$START_DIR/}"
            OUTNAME="$(basename "$i")"
            OUTNAME="lib/${OUTNAME%.cpp}.o"
            echo ""
            echo "Building $INNAME..."
            #echo "$INNAME"
            #echo "$OUTNAME"
            g++ -c "$INNAME" -std=c++17 -O3 -Wall -lphysfs -lpthread -Iinclude -DWTE_DEBUG_MODE=0 -o "$OUTNAME" `pkg-config --libs allegro-5 allegro_main-5 allegro_physfs-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_image-5 allegro_primitives-5`
        fi
        if [ -d "$i" ]; then
            build_items "$i"
        fi
    done
}

echo "Building wtengine..."

build_items "$START_DIR$SOURCE_DIR"

#"src/wte_main.cpp",
#"-std=c++17",
#"-lphysfs",
#"-lpthread",
#"-O3",
#"-Wall",
#"-Iinclude",
#"-DWTE_DEBUG_MODE=0",
#"-o",
#"build/wtengine.o",
#"`pkg-config",
#"--libs",
#"allegro-5",
#"allegro_main-5",
#"allegro_physfs-5",
#"allegro_audio-5",
#"allegro_acodec-5",
#"allegro_font-5",
#"allegro_image-5",
#"allegro_primitives-5",

#g++ -c src/test_main.cpp -std=c++17 -Iinclude -o libs/test_main.o
#ar rcs build/libtest.a libs/test_main.o libs/testa1.o libs/testa2.o libs/testb1.o libs/testb2.o
