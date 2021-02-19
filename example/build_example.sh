#!/bin/sh

#  get root project folder
#  then find build folder for the lib archive
#  then build engine

START_DIR="$( cd "$( dirname "$0" )" && pwd )"
PROJECT_DIR="$(dirname "$START_DIR")"

echo "Building wtengine demo..."

#g++ -c src/wte_demo.cpp -std=c++17 -O3 -Wall -I"$PROJECT_DIR"/include -L"$PROJECT_DIR"/build -lphysfs -lpthread -lwtengine `pkg-config --libs allegro-5 allegro_main-5 allegro_physfs-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_image-5 allegro_primitives-5`
#g++ -c src/main.cpp -std=c++17 -O3 -Wall -I"$PROJECT_DIR"/include -L"$PROJECT_DIR"/build -lphysfs -lpthread -lwtengine `pkg-config --libs allegro-5 allegro_main-5 allegro_physfs-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_image-5 allegro_primitives-5`

#g++ main.o wte_demo.o -O3 -o build/wte_demo -L"$PROJECT_DIR"/build -lphysfs -lpthread -lwtengine `pkg-config --libs allegro-5 allegro_main-5 allegro_physfs-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_image-5 allegro_primitives-5`

g++ -o build/wte_demo main.o wte_demo.o

echo "Done!"
