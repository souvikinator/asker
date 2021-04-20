#pragma once
#include <iostream>
#include <limits>
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
    }
    
    inline bool confirm(const std::string &msg)
    {
        bool res;
        std::string ans;
        std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::reset << " [y/n] " << termcolor::blue;
        getline(std::cin,ans);
        if (ans.length()==0 || ans[0] == 'y' || ans[0] == 'Y')
            res = true;
        else
            res = false;
        std::cout << termcolor::reset;
        return res;
    }

    inline std::string input(const std::string &msg)
    {
        /*
         & for validation 
         std::cout << termcolor::red << ">> invalid input (y/n)" << termcolor::reset;
         std::cout << _utils::mvStart << _utils::mvUp<1> << _utils::clearLn<_utils::EOL>;
         std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::reset << "  [y/n] " << termcolor::blue;
        */
        std::string ans;
        std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << " " << termcolor::blue;
        getline(std::cin,ans);
        std::cout << termcolor::reset;
        return ans;
    }
}