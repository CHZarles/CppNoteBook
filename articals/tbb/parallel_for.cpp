#include <cmath>
#include <iostream>
#include <tbb/parallel_for.h>
#include <vector>

void case0() {
  size_t n = 1 << 26;
  std::cout << "n = " << n << std::endl;
  std::vector<float> a(n);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
                    [&](const tbb::blocked_range<size_t> &r) {
                      for (size_t i = r.begin(); i != r.end(); ++i) {
                        a[i] = std::sqrt(a[i]);
                      }
                    });
}

int main() {
  case0();
  return 0;
}
