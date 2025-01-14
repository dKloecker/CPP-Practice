#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>
#include <utility>

// ============================================================== //
// ======================= LinkedList =========================== //
// ============================================================== //

template <typename T> class DoubleLinkedList {
  class Node;
  class Iterator;

private:
  std::shared_ptr<Node> d_left = nullptr;
  std::shared_ptr<Node> d_right = nullptr;

public:
  bool empty() { return d_left == nullptr; }
  void clear();
  void remove(const T &val);

  T &back();
  T &front();

  T pop_back();
  T pop_front();

  void push_back(T val);
  void push_front(T val);

  template <typename... Args> T &emplace_back(Args... args);
  template <typename... Args> T &emplace_front(Args... args);

  Iterator begin() { return Iterator(d_left); }
  Iterator end() { return Iterator(nullptr); }
  Iterator insert(const Iterator &pos, const T &val);
  Iterator erase(const Iterator &pos);
};

// ============================================================== //
// =========================== Node ============================= //
// ============================================================== //

template <typename T> class DoubleLinkedList<T>::Node {
private:
  T d_val;
  std::shared_ptr<Node> d_next = nullptr;
  std::shared_ptr<Node> d_prev = nullptr;

public:
  template <typename C = T>
  Node(C val, std::shared_ptr<Node> prev = nullptr,
       std::shared_ptr<Node> next = nullptr)
      : d_val(std::move(val)), d_prev(prev), d_next(next){};

  const T &val() const { return d_val; }
  T &val() { return d_val; }

  const std::shared_ptr<Node> &next() const { return d_next; }
  std::shared_ptr<Node> &next() { return d_next; }

  const std::shared_ptr<Node> &prev() const { return d_prev; }
  std::shared_ptr<Node> &prev() { return d_prev; }
};

// ============================================================== //
// ========================= ITERATOR =========================== //
// ============================================================== //

template <typename T> class DoubleLinkedList<T>::Iterator {
  friend DoubleLinkedList;

private:
  std::shared_ptr<Node> d_node = nullptr;

public:
  Iterator(std::shared_ptr<Node> node) : d_node(node) {};
  Iterator(const Iterator &o) : d_node(o.d_node) {};

  const Iterator &operator++() {
    if (d_node == nullptr) {
      return *this;
    }

    d_node = d_node->next();
    return *this;
  }

  const Iterator operator++(int) {
    Iterator old = *this;
    operator++();
    return old;
  }

  const Iterator operator--() {
    if (d_node == nullptr) {
      return *this;
    }
    d_node = d_node->prev();
    return *this;
  }

  const Iterator operator--(int) {
    Iterator old = *this;
    operator--();
    return old;
  }

  const T &operator*() const { return d_node->val(); }

  T &operator*() { return d_node->val(); }

  bool operator==(const Iterator &rhs) const { return d_node == rhs.d_node; }

  bool operator!=(const Iterator &rhs) const { return !(d_node == rhs.d_node); }
};

// ============================================================== //
// ======================= LinkedList =========================== //
// ============================================================== //

template <typename T> void DoubleLinkedList<T>::clear() {
  // TODO: Does this really free up resources?
  // I think not!
  d_left = nullptr;
  d_right = nullptr;
}

template <typename T> T &DoubleLinkedList<T>::front() { return d_left->val(); }

template <typename T> T &DoubleLinkedList<T>::back() { return d_right->val(); }

template <typename T>
typename DoubleLinkedList<T>::Iterator
DoubleLinkedList<T>::insert(const DoubleLinkedList<T>::Iterator &pos,
                            const T &val) {
  if (pos == end()) {
    push_back(val);
    return Iterator(d_right);
  } else if (pos == begin()) {
    push_front(val);
    return Iterator(d_left);
  }

  std::shared_ptr<Node> node = pos.d_node;
  std::shared_ptr<Node> prev = node->prev();
  std::shared_ptr<Node> newNode = std::make_shared<Node>(val);

  newNode->prev() = prev;
  newNode->next() = node;
  prev->next() = newNode;
  node->prev() = newNode;
  return Iterator(newNode);
}

template <typename T> void DoubleLinkedList<T>::remove(const T &val) {
  auto it = begin();
  while (it != end()) {
    if (*it == val) {
      it = erase(it); 
    } else {
      it++;
    }
  }
}

template <typename T>
typename DoubleLinkedList<T>::Iterator
DoubleLinkedList<T>::erase(const DoubleLinkedList<T>::Iterator &pos) {
  std::shared_ptr<Node> node = pos.d_node;
  if (node == nullptr) {
    // Iterator is end of list
    return end();
  }

  std::shared_ptr<Node> prev = node->prev();
  std::shared_ptr<Node> next = node->next();

  if (prev == nullptr && next == nullptr) {
    // There is only one element
    return end();
  } else if (prev == nullptr) {
    // We are deleting first element of list
    d_left = next;
    next->prev() = nullptr;
    return begin();
  } else if (next == nullptr) {
    // We are deleting last element of list
    prev->next() = nullptr;
    return end();
  } else {
    prev->next() = next;
    next->prev() = prev;
    return Iterator(next);
  }
}

template <typename T>
template <typename... Args>
T &DoubleLinkedList<T>::emplace_back(Args... args) {
  push_back(C(args...));
  return d_right->val();
}

template <typename T> T DoubleLinkedList<T>::pop_back() {
  T val = std::move(d_right->val());
  d_right = d_right->prev();

  if (d_right != nullptr) {
    d_right->next() = nullptr;
  }

  return val;
}

template <typename T> void DoubleLinkedList<T>::push_back(T val) {
  std::shared_ptr<Node> newNode =
      std::make_shared<Node>(std::move(val), d_right);

  if (empty()) {
    d_left = newNode;
    d_right = newNode;
    return;
  }

  d_right->next() = newNode;
  d_right = d_right->next();
}

template <typename T> void DoubleLinkedList<T>::push_front(T val) {
  std::shared_ptr<Node> newNode =
      std::make_shared<Node>(std::move(val), nullptr, d_left);

  if (empty()) {
    d_left = newNode;
    d_right = newNode;
    return;
  }

  d_left->prev() = newNode;
  d_left = newNode;
}

template <typename T>
template <typename... Args>
T &DoubleLinkedList<T>::emplace_front(Args... args) {
  push_front(C(args...));
  return d_left->val();
}

template <typename T> T DoubleLinkedList<T>::pop_front() {
  T val = std::move(d_left->val());
  d_left = d_left->next();

  if (d_left != nullptr) {
    d_right->prev() = nullptr;
  }

  return val;
}
