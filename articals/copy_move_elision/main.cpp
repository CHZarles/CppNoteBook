#include <iostream>
#include <string>

class A {
public:
  A() { std::cout << "Constructor" << std::endl; }
  A(const A &obj) { std::cout << "Copy constructor" << std::endl; }
  A(const A &&obj) { std::cout << "Move constructor" << std::endl; }
  int value;
};

A func() {
  A a;
  a.value = 10;
  return a;
}
A func2() {
  A a;
  a.value = 11;
  return std::move(a);
}
int main() {
  A a = func();
  A b = func2();
}
