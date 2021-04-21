#pragma once
#include <iostream>
#include <limits>
#include <sstream>
#include <typeinfo>
#include "../termcolor/termcolor.hpp"

namespace asker
{
    namespace _utils
    {
        const int EOL = 2; //End of line
        const int CURSOR_TO_EOL = 0;
        const int CURSOR_TO_SOL = 1; //start of line

        //* to move cursor n char
        template <uint8_t n>
        inline std::ostream &mvUp(std::ostream &stream)
        {
            char u[8];
            std::sprintf(u, "\033[%dA", n);
            stream << u;
            return stream;
        }

        //* to move cursor n char down
        template <uint8_t n>
        inline std::ostream &mvDown(std::ostream &stream)
        {
            char d[8];
            sprintf(d, "\033[%dB", n);
            stream << d;
            return stream;
        }

        //* to move cursor n char to left
        template <uint8_t n>
        inline std::ostream &mvleft(std::ostream &stream)
        {
            char l[8];
            sprintf(l, "\033[%dD", n);
            stream << l;
            return stream;
        }

        //* to move cursor n char to right
        template <uint8_t n>
        inline std::ostream &mvRight(std::ostream &stream)
        {
            char r[8];
            sprintf(r, "\033[%dC", n);
            stream << r;
            return stream;
        }

        //* to move cursor to the extreme left
        inline std::ostream &mvStart(std::ostream &stream)
        {
            stream << "\033[1000D";
            return stream;
        }

        //* clear line
        template <uint8_t mode>
        inline std::ostream &clearLn(std::ostream &stream)
        {
            char cl[8];
            if (mode != 1 && mode != 2 && mode != 3)
            {
                throw "Invalid mode provided in clearLn";
            }
            sprintf(cl, "\033[%dK", mode);
            stream << cl;
            return stream;
        }

        inline void showErr(const std::string &msg)
        {
            std::cout << termcolor::red << msg << termcolor::reset;
            // move cursor to the input line (above error)
            std::cout << _utils::mvStart << _utils::mvUp<1> << _utils::clearLn<_utils::EOL>;
        }
    }

    //* add default value option, add input type check
    template <typename T>
    inline T input(const std::string &msg, bool required = false)
    {
        std::string raw_ans;
        T ans;
        std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::blue;
        while (getline(std::cin, raw_ans) && raw_ans.length() == 0 && required)
        {
            _utils::showErr("! this is a required field");
            std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::blue;
        }
        // clear error in the next line
        std::cout << _utils::clearLn<_utils::EOL>;
        std::cout << termcolor::reset;
        std::istringstream sstream(raw_ans);
        sstream >> ans;
        return ans;
    }

    inline bool confirm(const std::string &msg)
    {
        bool res = true;
        char ans;
        ans = input<char>(msg + " [y/n] ");
        if (ans != 'y' && ans != 'Y' && ans != '\0')
            res = false;
        return res;
    }

}
/*
         & for validation 
         std::cout << termcolor::red << ">> invalid input (y/n)" << termcolor::reset;
         std::cout << _utils::mvStart << _utils::mvUp<1> << _utils::clearLn<_utils::EOL>;
         std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::reset << "  [y/n] " << termcolor::blue;
        */
