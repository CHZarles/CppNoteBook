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
  // sample 0
  {
    std::future<int> fret = std::async([&] { return download("hello.zip"); });
    interact();
    int ret = fret.get();
    std::cout << "Download returned: " << ret << std::endl;
  }

  // sample 1
  {
    std::future<int> fret = std::async([&] { return download("hello.zip"); });
    interact();
    while (true) {
      std::cout << "Waiting for download to finish...\n";
      auto stat = fret.wait_for(std::chrono::milliseconds(500));
      if (stat == std::future_status::ready) {
        int ret = fret.get();
        std::cout << "Download returned: " << ret << std::endl;
        break;
      } else {
        std::cout << "Download still in progress...\n";
      }
    }
  }

  // sample 2
  {

    std::future<int> fret = std::async(std::launch::deferred,
                                       [&] { return download("hello.zip"); });
    interact();
    int ret = fret.get();
    std::cout << "Download returned: " << ret << std::endl;
  }
  return 0;
}
