#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  printf "$input" | ./holyc 2>/dev/null > a.out && chmod u+x a.out
  ./a.out
  actual=$?

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

should_error() {
  input="$1"
  expected_msg="$2"

  printf "$input" | ./holyc 2>err.out >a.out

  echo "$expected_msg" > expected.out

  diff expected.out err.out
  if [ $? -ne 0 ]; then
    echo "$0: FAIL"
    echo "$input => '$expected' expected, but got '$actual'"
    exit 1
  else
    echo "$input => $actual"
  fi
}

#--

# basics
assert 0 '0;'
assert 42 '42;'
assert 255 '255;'

# arithmetic
assert 40 '42-2;'
assert 44 '42+2;'
assert 32 '42-20+10;'
assert 9 '1+2*3+4/2;'

# parenthesis
assert 4 '(1+3);'
assert 2 '((2));'
assert 8 '(1+3)*2;'
assert 2 '(1+3)*2/3;'
assert 4 '(1)+(3);'
assert 12 '(1+3)*2/1+(4);'
assert 2 '(1+3-1)*(2*3/2)-7;'

# spacing
assert 5 '  3 +2;'
assert 2 '(  1 +  3 - 1  ) * (2  * 3 / 2 ) - 7  ;'

# nothing
assert 0 "\0"
assert 1 ";"
assert 0 ";;"
assert 0 ";;;"

should_error "p" "-- error: unexpected character 'p' (112) at column 1\np^"
# should_error "p" "-- error: unexpected character 'p' (112) at column 1\np\n^"

# assert 2 "1;2"
# assert 2 "1;2;"
# assert 4 "1;1;4;;;;;;"

echo 'OK'
