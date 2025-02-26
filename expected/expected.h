#include <functional>
#include <iostream>
#include <optional>

template <typename T, typename E> class Expected {
public:
  Expected() : d_val(std::nullopt), d_unexpected(std::nullopt) {}
  Expected(T value) : d_val(std::move(value)), d_unexpected(std::nullopt) {}
  Expected(E error) : d_val(std::nullopt), d_unexpected(std::move(error)) {}

  const T &value() const;
  const T &value_or(const T &or_value) const;
  T value_or(T &&or_value);

  const E &error() const;
  const E &error_or(const E &or_value) const;
  E error_or(E &&or_value);

  bool has_value() const;
  explicit operator bool() const;

  void operator=(T value);
  void operator=(E error);

  bool operator==(const Expected<T, E> &other) const;
  bool operator!=(const Expected<T, E> &other) const;
  const T *operator->() const;
  const T &operator*() const;

  template <typename F> auto transform(F &&fn) const;
  template <typename F> auto transform_error(F &&fn) const;

private:
  std::optional<T> d_val;
  std::optional<E> d_unexpected;
};

template <typename T, typename E> const T *Expected<T, E>::operator->() const {
  return &value();
}

template <typename T, typename E> const T &Expected<T, E>::operator*() const {
  return value();
}

template <typename T, typename E>
template <typename F>
auto Expected<T, E>::transform(F &&fn) const {
  using R = decltype(std::invoke(std::forward<F>(fn), value()));
  Expected<R, E> res;

  if (*this) {
    res = std::invoke(std::forward<F>(fn), value());
  } else {
    res = error();
  }
  return res;
}

template <typename T, typename E>
template <typename F>
auto Expected<T, E>::transform_error(F &&fn) const {
  using R = decltype(std::invoke(std::forward<F>(fn), error()));
  Expected<T, R> res;

  if (*this) {
    res = value();
  } else {
    res = std::invoke(std::forward<F>(fn), error());
  }

  return res;
}

template <typename T, typename E>
bool Expected<T, E>::operator!=(const Expected<T, E> &other) const {
  return !(*this == other);
}

template <typename T, typename E>
bool Expected<T, E>::operator==(const Expected<T, E> &other) const {
  if (has_value() != other.has_value()) {
    return false;
  } else if (has_value()) {
    return d_val == other.d_val;
  } else {
    return d_unexpected == other.d_unexpected;
  }
}

template <typename T, typename E> void Expected<T, E>::operator=(T value) {
  d_val = std::move(value);
}

template <typename T, typename E> void Expected<T, E>::operator=(E error) {
  d_unexpected = std::move(error);
}

template <typename T, typename E> Expected<T, E>::operator bool() const {
  return has_value();
}

template <typename T, typename E> bool Expected<T, E>::has_value() const {
  return d_val.has_value();
}

template <typename T, typename E> const T &Expected<T, E>::value() const {
  return d_val.value();
}

template <typename T, typename E>
const T &Expected<T, E>::value_or(const T &or_value) const {
  return d_val.value_or(or_value);
}

template <typename T, typename E> T Expected<T, E>::value_or(T &&or_value) {
  return d_val.value_or(std::forward<T>(or_value));
}

template <typename T, typename E> const E &Expected<T, E>::error() const {
  return d_unexpected.value();
}

template <typename T, typename E>
const E &Expected<T, E>::error_or(const E &or_value) const {
  return d_unexpected.value_or(or_value);
}

template <typename T, typename E> E Expected<T, E>::error_or(E &&or_value) {
  return d_unexpected.value_or(std::forward<E>(or_value));
}
