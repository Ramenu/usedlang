#ifndef SIMDJSON_INC_H
#define SIMDJSON_INC_H

#if (defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)) && (!defined(__llvm__) && !defined(__INTEL_COMPILER))
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wuseless-cast"  
    #pragma GCC diagnostic ignored "-Wsuggest-override"  
    #pragma GCC diagnostic ignored "-Wextra-semi"
    #include "simdjson.h"
    #pragma GCC diagnostic pop
#elif defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wpadded"
    #pragma clang diagnostic ignored "-Wsign-conversion"
    #pragma clang diagnostic ignored "-Wsuggest-override"
    #pragma clang diagnostic ignored "-Wambiguous-reversed-operator"
    #include "simdjson.h"
    #pragma clang diagnostic pop
#else
    #include "simdjson.h"
#endif 


#endif // SIMDJSON_INC_H