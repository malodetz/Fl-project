#!/usr/bin/env bash

./build/lol-compiler "$1" --bin
llc lol.bc
cc lol.s
