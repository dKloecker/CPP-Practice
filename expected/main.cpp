#include "expected.h"
#include <cassert>
#include <iostream>
#include <string>

void test_construction() {
  std::cout << "Testing construction..." << std::endl;

  Expected<int, std::string> success(42);
  assert(success.has_value());
  assert(success.value() == 42);

  Expected<int, std::string> failure("error");
  assert(!failure.has_value());
  assert(failure.error() == "error");
}

void test_value_operations() {
  std::cout << "Testing value operations..." << std::endl;

  Expected<int, std::string> success(42);
  assert(success.value() == 42);
  assert(success.value_or(0) == 42);

  Expected<int, std::string> failure("error");
  assert(failure.value_or(0) == 0);

  // Test assignment
  success = 100;
  assert(success.value() == 100);
}

void test_error_operations() {
  std::cout << "Testing error operations..." << std::endl;

  Expected<int, std::string> failure("error message");
  assert(failure.error() == "error message");
  assert(failure.error_or("default") == "error message");

  Expected<int, std::string> success(42);
  assert(success.error_or("default") == "default");

  // Test assignment
  failure = "new error";
  assert(failure.error() == "new error");
}

void test_boolean_operations() {
  std::cout << "Testing boolean operations..." << std::endl;

  Expected<int, std::string> success(42);
  assert(success.has_value());
  assert(bool(success));

  Expected<int, std::string> failure("error");
  assert(!failure.has_value());
  assert(!bool(failure));
}

void test_equality() {
  std::cout << "Testing equality operations..." << std::endl;

  Expected<int, std::string> success1(42);
  Expected<int, std::string> success2(42);
  Expected<int, std::string> success3(43);

  Expected<int, std::string> failure1("error");
  Expected<int, std::string> failure2("error");
  Expected<int, std::string> failure3("different error");

  assert(success1 == success2);
  assert(!(success1 == success3));
  assert(failure1 == failure2);
  assert(!(failure1 == failure3));
  assert(!(success1 == failure1));
}

void test_complex_types() {
  std::cout << "Testing with complex types..." << std::endl;

  struct ComplexType {
    int x;
    std::string y;
    bool operator==(const ComplexType &other) const {
      return x == other.x && y == other.y;
    }
  };

  Expected<ComplexType, std::string> success({42, "test"});
  assert(success.has_value());
  assert(success.value().x == 42);
  assert(success.value().y == "test");

  Expected<ComplexType, std::string> failure("complex error");
  assert(!failure.has_value());
  assert(failure.error() == "complex error");
}

void test_transform() {
  std::cout << "Testing with transform()..." << std::endl;
  auto fn = [](int a) {
    if (a == 0) {
      return false;
    } else {
      return true;
    }
  };

  Expected<int, std::string> has_value{5};
  Expected<int, std::string> does_not_have_value{"Error Message"};

  auto result_1 = has_value.transform(fn);
  auto result_2 = does_not_have_value.transform(fn);

  assert(result_1.has_value());
  assert(result_1.value() == true);

  assert(!result_2.has_value());
  assert(result_2.error() == does_not_have_value.error());
}

void test_transform_error() {
  std::cout << "Testing with transform_error()..." << std::endl;

  auto error_handler = [](const std::string &error) {
    return true; // Convert any error to false
  };

  Expected<int, std::string> success(100);
  Expected<int, std::string> failure("error message");

  auto result1 = success.transform_error(error_handler);
  auto result2 = failure.transform_error(error_handler);

  assert(result1.has_value());
  assert(result1.value() == 100); // Original value preserved

  assert(!result2.has_value());
  assert(result2.error() == true); // Error handled and converted
}

void test_dereference_operators() {
  std::cout << "Testing dereference operators..." << std::endl;

  struct TestStruct {
    int x = 42;
    std::string str = "test";
  };

  Expected<TestStruct, std::string> success(TestStruct{});

  // Test arrow operator
  assert(success->x == 42);
  assert(success->str == "test");

  // Test dereference operator
  const TestStruct &ref = *success;
  assert(ref.x == 42);
  assert(ref.str == "test");

  // Test with primitive type
  Expected<int, std::string> num_success(123);
  assert(*num_success == 123);
}

int main() {
  try {
    test_construction();
    test_value_operations();
    test_error_operations();
    test_boolean_operations();
    test_equality();
    test_complex_types();
    test_transform();
    test_transform_error();
    test_dereference_operators();

    std::cout << "All tests passed successfully!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Test failed with exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
