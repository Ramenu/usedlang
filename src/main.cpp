#include "error.hpp"
#include "terminal.h"
#include <filesystem>
#include "file.hpp"
#include <fmt/core.h>
#include "color.h"

namespace std_fs = std::filesystem;

static inline size_t getSum(const std::vector<std::pair<std::string_view, std::size_t>> &results)
{
    size_t sum {};
    for (const auto &pair: results)
        sum += pair.second;
    return sum;
}

static inline constexpr double getPercentage(size_t numerator, size_t denominator) {
    return static_cast<double>(numerator)/static_cast<double>(denominator)*100.0;
}


int main(int argc, char **argv)
{
    // Enable virtual terminal processing on Windows machines (for colored console output support)
    #if defined(__MINGW32__) || defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
        enable_virtual_terminal_processing();
    #endif
    if (!(argc > 1))
        terminateWithError("Provide a directory as an argument.");
    if (!std_fs::is_directory(argv[1]))
        terminateWithError("Invalid or nonexistent directory given. Double check to make sure it exists and is valid.");
    
    const auto results {getResults(argv[1])};
    const size_t sum {getSum(results)};

    static constexpr const char *selectableColors[11] {
        COLOR_RED,
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_YELLOW,
        COLOR_MAGENTA,
        COLOR_MAGENTA,
        COLOR_CYAN,
        COLOR_CYAN,
        COLOR_GREEN,
        COLOR_GREEN,
        COLOR_BOLDGREEN
    };
    fmt::print("\nResults\n-------\n");
    for (const auto&pair: results)
    {
        const double percentage {getPercentage(pair.second, sum)};
        const size_t tier {static_cast<size_t>(percentage/10)};
        fmt::print("{}{:.3}%{} instances of {} files found\n", selectableColors[tier], percentage, COLOR_RESET, pair.first);
    }
    return EXIT_SUCCESS;
}