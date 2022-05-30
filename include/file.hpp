#ifndef USEDLANG_FILE_HPP
#define USEDLANG_FILE_HPP

#include <vector>
#include <string>

extern std::vector<std::pair<std::string, std::size_t>> getResults(const char *directoryPath, bool skipHidden);
extern std::vector<std::pair<std::string, std::size_t>> getResultsWithListings(const char *directoryPath, 
                                                                               const std::vector<std::string> &filesToList, 
                                                                               bool skipHidden);

#endif // USEDLANG_FILE_HPP
