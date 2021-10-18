#!/usr/bin/env bash

./build/lol-compiler "$1" --code
llc lol.ll
cc lol.s
