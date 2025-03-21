#include <cmath>
#include <iostream>
#include <sstream>
#include <tbb/parallel_for.h>
#include <thread>
#include <vector>

template <class T, class... Ts> static void mtprint(T &&t, Ts &&...ts) {
  std::stringstream ss;
  ss << std::forward<T>(t);
  ((ss << ' ' << std::forward<Ts>(ts)), ...);
  ss << std::endl;
  std::cout << ss.str();
}

// 静态均匀划分static_partitioner
void situation0() {

  size_t n = 32;

  tbb::task_arena ta(4);
  ta.execute([&] {
    tbb::parallel_for(
        tbb::blocked_range<size_t>(
            0,
            n), // 创建了4个线程4个任务，每个任务包含8个元素。

        /*
        另外可以指定区间的粒度，将tbb::parallel_for(tbb::blocked_range<size_t>(0,
        n) 改为tbb::parallel_for(tbb::blocked_range<size_t>(0, n, 16)，
        这个情况下创建了2个线程2个任务，每个任务包含16个元素。
        */

        // 任务函数，处理每个块的范围 r
        [&](tbb::blocked_range<size_t> r) {
          mtprint("thread", tbb::this_task_arena::current_thread_index(),
                  "size", r.size());
          // display the range
          for (size_t i = r.begin(); i != r.end(); ++i) {
            mtprint("thread", tbb::this_task_arena::current_thread_index(), "i",
                    i);
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(400));
        },
        // 使用 static_partitioner 进行静态分区
        tbb::static_partitioner{});
  });
}

// tbb::simple_partitioner 是 TBB
// 中的一种分区器，它会根据任务数量和线程数进行简单的任务分配。 与
// static_partitioner 不同，simple_partitioner
// 更灵活，可以根据负载情况动态调整任务分配， 更适合负载不均衡的情况。
void situation1() {
  size_t n = 32;

  tbb::task_arena ta(4);
  ta.execute([&] {
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, n),
        /*
         创建了 4 个线程 32 个任务，每个任务包含 1 个元素

         另外，也可以指定区间的粒度大小，将tbb::parallel_for(tbb::blocked_range<size_t>(0,
         n)改为tbb::parallel_for(tbb::blocked_range<size_t>(0, n, 4)

         创建了 4 个线程 8 个任务，每个任务包含 4 个元素
         */
        [&](tbb::blocked_range<size_t> r) {
          for (size_t i = r.begin(); i < r.end(); i++) {

            mtprint("thread", tbb::this_task_arena::current_thread_index(),
                    "size", r.size(), "begin", r.begin());
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
          }
        },
        tbb::simple_partitioner{});
  });
}

void situiation2() {
  size_t n = 32;

  tbb::task_arena ta(4);
  ta.execute([&] {
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, n),
        [&](tbb::blocked_range<size_t> r) {
          for (size_t i = r.begin(); i < r.end(); i++) {

            mtprint("thread", tbb::this_task_arena::current_thread_index(),
                    "size", r.size(), "begin", r.begin());
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
          }
        },
        tbb::auto_partitioner{});
  });
}

int main() {
  std::cout << "situiation 0" << std::endl;
  situation0();
  std::cout << "situiation 1" << std::endl;
  situation1();
  std::cout << "situiation 2" << std::endl;
  situiation2();
  return 0;
}
