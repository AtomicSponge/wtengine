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
            echo -n "Building $INNAME... "
            g++ -c "$INNAME" -std=c++17 -O3 -Wall -lphysfs -lpthread -Iinclude -DWTE_DEBUG_MODE=0 -o "$OUTNAME" `pkg-config --libs allegro-5 allegro_main-5 allegro_physfs-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_image-5 allegro_primitives-5`
            echo "Done"
        fi
        if [ -d "$i" ]; then
            build_items "$i"
        fi
    done
}

echo "Building wtengine..."

build_items "$START_DIR$SOURCE_DIR"

echo ""
echo -n "Creating library archive... "

#ar rcs build/libtest.a libs/test_main.o libs/testa1.o libs/testa2.o libs/testb1.o libs/testb2.o

echo "Done"

echo ""
echo "Build script complete"
