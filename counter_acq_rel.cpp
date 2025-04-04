#include "counter.h"
#include <atomic>
#include <thread>
#include <vector>

namespace counter_tests {

    int test_acq_rel(int iterations, int num_threads) {
        std::atomic<int> counter(0);
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&counter, iterations]() {
                for (int j = 0; j < iterations; ++j) {
                    counter.fetch_add(1, std::memory_order_acq_rel);
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }
        return counter.load(std::memory_order_acq_rel);
    }
}
