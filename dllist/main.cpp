#include "dllist.h"
#include <_stdlib.h>
#include <ios>
#include <iostream>

int object_count = 0;

class Test {
public:
  Test(int a, std::string b) : d_a(a), d_b(b), d_id(object_count++) {
    std::cout << "Constructed: " << *this;
  }

  Test(const Test& other) : d_a(other.d_a), d_b(other.d_b), d_id(object_count++) {
    std::cout << "Copy Constructed: " << *this;
  };

  Test(Test&& other) : d_a(std::move(other.d_a)), d_b(std::move(other.d_b)), d_id(object_count++){
    std::cout << "Move Constructed: " << *this;
  };

  Test& operator=(Test&&) {
    std::cout << "Move Assigned: " << *this;
    return *this;
  }

  Test& operator=(const Test& other) {
    std::cout << "Copy Assigned: " << *this;
    return *this;
  }

  ~Test(){
    std::cout << "Destructed: " << *this;
  }

  friend std::ostream& operator<<(std::ostream& out, const Test& val) {
    out << "Test(" << val.d_id << ")" << std::endl;
    return out;
  }
private:
  int d_a;
  std::string d_b;
  int d_id;
};


int main() {
  std::cout << "Hello Word" << std::endl;

  DoubleLinkedList<int> list;
  list.push_back(1);
  list.push_back(2);

  auto it = list.begin();
  list.insert(it, 0);

  it = list.begin();
  list.insert(it, -1);

  it = list.end();
  list.insert(it, 3);

  it = list.end(); 
  list.insert(it, 5);

  it = list.begin();
  it++;
  it++;
  list.insert(it, 420);


  std::cout << list.pop_back() << std::endl;
  std::cout << list.pop_back() << std::endl;
  std::cout << list.pop_back() << std::endl;
  std::cout << list.pop_back() << std::endl;
  std::cout << list.pop_back() << std::endl;
  std::cout << list.pop_back() << std::endl;

  return 0;
}
