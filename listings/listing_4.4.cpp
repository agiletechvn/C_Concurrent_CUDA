#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T> class threadsafe_queue {
private:
  std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;

public:
  void push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
  }

  void wait_and_pop(T &value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    value = data_queue.front();
    data_queue.pop();
  }

  bool empty() { return data_queue.empty(); }
};

struct data_chunk {
  std::string message;
  data_chunk(std::string msg) : message(msg) {}
};

data_chunk prepare_data(std::string msg = "hello") { return msg; }
threadsafe_queue<data_chunk> data_queue;
void process(data_chunk &chunk) { std::cout << chunk.message << std::endl; }
bool is_last_chunk(data_chunk &chunk) { return chunk.message == "last"; }
bool more_data_to_prepare() { return data_queue.empty(); };

void data_preparation_thread() {
  while (more_data_to_prepare()) {
    data_chunk const data = prepare_data("prepare");
    data_queue.push(data);
  }
}

void data_processing_thread() {
  while (true) {
    data_chunk data("empty");
    data_queue.wait_and_pop(data);
    process(data);
    if (is_last_chunk(data))
      break;
  }
}

int main() {

  data_queue.push(data_chunk("hello1"));
  data_queue.push(data_chunk("hello2"));
  data_queue.push(data_chunk("last"));

  std::thread t1(data_preparation_thread);
  std::thread t2(data_processing_thread);

  t1.join();
  t2.join();
}
