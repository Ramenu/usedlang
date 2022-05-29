#include "file.hpp"
#include "simdjson_inc.h"
#include <filesystem>
#include <unordered_map>
#include <future>

namespace std_fs = std::filesystem;

static inline simdjson::ondemand::document getDocument(simdjson::ondemand::parser &parser, const char *filePath) {
    const auto json {simdjson::padded_string::load(filePath)};
    simdjson::ondemand::document doc {parser.iterate(json)};
    return doc;
}

static void incrementFileCount(simdjson::ondemand::parser &parser, 
                               std::unordered_map<std::string, std::size_t> &fileCount,
                               const char *extension)
{
    auto doc {getDocument(parser, "./file.json")};
    bool foundExtension {false};
    for (simdjson::ondemand::object obj: doc)
    {
        auto docExtensions {obj["extensions"]};
        for (auto ext: docExtensions)
        {
            const std::string_view extStr {ext.get_string().value()};
            if (extStr == extension)
            {
                foundExtension = true;
                ++fileCount[std::string{obj["name"].get_string().value()}];
                return;
            }
        }
    }
    if (!foundExtension)
        ++fileCount["Other"];
}

static void iterateDirectory(simdjson::ondemand::parser &parser,
                             const char *directory,
                             std::unordered_map<std::string, std::size_t> &fileCount)
{
    for (const auto &entry: std_fs::directory_iterator(directory))
    {
        if (entry.is_directory())
            iterateDirectory(parser, entry.path().c_str(), fileCount);
        else
            incrementFileCount(parser, fileCount, entry.path().extension().c_str());
    }
}

std::vector<std::pair<std::string_view, std::size_t>> getResults(const char *directoryPath) 
{
    std::unordered_map<std::string, std::size_t> fileCount;
    // Load the JSON Document
    simdjson::ondemand::parser parser;
    iterateDirectory(parser, directoryPath, fileCount);

    // Once the map has been filled with the tallies, we can put it into
    // a sorted vector for the caller to display the results
    std::vector<std::pair<std::string_view, std::size_t>> results {fileCount.begin(), fileCount.end()};
    std::sort(results.begin(), results.end(), [](const auto &a, const auto &b){
        return a.second > b.second;
    });
    return results;
}
