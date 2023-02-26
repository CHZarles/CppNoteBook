#include <iostream>
#include <memory>


class Entity {
public:
  Entity() { puts("Entity created!"); }
  ~Entity() { puts("Entity destroyed!"); }
};

void ex1() {
  puts("Entering ex1");
  {
    puts("Entering ex1::scope1");
    auto e1 = std::make_unique<Entity>();    
    puts("Leaving ex1::scope1");
  }
  puts("Leaving ex1");
}

int main(){
  ex1();
}