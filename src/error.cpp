#include "error.hpp"
#include "color.h"
#include <fmt/core.h>

/**
 * Prints an error message to stderr
 * before terminating the program.
 */
void terminateWithError(const char *errorMessage) 
{
    fmt::print(stderr, COLOR_BOLDRED "FATAL ERROR: " COLOR_RESET "{}\n", errorMessage);
    exit(EXIT_FAILURE);
}
