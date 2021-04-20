#pragma once
#include <iostream>
#include <limits>
#include "termcolor.hpp"

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
    // TODO: remove redundant code
    // FIXME: breaks when user enters a string rather than char
    char inline confirm(const std::string &msg)
    {
        char res;
        std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::reset << "  [y/n] " << termcolor::blue;
        // TODO: only take one char as input
        while (std::cin>>res && (res!='y' && res!='n'))
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << termcolor::red << ">> invalid input (y/n)" << termcolor::reset;
            // move to start=>up=>clear_line=>print lien
            std::cout<<_utils::mvStart<<_utils::mvUp<1><<_utils::clearLn<_utils::EOL>;
            std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::reset << "  [y/n] " << termcolor::blue;
        }
        std::cout << _utils::clearLn<_utils::EOL>;
        std::cout << termcolor::reset;
        return res;
    }
}