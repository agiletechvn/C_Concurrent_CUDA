#include <iostream>
#include <thread>

void hello(std::string message) { std::cout << message << std::endl; }

class background_task {
public:
  void operator()() const { hello("helloworld from background_task"); }
};

int main() {
  background_task f;
  std::thread t(f);
  t.join();
}
