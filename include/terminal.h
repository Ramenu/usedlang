#ifndef TERMINAL_H
#define TERMINAL_H

    /* Only compile if on Windows */
    #if defined(__MINGW32__) || defined(__WIN32__) || defined(WIN32) || defined(_WIN32)

        #include <windows.h>
        #include <stdbool.h>

        #define TERMINAL_COLOR_DARKBLUE 1
        #define TERMINAL_COLOR_GREEN 2
        #define TERMINAL_COLOR_CYAN 3
        #define TERMINAL_COLOR_RED 4
        #define TERMINAL_COLOR_PURPLE 5
        #define TERMINAL_COLOR_YELLOW 6
        #define TERMINAL_COLOR_WHITE 7
        #define TERMINAL_COLOR_GRAY 8
        #define TERMINAL_COLOR_BLUE 9
        #define TERMINAL_COLOR_BRIGHTGREEN 10
        #define TERMINAL_COLOR_BRIGHTCYAN 11
        #define TERMINAL_COLOR_BRIGHTRED 12
        #define TERMINAL_COLOR_BRIGHTPURPLE 13
        #define TERMINAL_COLOR_BRIGHTYELLOW 14
        #define TERMINAL_COLOR_BRIGHTWHITE  15

        /**
         * Enables the specified color in the terminal.
         */
        void enable_terminal_color(WORD color)
        {
            HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h_console, color);
        }

        /**
         * Enables virtual terminal processing, which enables
         * ASCI colors on Windows consoles, and maybe some
         * other side-effects.
         */
        bool enable_virtual_terminal_processing()
        {
            HANDLE h_output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dw_mode = 0;

            GetConsoleMode(h_output, &dw_mode);
            dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

            if (!SetConsoleMode(h_output, dw_mode))
                return false;
            return true;
        }
    #endif // (__MINGW32__)|(_WIN32)|(__WIN32__)
#endif // TERMINAL_H