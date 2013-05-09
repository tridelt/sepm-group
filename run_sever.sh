#!/bin/bash

cd ./build/bin

function stop {
  if [ -e ./server.pid ] ; then
    echo "killing old server"
    kill -2 `cat ./server.pid`
    rm ./server.pid
  fi
}

function start {
  stop
  mkdir -p ./log
  mv ./server.log "./log/server-$(date +%Y%m%d%S).log"
  ./server -q &
  disown
  SERVER_PID=$!
  echo "Started server with $SERVER_PID"
  echo "$SERVER_PID" > ./server.pid
}

case "$1" in
  "stop")
    stop
    ;;
  "start")
    start
    ;;
  *)
    start
    ;;
esac
