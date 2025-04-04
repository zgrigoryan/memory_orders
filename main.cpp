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

    // Time each test function
    auto times_seq_cst = time_function(test_seq_cst, iterations, num_threads, trials);
    auto times_relaxed = time_function(test_relaxed, iterations, num_threads, trials);
    auto times_acq_rel = time_function(test_acq_rel, iterations, num_threads, trials);

    // Verify correctness (each should equal num_threads * iterations)
    int expected = num_threads * iterations;
    int final_seq_cst = test_seq_cst(iterations, num_threads);
    int final_relaxed = test_relaxed(iterations, num_threads);
    int final_acq_rel = test_acq_rel(iterations, num_threads);

    std::cout << "Final counter values (expected " << expected << "):\n";
    std::cout << "  seq_cst: " << final_seq_cst << "\n";
    std::cout << "  relaxed: " << final_relaxed << "\n";
    std::cout << "  acq_rel: " << final_acq_rel << "\n\n";

    // Compute average times for each test variant
    auto avg = [](const std::vector<long long>& v) -> long long {
        return std::accumulate(v.begin(), v.end(), 0LL) / v.size();
    };

    long long avg_seq_cst = avg(times_seq_cst);
    long long avg_relaxed = avg(times_relaxed);
    long long avg_acq_rel = avg(times_acq_rel);

    // Write results to CSV
    std::ofstream csv("results.csv");
    csv << "Trial,SeqCst(ns),Relaxed(ns),AcqRel(ns)\n";
    for (size_t i = 0; i < times_seq_cst.size(); ++i) {
        csv << (i + 1) << "," 
            << times_seq_cst[i] << "," 
            << times_relaxed[i] << "," 
            << times_acq_rel[i] << "\n";
    }
    csv << "Average," << avg_seq_cst << "," << avg_relaxed << "," << avg_acq_rel << "\n";
    csv.close();

    // Nicely formatted output using a stringstream
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "| " << std::left << std::setw(35) << "SeqCst operations avg (ns)"
       << " | " << std::right << std::setw(12) << avg_seq_cst << " |\n";
    ss << "| " << std::left << std::setw(35) << "Relaxed operations avg (ns)"
       << " | " << std::right << std::setw(12) << avg_relaxed << " |\n";
    ss << "| " << std::left << std::setw(35) << "AcqRel operations avg (ns)"
       << " | " << std::right << std::setw(12) << avg_acq_rel << " |\n";
    
    ss << "| " << std::left << std::setw(35) << "Speedup (seq_cst / relaxed)"
       << " | " << std::right << std::setw(12) << static_cast<double>(avg_seq_cst) / avg_relaxed << " |\n";
    ss << "| " << std::left << std::setw(35) << "Speedup (seq_cst / acq_rel)"
       << " | " << std::right << std::setw(12) << static_cast<double>(avg_seq_cst) / avg_acq_rel << " |\n";

    zen::print(ss.str());

    if (verbose) {
        std::cout << "\nTrial Results:\n";
        for (size_t i = 0; i < times_seq_cst.size(); ++i) {
            std::cout << "Trial " << (i + 1) 
                      << ": SeqCst = " << times_seq_cst[i]
                      << " ns, Relaxed = " << times_relaxed[i]
                      << " ns, AcqRel = " << times_acq_rel[i] << " ns\n";
        }
    }

    return 0;
}
