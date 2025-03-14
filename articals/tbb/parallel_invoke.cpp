#include <iostream>
#include <string>
#include <tbb/parallel_invoke.h>

int main() {
  std::string s = "hello, world!";
  char ch = 'l';
  tbb::parallel_invoke(
      [&] {
        for (int i = 0; i < s.size() / 2; ++i) {
          if (s[i] == ch)
            std::cout << "Found char " << ch << " at position " << i
                      << std::endl;
        }
      },
      [&] {
        for (int i = s.size() / 2; i < s.size(); ++i) {
          if (s[i] == ch)
            std::cout << "Found char " << ch << " at position " << i
                      << std::endl;
        }
      });
  return 0;
}
