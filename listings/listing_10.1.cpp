#include <execution>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <vector>

class X {
  mutable std::mutex m;
  int data;


public:
  X() : data(0) {}
  int get_value() const {
    std::lock_guard guard(m);
    return data;
  }
  void increment() {
    std::lock_guard guard(m);
    ++data;
  }
};

void increment_all(std::vector<X> &v) {

  std::for_each(std::execution::par, v.begin(), v.end(),
                [](X &x) { x.increment(); });
}

int main() {
  std::vector<X> list(5);
  increment_all(list);
  for (auto const &item : list)
    std::cout << item.get_value() << ", ";
  std::cout << std::endl;
  return 0;
}
