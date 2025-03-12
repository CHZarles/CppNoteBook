# Time in Cpp

- reference: https://github.com/parallel101/course/blob/master/05/slides.pptx

## Time in C

```c
long t0 = time(NULL); // 获取从1970年1月1日到当前时经过的秒数
sleep(3); // 让程序休眠3秒
long t1 = t0 + 3; // 当前时间的三秒后
usleep(3000000); // 让程序休眠3000000微秒，也就是3秒
```

> C 语言原始的 API，没有类型区分，导致很容易弄错单位，混淆时间点和时间段。
> 比如 t0 \* 3，乘法对时间点而言根本是个无意义的计算，然而 C 语言把他们看做一样的 long 类型，从而容易让程序员犯错。

## std::chrono

利用 C++ 强类型的特点，明确区分时间点与时间段，明确区分不同的时间单位。

- 时间点类型：`chrono::steady_clock::time_point` 等
- 时间段类型：`chrono::milliseconds`，`chrono::seconds`，`chrono::minutes`, 等
  方便的运算符重载：时间点+时间段=时间点，时间点-时间点=时间段

```cpp
auto t0 = chrono::steady_clock::now();
auto t1 = t0 + chrono::seconds(30);    // 当前时间点的30秒后
auto dt = t1 - t0;                     // 获取两个时间点的差（时间段）
int64_t sec = chrono::duration_cast<chrono::seconds>(dt).count();  // 时间差的秒数
```

### sample

```c++

#include <chrono>
#include <iostream>

int main() {
  auto t0 = std::chrono::steady_clock::now();
  for (volatile int i = 0; i < 1e7; ++i) {
  }
  auto t1 = std::chrono::steady_clock::now();
  auto dt = t1 - t0;
  {

    int64_t ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(dt).count();
    std::cout << "Time: " << ms << " ms" << std::endl;
  }

  {

    using double_ms = std::chrono::duration<double, std::milli>;
    double ms = std::chrono::duration_cast<double_ms>(dt).count();
    std::cout << "Time: " << ms << " ms" << std::endl;
  }
  return 0;
}


```

> - duration_cast 可以在任意的 duration 类型之间转换 。
> - duration<T, R> 表示用 T 类型表示，且时间单位是 R。R ，省略不写就是秒，std::milli 就是毫秒，std::micro 就是微秒。
> - seconds 是 duration<int64_t> 的类型别名
>   ,milliseconds 是 duration<int64_t, std::milli> 的类型别名

## std::this_thread::sleep_for

可以用 std::this_thread::sleep_for 替代 Unix 类操作系统专有的的 usleep。他可以让当前线程休眠一段时间，然后继续。

而且单位也可以自己指定，比如这里是 milliseconds 表示毫秒，也可以换成 microseconds 表示微秒，seconds 表示秒，chrono 的强类型让单位选择更自由。

```C++
std::this_thread::sleep_for(std::chrono::milliseconds(400));
```

## std::this_thread::sleep_until

除了接受一个时间段的 sleep_for，还有接受一个时间点的 sleep_until，表示让当前线程休眠直到某个时间点。

```C++
auto t = std::chrono::steady_clock::now() + std::chrono::milliseconds(400);
std::this_thread::sleep_until(t);
```
