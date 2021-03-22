#!/usr/bin/env bash
[ -d build ] || mkdir build
set -e
cd build && cmake .. -G Ninja || exit $? && ninja || exit $? && \
[ "$1" == "run" ] && ./src/MyApplication && exit 0
exit $?
