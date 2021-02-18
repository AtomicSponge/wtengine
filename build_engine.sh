#!/bin/sh
#g++ -c src/test_main.cpp -std=c++17 -Iinclude -o libs/test_main.o

#set -x
CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

for i in "$CURRENT_DIR"/src/*; do
    filename=$(basename "$i")
    if [ -f $i ]; then
        g++ -c src/$filename -std=c++17 -O3 -Wall -lphysfs -lpthread -Iinclude -DWTE_DEBUG_MODE=0 -o lib/${filename%.*}.o `pkg-config --libs allegro-5 allegro_main-5 allegro_physfs-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_image-5 allegro_primitives-5`
    fi
done

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

#ar rcs build/libtest.a libs/test_main.o libs/testa1.o libs/testa2.o libs/testb1.o libs/testb2.o
