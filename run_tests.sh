#!/usr/bin/bash

make -j4

prefix="tests/error_handling/syntax_errors"

./build/lol-compiler "$prefix/test1.lang" test > out.txt
if [ -n "$(cmp "$prefix/out1.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test2.lang" test > out.txt
if [ -n "$(cmp "$prefix/out2.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test3.lang" test > out.txt
if [ -n "$(cmp "$prefix/out3.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi


./build/lol-compiler "$prefix/test4.lang" test > out.txt
if [ -n "$(cmp "$prefix/out4.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi
