#include "file.hpp"
#include "simdjson_inc.h"
#include <filesystem>
#include <unordered_map>
#include <fmt/core.h>
#include "color.h"

namespace std_fs = std::filesystem;

/**
 * Returns true if the file category is in the list
 * of files to be displayed.
 */
static inline bool isInFileListCategory(const std::vector<std::string> &fileList, const char *fileCategory) {
    return (std::find(fileList.begin(), fileList.end(), fileCategory) != fileList.end());
}

/**
 * Returns true if the file/directory can be accessed.
 */
static inline bool canAccess(bool shouldSkipFiles, const char *fileName) {
    return (shouldSkipFiles) ? fileName[0] != '.' : true;
}

/**
 * Iterates over the JSON and maps a file extension to
 * its belonging file name. Returns a map with the
 * extension to file-name pairs.
 */
static std::unordered_map<std::string, std::string> getJSONMap()
{
    std::unordered_map<std::string, std::string> jsonMap;
    simdjson::ondemand::parser parser;
    const auto json {simdjson::padded_string::load("./file.json")};
    simdjson::ondemand::document doc {parser.iterate(json)};
    for (simdjson::ondemand::object obj : doc)
    {
        const std::string docName {obj["name"].get_string().value()};
        auto docExtensions {obj["extensions"]};
        for (auto ext: docExtensions)
        {
            const std::string extStr {ext.get_string().value()};
            jsonMap[extStr] = docName;
        }
    }
    return jsonMap;
}

static inline void incrementFileCount(const std::unordered_map<std::string, std::string> &jsonMap, 
                                      std::unordered_map<std::string, std::size_t> &fileCount,
                                      const char *extension)
{
    if (jsonMap.find(extension) != jsonMap.end()) // If found
        ++fileCount[jsonMap.at(extension)];
    else
        ++fileCount["Other"];
}

/**
 * A variant of 'incrementFileCount'.
 * This is called only when the program
 * has been executed with the '-list=' option.
 * It checks if the file in the list of categories
 * wanted to be displayed by the user, and if it is,
 * it prints it to stdout.
 */
static inline void incrementFileCountAndList(const std::unordered_map<std::string, std::string> &jsonMap, 
                                             const std::vector<std::string> &filesToList,
                                             std::unordered_map<std::string, std::size_t> &fileCount,
                                             const std_fs::path &file)
{
    const std::string ext {file.extension()};
    if (jsonMap.find(ext) != jsonMap.end()) // If found
    {
        const auto languageName {jsonMap.at(ext)};
        if (isInFileListCategory(filesToList, languageName.c_str()))
            fmt::print("{}NOTIFICATION:{} {}\"{}\"{} found in category: {}\n", COLOR_BOLDYELLOW , COLOR_RESET, 
                        COLOR_MAGENTA, file.c_str(), COLOR_RESET, languageName);
        ++fileCount[languageName];
    }
    else
    {
        if (isInFileListCategory(filesToList, "Other"))
            fmt::print("{}NOTIFICATION:{} {}\"{}\"{} found in category: Other\n", COLOR_BOLDYELLOW , COLOR_RESET, 
                        COLOR_MAGENTA, file.c_str(), COLOR_RESET);
        ++fileCount["Other"];
    }
}

/**
 * A variant of 'iterateDirectory'.
 * Does exactly the same thing, except it calls
 * 'iterateDirectoryAndList' instead of 'iterateDirectory'.
 */
static void iterateDirectoryAndList(const std::unordered_map<std::string, std::string> &jsonMap,
                                    const std::vector<std::string> &filesToList,
                                    std::unordered_map<std::string, std::size_t> &fileCount,
                                    const char *directory,
                                    bool skipHiddenFile)
{
    for (const auto &entry: std_fs::directory_iterator(directory))
    {
        if (canAccess(skipHiddenFile, entry.path().filename().c_str()))
        {
            if (entry.is_directory())
                iterateDirectoryAndList(jsonMap, filesToList, fileCount, entry.path().c_str(), skipHiddenFile);
            else
                incrementFileCountAndList(jsonMap, filesToList, fileCount, entry.path());
        }
    }
}

/**
 * A recursive function that iterates over
 * the given directory and increments the
 * file count in the map.
 */
static void iterateDirectory(const std::unordered_map<std::string, std::string> &jsonMap,
                             const char *directory,
                             std::unordered_map<std::string, std::size_t> &fileCount,
                             bool skipHiddenFile)
{
    for (const auto &entry: std_fs::directory_iterator(directory))
    {
        if (canAccess(skipHiddenFile, entry.path().filename().c_str()))
        {
            if (entry.is_directory())
                iterateDirectory(jsonMap, entry.path().c_str(), fileCount, skipHiddenFile);
            else
                incrementFileCount(jsonMap, fileCount, entry.path().extension().c_str());
        }
    }
}

/**
 * Returns a vector sorted based on
 * highest file count.
 */
static std::vector<std::pair<std::string, std::size_t>> getSortedResults(const std::unordered_map<std::string, std::size_t> &fileCount)
{
    std::vector<std::pair<std::string, std::size_t>> results {fileCount.begin(), fileCount.end()};

    // Once the map has been filled with the tallies, we can put it into
    // a sorted vector for the caller to display the results
    std::sort(results.begin(), results.end(), [](const auto &a, auto b){
        return a.second > b.second;
    });
    return results;
}

/**
 * To be called when the user has
 * not listed the '-list=' option.
 * Recursively iterates through the directory,
 * incrementing the file count and then returns
 * a sorted vector to the caller.
 */
std::vector<std::pair<std::string, std::size_t>> getResults(const char *directoryPath, bool skipHiddenFiles) 
{
    std::unordered_map<std::string, std::size_t> fileCount;

    const auto jsonMap {getJSONMap()};
    iterateDirectory(jsonMap, directoryPath, fileCount, skipHiddenFiles);

    return getSortedResults(fileCount);
}

/**
 * A variant of 'getResults'.
 * Does exactly the same thing, except it
 * calls 'iterateDirectoryAndList' instead of
 * 'iterateDirectory'.
 */
std::vector<std::pair<std::string, std::size_t>> getResultsWithListings(const char *directoryPath, 
                                                                        const std::vector<std::string> &filesToList, 
                                                                        bool skipHiddenFiles) 
{
    std::unordered_map<std::string, std::size_t> fileCount;

    const auto jsonMap {getJSONMap()};
    iterateDirectoryAndList(jsonMap, filesToList, fileCount, directoryPath, skipHiddenFiles);

    return getSortedResults(fileCount);
}
