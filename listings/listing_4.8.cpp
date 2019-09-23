#include <future>
#include <string>
#include <vector>

// define class template
template <typename T> class packaged_task;

template <> class packaged_task<std::string(std::vector<char> *, int)> {
public:
  template <typename Callable> explicit packaged_task(Callable &&f);
  std::future<std::string> get_future();
  void operator()(std::vector<char> *, int);
};

int main() {}
