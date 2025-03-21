## Install

按照指南安装 benchmakr globally

```
https://github.com/google/benchmark?tab=readme-ov-file#installation
```

## 用法

这个链接有个具体的例子。 https://bencher.dev/learn/benchmarking/cpp/google-benchmark/

我这里也保存相关代码，直接可以运行。

```bash
cd articals/google_benchmark
# ./run.sh
g++ play_game.cpp benchmark_game.cpp -std=c++11  -lbenchmark -lpthread  -o benchmark_game && ./benchmark_game
```

编译出来的可执行文件可接受一些命令行参数来控制测试的输出格式为 csv 等等，你可以调用 --help 查看更多用法。
```bash
./benchmark_game --help
```
