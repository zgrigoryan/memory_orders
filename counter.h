#ifndef COUNTER_H
#define COUNTER_H

#include <chrono>
#include <vector>

namespace counter_tests {

    // Test function for the sequentially consistent counter.
    // Each thread increments the shared counter `iterations` times.
    // Returns the final counter value.
    int test_seq_cst(int iterations, int num_threads);

    // Test function for the counter using relaxed memory order.
    int test_relaxed(int iterations, int num_threads);

    // Test function for the counter using acquire-release semantics.
    int test_acq_rel(int iterations, int num_threads);

    // A helper that runs a test function for a given number of trials and
    // returns a vector with the elapsed time (in nanoseconds) for each trial.
    template<typename Func>
    std::vector<long long> time_function(Func func, int iterations, int num_threads, int trials) {
        std::vector<long long> times;
        for (int t = 0; t < trials; ++t) {
            auto start = std::chrono::high_resolution_clock::now();
            func(iterations, num_threads);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            times.push_back(duration);
        }
        return times;
    }
}

#endif // COUNTER_H
