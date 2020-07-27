#include <cassert>
#include <istream>
#include <thread>
#include <iostream>

#include "thread_pool.hpp"

void test_smth() {
    utils::thread_pool tp(2);

    auto temp_occupy_one_thread_task = tp.submit([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        return std::this_thread::get_id();
    });

    std::atomic<utils::thread_pool::task_t<std::thread::id> *> B;

    auto A = tp.submit([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        assert(B.load()->get() == std::this_thread::get_id());
        return std::this_thread::get_id();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto blocker_task = tp.submit([]() { return std::this_thread::get_id(); });
    B.store(&blocker_task);

    assert(A.get() != std::this_thread::get_id());
}


int main() {
    test_smth();

    return 0;
}