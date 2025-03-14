# 原子操作

`std::atomic` 是 C++11 引入的一个模板类，用于实现原子操作。
原子操作是指在多线程环境下，不可被中断的操作，确保数据的一致性和正确性。

```c++
#include <iostream>
#include <thread<
#include <atomic>

int main(){
    std::atomic<int> counter = 0;
    std::thread t1([&]{
    for( int i = 0 ; i < 10000 ; ++ i)
        counter += 1;
    });
    std::thread t2([&]{
    for( int i = 0 ; i < 10000 ; ++ i)
        counter += 1;
    });
    t1.join();
    t2.join();
    std::cout << counter << std::endl;
    return 0;
}

```

在使用时候 atomic，对他的 += 等操作，会被编译器转换成专门的指令。
CPU 识别到该指令时，会锁住内存总线，放弃乱序执行等优化策略（将该指令视为一个同步点，强制同步掉之前所有的内存操作），从而向你保证该操作是原子 (atomic) 的（取其不可分割之意），不会加法加到一半另一个线程插一脚进来。

注意：请用 +=，不要让 + 和 = 分开

- counter = counter + 1; // 错，不能保证原子性
- counter += 1; // OK，能保证原子性
- counter++; // OK，能保证原子性

## fetch_add：和 += 等价

除了用方便的运算符重载之外，还可以直接调用相应的函数名，比如：

- fetch_add 对应于 +=
- store 对应于 =
- load 用于读取其中的 int 值

## fetch_add：会返回其旧值

- int old = atm.fetch_add(val)
- 除了会导致 atm 的值增加 val 外，还会返回 atm 增加前的值，存储到 old。
- 这个特点使得他可以用于并行地往一个列表里追加数据：追加写入的索引就是 fetch_add 返回的旧值。
- 当然这里也可以 counter++，不过要追加多个的话还是得用到 counter.fetch_add(n)。

```c++

std::atomic<int> counter;
counter.store(0);

std::vector<int> data(20000);

std::thread t1([&]{
    for(int i = 0 ; i < 10000; ++j){
        int idx = counter.fetch_add(1);
        data[idx] = i;
    }
});

std::thread t2([&]{
    for(int i = 0 ; i < 10000; ++j){
        int idx = counter.fetch_add(1);
        data[idx] = i;
    }
});

// ...
```

## exchange：读取的同时写入

exchange(val) 会把 val 写入原子变量，同时返回其旧的值。

## compare_exchange_strong：读取，比较是否相等，相等则写入

compare_exchange_strong(old, val) 会读取原子变量的值，比较他是否和 old 相等：

如果不相等，则把原子变量的值写入 old。

如果相等，则把 val 写入原子变量。

返回一个 bool 值，表示是否相等。

注意 old 这里传的其实是一个引用，因此 compare_exchange_strong 可修改他的值
