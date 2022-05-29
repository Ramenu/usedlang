#ifndef SIMDJSON_INC_H
#define SIMDJSON_INC_H

#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wuseless-cast"  
    #pragma GCC diagnostic ignored "-Wsuggest-override"  
    #pragma GCC diagnostic ignored "-Wextra-semi"
    #include "simdjson.h"
    #pragma GCC diagnostic pop
#else
    #include "simdjson.h"
#endif 


#endif // SIMDJSON_INC_H