#pragma once
#include <iostream>
#include <limits>
#include <sstream>
#include <typeinfo>
#include <termios.h>
#include <unistd.h>
#include "../termcolor/termcolor.hpp"

namespace asker
{
    namespace _utils
    {
        const int EOL = 2; //End of line
        const int CURSOR_TO_EOL = 0;
        const int CURSOR_TO_SOL = 1; //start of line

        //* to move cursor n char
        inline std::string mvUp(int n)
        {
            // char u[8];
            return "\033[" + std::to_string(n) + "A";
        }

        //* to move cursor n char down
        inline std::string mvDown(int n)
        {
            return "\033[" + std::to_string(n) + "B";
        }

        //* to move cursor n char to left
        inline std::string mvleft(int n)
        {
            return "\033[" + std::to_string(n) + "D";
        }

        //* to move cursor n char to right
        inline std::string mvRight(int n)
        {
            return "\033[" + std::to_string(n) + "C";
        }

        //* to move cursor to the extreme left
        inline std::string mvStart()
        {
            return "\033[1000D";
        }

        //* clear line
        inline std::string clearLn(int mode)
        {
            char cl[8];
            if (mode != 1 && mode != 2 && mode != 3)
            {
                throw "Invalid mode provided in clearLn";
            }
            return "\033[" + std::to_string(mode) + "K";
        }

        inline void showErr(const std::string &msg)
        {
            // move cursor to the input line (above error)
            std::cout << termcolor::red << msg << termcolor::reset << _utils::mvUp(1) << _utils::mvStart();
        }

        inline void mvToInput(const int input_msg_length)
        {
            std::cout << _utils::mvRight(input_msg_length) << termcolor::blue;
        }

        inline int getArrowKey(char key)
        {
            if (iscntrl(key))
            {
                char c1, c2;
                if (key == 27)
                {
                    std::cin.get(c1);
                    std::cin.get(c2);
                    if (c1 != 91)
                        return -1;
                    if (c2 >= 65 && c2 <= 68)
                        return c2;
                }
            }
            return -1;
        }

        //* raw mode stuff
        //? https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
        struct termios orig_termios;
        inline void rawModeOff()
        {
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        }
        inline void rawModeOn()
        {
            tcgetattr(STDIN_FILENO, &orig_termios);
            atexit(rawModeOff);
            struct termios raw = orig_termios;
            raw.c_lflag &= ~(ECHO | ICANON);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        }
    }

    //* add default value option, add input type check
    template <typename T>
    inline T input(const std::string &msg, bool required = false)
    {
        std::string raw_ans;
        const int in_msg_len = msg.length() + 3; // "? " + msg + " "
        T ans;
        std::cout << termcolor::green << "? " << termcolor::reset << termcolor::bold << msg << termcolor::blue;
        while (getline(std::cin, raw_ans) && raw_ans.length() == 0 && required)
        {
            _utils::showErr("! this is a required field");
            //move cursor at the end of input msg
            _utils::mvToInput(in_msg_len);
        }
        // clear error in the next line
        std::cout << _utils::clearLn(_utils::EOL);
        std::cout << termcolor::reset;
        std::istringstream sstream(raw_ans);
        // FIXME: add type check
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

    template <size_t n>
    inline std::string selectList(const std::string &msg, const std::string (&options)[n])
    {
        _utils::rawModeOn();
        char key, c1, c2;
        std::string ans;
        int pos = 0, max_pos = 0;
        std::cout << termcolor::green << "? " << termcolor::blue << msg << termcolor::bright_grey << "(up and down arrow key)" << termcolor::reset << std::endl;
        // print all options
        for (int i = 0; i < n; i++)
        {
            //if constains empty string then continue
            if (options[i].empty())
                continue;
            if (i == 0)
                std::cout << termcolor::yellow << "> " << termcolor::reset << options[i] << std::endl;
            else
                std::cout << "  " << options[i] << std::endl;
            max_pos += 1;
        }
        // move cursor to first option and extreme left
        std::cout << _utils::mvUp(max_pos) << _utils::mvStart();
        while (std::cin.get(key) && key != '\n')
        {
            int arrKey = _utils::getArrowKey(key);
            switch (arrKey)
            {
            case 65: //up
                if (pos > 0)
                {
                    //change ">" to " " of current option
                    std::cout << termcolor::reset << " " << _utils::mvStart();
                    pos -= 1;
                    //change " " to ">" of next option
                    std::cout << _utils::mvUp(1) << termcolor::yellow << ">" << termcolor::reset;
                    std::cout << _utils::mvStart();
                }
                break;
            case 66: //down
                if (pos < max_pos - 1)
                {
                    //change ">" to " " of current option
                    std::cout << termcolor::reset << " " << _utils::mvStart();
                    pos += 1;
                    //change " " to ">" of next option
                    std::cout << _utils::mvDown(1) << termcolor::yellow << ">" << termcolor::reset;
                    std::cout << _utils::mvStart();
                }
                break;
            default:
                break;
            }
        }
        ans = options[pos];
        // mv to end options
        std::cout << _utils::mvDown(max_pos - (pos + 1));
        for (int i = 0; i < max_pos; i++)
        {
            std::cout << _utils::clearLn(_utils::EOL) << _utils::mvUp(1);
        }
        std::cout << _utils::clearLn(_utils::EOL);
        std::cout << termcolor::green << "? " << termcolor::blue << msg << " " << termcolor::reset << ans << std::endl;
        _utils::rawModeOff();
        return ans;
    }

}