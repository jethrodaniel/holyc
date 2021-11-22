#!/bin/bash

assert() {
 expected="$1"
 input="$2"

 echo "$input" | ./holyc 2>/dev/null > a.out
 chmod u+x a.out
 ./a.out
 actual="$?"

 if [ "$actual" = "$expected" ]; then
   echo "$input => $actual"
 else
   echo "$input => $expected expected, but got $actual"
   exit 1
 fi
}

assert 0 0
assert 42 42
assert 255 255
assert 40 '42-2'
assert 44 '42+2'
assert 32 '42-20+10'
assert 9 '1+2*3+4/2'
assert 4 '(1+3)'
assert 2 '((2))'
assert 8 '(1+3)*2'
assert 2 '(1+3)*2/3'
assert 4 '(1)+(3)'
assert 12 '(1+3)*2/1+(4)'
assert 2 '(1+3-1)*(2*3/2)-7'

echo OK