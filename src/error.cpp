#include "error.hpp"
#include "color.h"
#include <fmt/core.h>

/**
 * Prints an error message to stderr
 * before terminating the program.
 */
void terminateWithError(const char *errorMessage) 
{
    fmt::print(stderr, "{}FATAL ERROR:{} {}\n", COLOR_BOLDRED, COLOR_RESET, errorMessage);
    exit(EXIT_FAILURE);
}
