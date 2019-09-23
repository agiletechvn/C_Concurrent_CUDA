#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

struct data_chunk {
  std::string message;
  data_chunk(std::string msg) : message(msg) {}
};

data_chunk prepare_data(std::string msg = "hello") { return msg; }

void process(data_chunk &chunk) { std::cout << chunk.message << std::endl; }

bool is_last_chunk(data_chunk &) { return true; }

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

bool more_data_to_prepare() { return data_queue.empty(); }

void data_preparation_thread() {
  int ind = 10;
  while (ind-- > 0) {
    data_chunk const data = prepare_data();
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(data);
    data_cond.notify_one();
  }
}

void data_processing_thread() {
  while (true) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [] { return !data_queue.empty(); });
    data_chunk data = data_queue.front();
    data_queue.pop();
    lk.unlock();
    process(data);
    if (is_last_chunk(data))
      break;
  }
}

int main() {

  data_queue.push(data_chunk("hello1"));
  data_queue.push(data_chunk("hello2"));
  data_queue.push(data_chunk("hello3"));

  std::thread t1(data_preparation_thread);
  std::thread t2(data_processing_thread);

  t1.join();
  t2.join();
}
