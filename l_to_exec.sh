#!/usr/bin/bash
# $1 -- source code, $2 - exec name
make -j4

build/lol-compiler "$1" "$2" # compiler automatically add suffix .ll
llc "$2.ll"  
clang -no-pie "$2.s" -o "$2"
