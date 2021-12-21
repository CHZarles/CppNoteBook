#include <iostream>
#include <memory>


void observe(std::weak_ptr<Entity> ew) {
  if (std::shared_ptr<Entity> spt = ew.lock()) {
    std::cout << spt.use_count() << std::endl;
    std::cout << "entity still alive!" << std::endl;
  } else {
    std::cout << "entity was expired :(" << std::endl;
  }
}
 
void ex4() {
  puts("--------");
  puts("Entering ex4");
  std::weak_ptr<Entity> ew;  
  {
    puts("Entering ex4::scope1");
    auto e1 = std::make_shared<Entity>();
    std::cout << e1.use_count() << std::endl;
    ew = e1; // use_count remains
    std::cout << e1.use_count() << std::endl;
    observe(ew);
    puts("Leaving ex4::scope1");
  }
  observe(ew);
  puts("Leaving ex4");
}
 
int main(int argc, char** argv) {
  ex4();
  return 0;
}