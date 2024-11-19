#include "linkedList.h"
#include <iostream>

int TestNr = 0;

class Test {
public: 
  Test(){
    cout << "Constructed" << endl;
  }

  Test(const Test& o) {
    cout << "Copied" << endl;
  }

  Test(Test&& o) {
    cout << "Moved" << endl;
  }

  Test& operator=(const Test& o) {
    cout << "Copy Assigned" << endl;
    return *this;
  }

  Test& operator=(Test&& o) {
    cout << "Move Assigned" << endl;
    return *this;
  }

  ~Test(){
    cout << "Destroyed" << endl;
  }
};


int main() {
  LinkedList<int> list;
  int a = 10;
  int b = 12;
  int c = 13;
  // Test a;
  // Test b = a;
  std::cout << "Empty = " << list.empty() << std::endl;
  list.push_back(a);
  std::cout << "Empty = " << list.empty() << std::endl;
  list.push_back(b);
  std::cout << "Size = " << list.size() << std::endl;
  list.push_back(c);
  list.push_back(15);

  for (LinkedList<int>::Iterator listIt = list.begin(); listIt != list.end(); listIt++) {
    std::cout << (*listIt) << std::endl;
  }

  std::cout << "Size = " << list.size() << std::endl;
  std::cout << "Size = " << list.size() << std::endl;

  return 0;
};
