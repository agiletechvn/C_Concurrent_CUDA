#include <atomic>
#include <future>
#include <iostream>
#include <vector>

template <typename T> class lock_free_stack {
private:
  struct node {
    std::shared_ptr<T> data;
    node *next;
    node(T const &data_) : data(std::make_shared<T>(data_)) {}
  };
  std::atomic<node *> head;

public:
  void push(T const &data) {
    node *const new_node = new node(data);
    new_node->next = head.load();
    while (!head.compare_exchange_weak(new_node->next, new_node))
      ;
  }
  std::shared_ptr<T> pop() {
    node *old_head = head.load();
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
      ;
    return old_head ? old_head->data : std::shared_ptr<T>();
  }
};

// try with future with 8 threads to test lock_free data structure without using
// concurrent queue
int main() {
  lock_free_stack<int> stack;
  // run parallel
  stack.push(12);
  std::cout << *stack.pop() << std::endl;
  //  auto start_time = std::chrono::system_clock::now();
  //  for (auto &e : futures)
  //    e.wait();
  //  auto eclipsed = std::chrono::duration_cast<std::chrono::milliseconds>(
  //                      std::chrono::system_clock::now() - start_time)
  //                      .count();
}
