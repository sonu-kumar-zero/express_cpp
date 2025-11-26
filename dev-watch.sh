#!/bin/bash

cd /home/sonu/projects/cplus_express

while inotifywait -e modify,create,delete -r core include; do
    cd build
    cmake ..
    make -j$(nproc)
    pkill server
    ./server &
    cd ..
done
