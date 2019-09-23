#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack : std::exception {
  const char *what() const throw() { return "empty stack"; }
};

template <typename T> class threadsafe_stack {
private:
  std::stack<T> data;
  mutable std::mutex m;

public:
  threadsafe_stack() {}
  threadsafe_stack(const threadsafe_stack &other) {
    std::lock_guard<std::mutex> lock(other.m);
    data = other.data;
  }
  threadsafe_stack &operator=(const threadsafe_stack &) = delete;

  void push(T new_value) {
    std::lock_guard<std::mutex> lock(m);
    data.push(new_value);
  }
  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty())
      throw empty_stack();
    // get share value from data
    std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
    data.pop();
    return res;
  }
  void pop(T &value) {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty())
      throw empty_stack();
    value = data.top();
    data.pop();
  }
  bool empty() const {
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
  }
};

int main() {
  threadsafe_stack<int> si;
  si.push(5);
  auto y = si.pop();
  std::cout << *y << std::endl;
  si.push(6);
  if (!si.empty()) {
    int x;
    si.pop(x);

    std::cout << x << std::endl;
  }
}
