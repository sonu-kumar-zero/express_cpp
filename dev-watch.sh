#!/bin/bash

PROJECT_DIR="/home/sonu/projects/cplus_express"
BUILD_DIR="$PROJECT_DIR/build"
SERVER="$BUILD_DIR/server"

RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
YELLOW="\033[33m"
NC="\033[0m"

log() {
    echo -e "${YELLOW}[$(date +'%H:%M:%S')]${NC} $1"
}

cd "$PROJECT_DIR" || exit 1

log "Watching for file changes in source folders..."

SERVER_PID=0

start_server() {
    log "${BLUE}▶ Starting server...${NC}"
    setsid "$SERVER" &>/dev/null &
    SERVER_PID=$!
    log "${BLUE}▶ server job id...${SERVER_PID}"
}

stop_server() {
    if [ $SERVER_PID -ne 0 ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        log "${RED}⛔ Stopping old server (PID: $SERVER_PID)${NC}"
        kill "$SERVER_PID"
    fi
}

cleanup() {
    log "${RED}Exiting... killing server (PID: $SERVER_PID)${NC}"
    kill -TERM "$SERVER_PID" 2>/dev/null
    exit 0
}

trap cleanup SIGINT SIGTERM EXIT


start_server  # first run before watch loop

while inotifywait -e modify,create,delete -r core include main.cpp; do
    log "${GREEN}🔄 Change detected — Rebuilding...${NC}"

    cd "$BUILD_DIR"
    cmake .. > build.log 2>&1 && make -j$(nproc) >> build.log 2>&1

    if [ $? -eq 0 ]; then
        log "${GREEN}✔ Build success${NC}"
        stop_server
        start_server
    else
        log "${RED}❌ Build failed (see build.log)${NC}"
    fi

    cd "$PROJECT_DIR"
done
