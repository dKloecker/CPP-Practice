#include <iostream>
#include <memory>
#include <ostream>

using namespace std;

template <typename T>
class LinkedList {
private:
  struct Node {
    template<typename C = T>
    Node(C&& val, unique_ptr<Node> nextNode) 
    : d_val(forward<C>(val))
    , next(move(nextNode))
    {};

    T                 d_val;
    unique_ptr<Node>  next;

    const T& val() const {
      return d_val;
    }

    T& val() {
      return d_val;
    }
  };

  unique_ptr<Node> root;
  Node* head = nullptr;
public:
  

  template<typename C = T>
  void push_back(C&& val) {
    if (root == nullptr) {
      root = make_unique<Node>(forward<C>(val), nullptr);
      head = root.get();
      return;
    }
    head->next = make_unique<Node>(forward<C>(val), nullptr);
    head = head->next.get();
  } 

  template<typename C=T>
  C& emplace_back(C&& val) {
    push_back(forward<C>(val));
    return head->val();
  }

  template<typename C=T>
  C pop_back() {
    if (root == nullptr) {
      throw std::runtime_error("Cannot pop_back() on empty list");
    } 
    else if (root->next == nullptr) {
      C tmp = root->val();
      root = nullptr;
      return tmp;
    }


    Node* tail = nullptr;
    Node* curr = root.get();

    while (curr->next.get() != nullptr) {
      tail = curr;
      curr = curr->next.get();
    }

    const C tmp = curr->val();
    tail->next = nullptr;
    head = tail;
    return tmp;
  }


  template<typename C=T>
  C pop_front() {
    if (root == nullptr) 
      throw std::runtime_error("Cannot pop_front() on empty list");
   
    const C tmp = root->val();
    root = move(root->next);
    return tmp;
  }

  struct Iterator {
    friend LinkedList;
  private:
    Node* nodePtr = nullptr;

  public:
    Iterator(Node *node) : nodePtr(node) {};
    Iterator(const Iterator& o) : nodePtr(o.nodePtr) {};

    const Iterator& operator++() {
      if (nodePtr == nullptr) {
        return *this;
      }
      nodePtr = nodePtr->next.get();
      return *this;
    }

    const Iterator operator++(int) {
      Iterator old = *this;
      operator++();
      return old;
    }

    bool operator==(const Iterator& rhs) const {
      return nodePtr == rhs.nodePtr;
    }
    bool operator!=(const Iterator& rhs) const {
      return !(nodePtr == rhs.nodePtr); 
    }

    template<typename C = T>
    C& operator*() {
      C& val = nodePtr->val();
      return val;
    }
  };

  Iterator begin() {
    return Iterator(root.get());
  }

  Iterator end() {
    return Iterator(nullptr);
  }

  bool empty() {
    return root == nullptr;
  } 

  int size() {
    int size = 0;
    Iterator it = begin();
    while (it != end()) {
      +it;
      ++size;
    }
    return size;
  }

  void clear() {
    root = nullptr;
    head = nullptr;
  }

  template<typename C = T>
  C& front() {
    return root->val();
  }

  template<typename C = T>
  C& back() {
    return head->val();
  }
};
