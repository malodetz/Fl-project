#!/usr/bin/bash

make -j4

prefix="tests/error_handling/syntax_errors"

./build/lol-compiler "$prefix/test1.lang" test >out.txt
if [ -n "$(cmp "$prefix/out1.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test2.lang" test >out.txt
if [ -n "$(cmp "$prefix/out2.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test3.lang" test >out.txt
if [ -n "$(cmp "$prefix/out3.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test4.lang" test >out.txt
if [ -n "$(cmp "$prefix/out4.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

prefix="tests/error_handling/type_mismatch"

./build/lol-compiler "$prefix/test1.lang" test >out.txt
if [ -n "$(cmp "$prefix/out1.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test2.lang" test >out.txt
if [ -n "$(cmp "$prefix/out2.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test3.lang" test >out.txt
if [ -n "$(cmp "$prefix/out3.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

prefix="tests/error_handling/var_redeclaration"

./build/lol-compiler "$prefix/test1.lang" test >out.txt
if [ -n "$(cmp "$prefix/out1.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./build/lol-compiler "$prefix/test2.lang" test >out.txt
if [ -n "$(cmp "$prefix/out2.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

prefix="tests/valid/factorial"

./l_to_exec.sh "$prefix/factorial.lang" test >out.txt
# shellcheck disable=SC2065
./test >out.txt
if [ -n "$(cmp "$prefix/out.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

prefix="tests/valid/fibonacci"

./l_to_exec.sh "$prefix/fibonacci.lang" test >out.txt
# shellcheck disable=SC2065
./test >out.txt
if [ -n "$(cmp "$prefix/out.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

prefix="tests/valid/gcd"

./l_to_exec.sh "$prefix/gcd.lang" test >out.txt
# shellcheck disable=SC2065
./test >out.txt
if [ -n "$(cmp "$prefix/out.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

prefix="tests/valid/nested"

./l_to_exec.sh "$prefix/nested.lang" test >out.txt
# shellcheck disable=SC2065
./test >out.txt
if [ -n "$(cmp "$prefix/out.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

prefix="tests/valid/sqrt"

./l_to_exec.sh "$prefix/sqrt.lang" test >out.txt
# shellcheck disable=SC2065
./test >out.txt
if [ -n "$(cmp "$prefix/out.txt" out.txt)" ]; then
  echo "FAILED"
else
  echo "OK"
fi