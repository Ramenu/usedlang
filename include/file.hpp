#ifndef USEDLANG_FILE_HPP
#define USEDLANG_FILE_HPP

#include <vector>
#include <string_view>

extern std::vector<std::pair<std::string_view, std::size_t>> getResults(const char *directoryPath);

#endif // USEDLANG_FILE_HPP
