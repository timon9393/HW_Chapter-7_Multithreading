#include <iostream>
#include <mutex>

class Data {
public:
    Data(int value) : value(value) {}

    int value;
    std::mutex mutex;
};

// lock
void swap_with_lock(Data& data1, Data& data2) {
    std::lock(data1.mutex, data2.mutex);
    std::lock_guard lg1(data1.mutex, std::adopt_lock);
    std::lock_guard lg2(data2.mutex, std::adopt_lock);
    std::swap(data1.value, data2.value);    
}

// scoped_lock
void swap_with_scoped_lock(Data& data1, Data& data2) {
    std::scoped_lock lock(data1.mutex, data2.mutex);
    std::swap(data1.value, data2.value);
}

// unique_lock
void swap_with_unique_lock(Data& data1, Data& data2) {
    std::unique_lock lock1(data1.mutex, std::defer_lock);
    std::unique_lock lock2(data2.mutex, std::defer_lock);
    std::lock(lock1, lock2);
    std::swap(data1.value, data2.value);
}

int main() {
    Data data1(10);
    Data data2(20);

    std::cout << "Data1: " << data1.value << std::endl;
    std::cout << "Data2: " << data2.value << std::endl;
    std::cout << "After swap:" << std::endl;

    swap_with_lock(data1, data2);
    //swap_with_scoped_lock(data1, data2);
    //swap_with_unique_lock(data1, data2);

    std::cout << "Data1: " << data1.value << std::endl;
    std::cout << "Data2: " << data2.value << std::endl;

    return 0;
}