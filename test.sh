#!/usr/bin/env bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -static -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert_error() {
  expected="$1"
  input="$2"

  ./9cc "$input" > /dev/null 2> tmp.err
  actual="$(cat tmp.err)"

  if [ "$actual" = "$expected" ]; then
    echo "$input => error"
  else
    echo "$input => expected error:"
    echo "$expected"
    echo "but got:"
    echo "$actual"
    exit 1
  fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "
assert_error "1+3++
    ^ 不是数字" "1+3++"
echo OK
