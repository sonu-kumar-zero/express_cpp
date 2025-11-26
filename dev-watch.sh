#!/bin/bash

cd /home/sonu/projects/cplus_express

while inotifywait -e modify,create,delete -r core main.cpp include; do
    cd /home/sonu/projects/cplus_express/build
    cmake ..
    make -j$(nproc)
    pkill server
    ./server
done
