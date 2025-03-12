#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

int download(std::string file) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "Downloading " << file << " (" << i * 10 << "%) ... "
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "Download complete: " << file << "\n";
  return 404;
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}

int main() {

    // 注释 std::future<int> fret = std::async([&] { return download("hello.zip"); });
    // 等价注释部分
    std::promise<int> pret;
    std::thread t1([&] {
      int ret = download("hello.zip");
      pret.set_value(ret);
    });
    std::future<int> fret = pret.get_future();

    interact();
    int ret = fret.get();
    std::cout << "Download returned: " << ret << std::endl;
  
    // 别忘记这个
    t1.join();
}
