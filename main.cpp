#include "counter.h"
#include "kaizen.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <string>


int main(int argc, char* argv[]) {

    zen::cmd_args args(argv, argc);

    if (!args.is_present("--iterations") || !args.is_present("--trials")) {
        std::cerr << "Usage: " << argv[0] << " --iterations <iterations> --trials <trials> [--verbose]" << std::endl;
        return 1;
    }

    int iterations = std::stoi(args.get_options("--iterations")[0]);
    int trials = std::stoi(args.get_options("--trials")[0]);
    int num_threads = 8; // default number of threads
    if (args.is_present("--threads")) {
        num_threads = std::stoi(args.get_options("--threads")[0]);
    }
    bool verbose = args.is_present("--verbose");

    using namespace counter_tests;

    auto times_seq_cst = time_function(test_seq_cst, iterations, num_threads, trials);

    int expected = num_threads * iterations;
    int final_seq_cst = test_seq_cst(iterations, num_threads);

    std::cout << "Final counter values (expected " << expected << "):\n";
    std::cout << "  seq_cst: " << final_seq_cst << "\n";

    auto avg = [](const std::vector<long long>& v) -> long long {
        return std::accumulate(v.begin(), v.end(), 0LL) / v.size();
    };

    long long avg_seq_cst = avg(times_seq_cst);

    // Write results to CSV
    std::ofstream csv("results.csv");
    csv << "Trial,SeqCst(ns)\n";
    for (size_t i = 0; i < times_seq_cst.size(); ++i) {
        csv << (i + 1) << "," 
            << times_seq_cst[i] << "\n";
    }
    csv << "Average," << avg_seq_cst << "\n";
    csv.close();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "| " << std::left << std::setw(35) << "SeqCst operations avg (ns)"
       << " | " << std::right << std::setw(12) << avg_seq_cst << " |\n";
   
    zen::print(ss.str());

    if (verbose) {
        std::cout << "\nTrial Results:\n";
        for (size_t i = 0; i < times_seq_cst.size(); ++i) {
            std::cout << "Trial " << (i + 1) 
                      << ": SeqCst = " << times_seq_cst[i]
                      << " ns\n";
        }
    }

    return 0;
}
