#include <iostream>
#include <memory>


class Entity {
public:
  Entity() { puts("Entity created!"); }
  ~Entity() { puts("Entity destroyed!"); }
};

void ex3() {
  puts("--------");
  puts("Entering ex3");
  auto e1 = std::make_shared<Entity>();
  std::cout << e1.use_count() << std::endl;
  {
    puts("Entering ex3::scope1");
    auto e2 = e1; // use_count ++
    std::cout << e1.use_count() << std::endl;
    auto e3 = std::move(e2); // use_count remains
    std::cout << e1.use_count() << std::endl;
    puts("Leaving ex3::scope1");
  }
  std::cout << e1.use_count() << std::endl;
  puts("Leaving ex3");
}
 
 
 int main()
 {
   ex3();
 }