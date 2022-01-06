#!/bin/bash


it_lexes() {
  input="$1"
  expected="$2"
  printf "[lex] \"$input\""
  printf "$input" | ./holyc --debug-lexer >/dev/null 2>actual.out
  echo "$expected" > expected.out

  diff --text expected.out actual.out
  result=$?

  if [ $result -ne 0 ]; then
    echo "$0: FAIL"
    exit 1
  else
    echo "\tok"
    rm actual.out expected.out
  fi
}
assert() {
  expected="$1"
  input="$2"

  echo "$input => $expected"
  printf "$input" | ./holyc --debug-parser --debug-lexer 2>err.out > a.out && chmod u+x a.out
  if [ $? -ne 0 ]; then
    echo '-- error --'
    cat err.out
    exit 2
  fi
  ./a.out
  actual=$?

  if [ "$actual" = "$expected" ]; then
    rm ./a.out
  else
    echo "expected $expected, but got $actual"
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
it_lexes "42;" \
"[lexer] Lex(): [INT, '42']
[lexer] Lex(): [SEMI, ';']
[lexer] Unlex()
[lexer] Lex(): [SEMI, ';']
[lexer] Unlex()
[lexer] Lex(): [SEMI, ';']
[lexer] Lex(): [EOF, '\\\0']"


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
assert 2 '(  1 +  3 - 1  ) * (2  * 3 / 2 ) - 7  ;\n'

# nothing
assert_error "expected a ';', got '\\\0' at column 3" '\0'
assert 1 ";"
assert_error "expected a '\\\0', got ';' at column 2" ';;'
assert_error "expected a '\\\0', got ';' at column 2" ';;;'
assert_error "expected a '\\\0', got ';' at column 2" ';;;;'

# unexpected
assert_error "-- error: unexpected character 'p' (112) at column 0" 'p'

# multiple statements
# assert 42 "40;2;"

echo 'OK'
