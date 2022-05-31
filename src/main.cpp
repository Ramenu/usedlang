#include "error.hpp"
#include "terminal.h"
#include <filesystem>
#include "file.hpp"
#include <fmt/core.h>
#include "color.h"

#if defined(__MINGW32__) || defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
    #define WINDOWS_OS
    #include <Libloaderapi.h>
#elif defined(__linux__)
    #include <libgen.h>         
    #include <unistd.h>         
    #include <linux/limits.h>   
#endif
namespace std_fs = std::filesystem;

static inline size_t getSum(const std::vector<std::pair<std::string, std::size_t>> &results)
{
    size_t sum {};
    for (const auto &pair: results)
        sum += pair.second;
    return sum;
}

static inline constexpr double getPercentage(size_t numerator, size_t denominator) {
    return static_cast<double>(numerator)/static_cast<double>(denominator)*100.0;
}

static inline constexpr bool isInQuotations(std::string_view str) {
    return str.starts_with('\"') && str.ends_with('\"');
}

/**
 * Given a list of arguments, this function returns a 
 * vector with all of the file categories the user wants
 * to list.
 */
static std::vector<std::string> gatherFilesToList(std::vector<std::string_view> &args)
{
    static constexpr int offset {6};
    static constexpr int twoQuotations {2};
    std::vector<std::string> filesToList;
    for (std::size_t i {}; i < args.size(); ++i)
    {
        if (isInQuotations(args[i]) && args[i].length() > twoQuotations)
            args[i] = args[i].substr(args[i].front() + 1, args[i].back() - 1);
        if (args[i].length() > offset)
            if (args[i].substr(0, offset) == "-list=")
                filesToList.emplace_back(args[i].substr(offset, args[i].back() - offset));
    }
    return filesToList;
}


int main(int argc, char **argv)
{
    // Enable virtual terminal processing on Windows machines (for colored console output support)
    #ifdef WINDOWS_OS
        enable_virtual_terminal_processing();
    #endif
    if (!(argc > 1))
        terminateWithError("Provide a directory as an argument.");
    if (!std_fs::is_directory(argv[1]))
        terminateWithError("Invalid or nonexistent directory given. Double check to make sure it exists and is valid.");
    
    // Fill args into vector
    std::vector<std::string_view> args;
    for (int i {1}; i < argc; ++i)
        args.emplace_back(argv[i]);
    const auto filesToList {gatherFilesToList(args)};
    const bool skipHiddenFiles {std::find(args.begin(), args.end(), "-skipHidden") != args.end()};
    const char *absolutePath;
    #ifdef __linux__
        char result[PATH_MAX];
        ssize_t count {readlink("/proc/self/exe", result, PATH_MAX)};
        if (count != -1)
            absolutePath = dirname(result);
    /* WARNING: WINDOWS CODE UNTESTED */
    #elif defined(WINDOWS_OS)
        GetModuleFileName(NULL, absolutePath, MAX_PATH);
    #endif
    
    std::vector<std::pair<std::string, std::size_t>> results;
    if (filesToList.empty())
        results = getResults(argv[1], skipHiddenFiles, absolutePath);
    else
        results = getResultsWithListings(argv[1], filesToList, skipHiddenFiles, absolutePath);
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