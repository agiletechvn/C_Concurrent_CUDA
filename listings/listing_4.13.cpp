#include <algorithm>
#include <future>
#include <iostream>
#include <list>

template <typename T> std::list<T> parallel_quick_sort(std::list<T> input) {
  if (input.empty()) {
    return input;
  }
  std::list<T> result;
  result.splice(result.begin(), input, input.begin());
  T const &pivot = *result.begin();
  //  std::cout << "pivot " << pivot << std::endl;
  auto divide_point = std::partition(input.begin(), input.end(),
                                     [&](T const &t) { return t < pivot; });
  std::list<T> lower_part;
  lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
  std::future<std::list<T>> new_lower(
      std::async(&parallel_quick_sort<T>, std::move(lower_part)));
  auto new_higher(parallel_quick_sort(std::move(input)));
  result.splice(result.end(), new_higher);
  result.splice(result.begin(), new_lower.get());
  return result;
}

int main() {
  std::list<int> list = {10, 211, 30, 25, 70, 29};
  auto output = parallel_quick_sort(list);
  for (auto const &item : output)
    std::cout << item << ", ";
  std::cout << std::endl;
  return 0;
}
