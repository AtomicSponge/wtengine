/*
  WTEngine
  By:  Matthew Evans
  File:  benchmark.hpp

  See LICENSE.txt for copyright information

  Run a benchmark, recording time elapsed in microseconds to a log file
*/

#ifndef WTE_BENCHMARK_HPP
#define WTE_BENCHMARK_HPP

#include <string>
#include <fstream>
#include <chrono>

namespace wte
{

//! Benchmark class
/*!
  Used to time a block of code
*/
class benchmark {
    public:
        benchmark(std::string);
        void stop(void);
    private:
        std::string benchmark_label;
        std::chrono::system_clock::time_point start, end;
};

//! Benchmark constructor
/*!
  Create a new benchmark
*/
inline benchmark::benchmark(std::string label) {
    benchmark_label = label;
    start = std::chrono::system_clock::now();
}

//! Stop benchmark member
/*!
  Stop benchmark and log to file
*/
inline void benchmark::stop(void) {
    end = std::chrono::system_clock::now();
    std::chrono::system_clock::duration total_time = end - start;
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::ofstream benchmark_log;
    benchmark_log.open("wte_debug\\wte_debug_benchmark_log.txt", std::ios::app);
    benchmark_log << "Benchmark:  " << benchmark_label << std::endl;
    benchmark_log << "Started at:  " << std::ctime(&start_time);
    benchmark_log << "Completed at:  " << std::ctime(&end_time);
    if(total_time == std::chrono::system_clock::duration::zero()) {
        benchmark_log << "Internal clock did not tick during benchmark";
    } else {
        benchmark_log << "Total time:  " <<
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() <<
            " microseconds";
    }
    benchmark_log << std::endl << std::endl;
    benchmark_log.close();
}

} //  namespace wte

#endif
