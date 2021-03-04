#!/bin/sh

##################################################
#  Script variables
##################################################
START_DIR="$( cd "$( dirname "$0" )" && pwd )"
SOURCE_DIR="src"
LIBRARY_DIR="lib"

##################################################
#  Function to build all source files
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
            g++ -c "$INNAME" -std=c++17 -O3 -Wall -lphysfs -lpthread -Iinclude -DWTE_DEBUG_MODE=0 -o "$OUTNAME" `pkg-config --libs allegro-5 allegro_main-5 allegro_physfs-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_image-5`
            echo "Done"
        fi
        if [ -d "$i" ]; then
            build_items "$i"
        fi
    done
}

##################################################
#  Start script
##################################################
echo "Building wtengine..."
build_items "$START_DIR/$SOURCE_DIR"

echo ""
echo -n "Removing old library archive if it exists... "
rm "$LIBRARY_DIR"/libwtengine.a
echo "Done"

echo ""
echo -n "Creating library archive... "
LIBRARY_FILE_ARRAY=()
for i in "$START_DIR$LIBRARY_DIR"/*.o; do
    LIBRARY_FILE_ARRAY+=("$(basename "$i")")
done
ar rcs "$LIBRARY_DIR"/libwtengine.a $(printf "$LIBRARY_DIR/%s " "${LIBRARY_FILE_ARRAY[@]}")
echo "Done"

echo ""
echo -n "Cleaning up object files... "
rm "$LIBRARY_DIR"/*.o
echo "Done"

echo ""
echo "Build script complete"
