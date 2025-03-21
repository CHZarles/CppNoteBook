#include <cmath>
#include <cstdlib>
#include <iostream>
#include <tbb/parallel_for_each.h>
#include <tbb/parallel_pipeline.h>
#include <tbb/tick_count.h>
#include <vector>
#define TICK(x) auto bench_##x = tbb::tick_count::now();
#define TOCK(x)                                                                \
  std::cout << #x ": " << (tbb::tick_count::now() - bench_##x).seconds()       \
            << "s" << std::endl;


#define MAGIC 11

struct Data {
  std::vector<float> arr;

  Data() {
    arr.resize(std::rand() % 100 * 500 + 10000);
    for (int i = 0; i < arr.size(); i++) {
      arr[i] = std::rand() * (1.f / (float)RAND_MAX);
    }
  }

  void step1() {
    for (int i = 0; i < arr.size(); i++) {
      arr[i] += 3.14f;
    }
  }

  void step2() {
    std::vector<float> tmp(arr.size());
    for (int i = 1; i < arr.size() - 1; i++) {
      tmp[i] = arr[i - 1] + arr[i] + arr[i + 1];
    }
    std::swap(tmp, arr);
  }

  void step3() {
    for (int i = 0; i < arr.size(); i++) {
      arr[i] = std::sqrt(std::abs(arr[i]));
    }
  }

  void step4() {
    std::vector<float> tmp(arr.size());
    for (int i = 1; i < arr.size() - 1; i++) {
      tmp[i] = arr[i - 1] - 2 * arr[i] + arr[i + 1];
    }
    std::swap(tmp, arr);
  }
};

void seq_process() {
  size_t n = 1 << MAGIC;

  std::vector<Data> dats(n);

  TICK(seq_process);
  for (auto &dat : dats) {
    dat.step1();
    dat.step2();
    dat.step3();
    dat.step4();
  }
  TOCK(seq_process);
}

void parallel_for_process() {
  size_t n = 1 << MAGIC;
  std::vector<Data> dats(n);
  TICK(parallel_for_process)
  tbb::parallel_for_each(dats.begin(), dats.end(), [&](Data &dat) {
    dat.step1();
    dat.step2();
    dat.step3();
    dat.step4();
  });
  TOCK(parallel_for_process);
}

void parallel_for_process2() {
  size_t n = 1 << MAGIC;
  std::vector<Data> dats(n);
  TICK(parallel_for_process_2)
  tbb::parallel_for_each(dats.begin(), dats.end(),
                         [&](Data &dat) { dat.step1(); });
  tbb::parallel_for_each(dats.begin(), dats.end(),
                         [&](Data &dat) { dat.step2(); });
  tbb::parallel_for_each(dats.begin(), dats.end(),
                         [&](Data &dat) { dat.step3(); });
  tbb::parallel_for_each(dats.begin(), dats.end(),
                         [&](Data &dat) { dat.step4(); });
  TOCK(parallel_for_process_2);
}

void tbb_pipeline_process() {
  size_t n = 1 << MAGIC;
  std::vector<Data> dats(n);
  TICK(pipeline_process)
  auto it = dats.begin();
  tbb::parallel_pipeline(
      8,
      tbb::make_filter<void, Data *>(tbb::filter_mode::serial_in_order,
                                     [&](tbb::flow_control &fc) -> Data * {
                                       if (it == dats.end()) {
                                         fc.stop();
                                         return nullptr;
                                       }
                                       return &*it++;
                                     }),
      tbb::make_filter<Data *, Data *>(tbb::filter_mode::parallel,
                                       [&](Data *dat) -> Data * {
                                         dat->step1();
                                         return dat;
                                       }),
      tbb::make_filter<Data *, Data *>(tbb::filter_mode::parallel,
                                       [&](Data *dat) -> Data * {
                                         dat->step2();
                                         return dat;
                                       }),
      tbb::make_filter<Data *, Data *>(tbb::filter_mode::parallel,
                                       [&](Data *dat) -> Data * {
                                         dat->step3();
                                         return dat;
                                       }),
      tbb::make_filter<Data *, void>(tbb::filter_mode::parallel,
                                     [&](Data *dat) -> void { dat->step4(); })
  );
  TOCK(pipeline_process)
}
int main() {

  seq_process();
  parallel_for_process();
  parallel_for_process2();
  tbb_pipeline_process();

}
