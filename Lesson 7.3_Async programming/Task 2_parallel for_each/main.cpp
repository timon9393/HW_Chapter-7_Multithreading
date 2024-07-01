#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <vector>

template <typename Iterator, typename Function>
void parallel_for_each(Iterator begin, Iterator end, Function func) {
    const size_t min_block_size = 2;

    if (std::distance(begin, end) <= min_block_size) {
        std::for_each(begin, end, func);
        return;
    }

    Iterator middle = begin + std::distance(begin, end) / 2;

    std::future<void> left_future = std::async(std::launch::async, parallel_for_each<Iterator, Function>, begin, middle, func);
    parallel_for_each(middle, end, func);

    left_future.wait();
}

int main() {
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto square = [](int& number) { number = number * number; };

    parallel_for_each(numbers.begin(), numbers.end(), square);

    for (int number : numbers) {
        std::cout << number << " ";
    }
    std::cout << std::endl;

    return 0;
}