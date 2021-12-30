#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  printf "$input" | ./holyc 2>err.out > a.out && chmod u+x a.out
  if [ $? -ne 0 ]; then
    echo '-- error --'
    cat err.out
    exit 2
  fi
  ./a.out
  actual=$?

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
    rm ./a.out
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert_error() {
  expected="$1"
  input="$2"

  printf "$input" | ./holyc 2>err.out >a.out

  echo "$expected" > expected.out

  diff --text expected.out err.out
  result=$?

  if [ $result -ne 0 ]; then
    echo "$0: FAIL"
    exit 1
  else
    echo "$input => $expected"
    rm err.out expected.out a.out
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
if grep -q docker /proc/1/cgroup; then
  assert_error "expected a ';', got '\0' at column 3" '\0'
else
  assert_error "expected a ';', got '\\\0' at column 3" '\0'
fi
assert 1 ";"
# assert 0 ";;"
# assert 0 ";;;"

assert_error "-- error: unexpected character 'p' (112) at column 0" 'p'

# assert 2 "1;2"
# assert 2 "1;2;"
# assert 4 "1;1;4;;;;;;"

echo 'OK'
