#!/usr/bin/bash
# $1 -- source code, $2 - exec name
make -j4

cat "$1" | build/lol-compiler "$2" # compiler automatically add suffix .ll
llc "$2.ll"  
clang "$2.s" -o "$2"
