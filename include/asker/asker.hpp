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
            // std::string r;
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
        int pos = 0;
        std::cout << termcolor::green << "? " << termcolor::blue << msg << termcolor::bright_grey << "(up and down arrow key)" << termcolor::reset << std::endl;
        // print all options
        for (int i = 0; i < n; i++)
        {
            if (i == 0)
                std::cout << termcolor::yellow << "> " << termcolor::reset << options[i] << std::endl;
            else
                std::cout << "  " << options[i] << std::endl;
            pos += 1;
        }
        // move cursor to first option and extreme left
        std::cout << _utils::mvUp(pos) << _utils::mvStart();
        pos = 0;
        while (std::cin.get(key))
        {
            if (iscntrl(key))
            { //TODO: cleanup!
                if (key == 27)
                {
                    std::cin.get(c1);
                    std::cin.get(c2);
                    if (c1 != 91)
                        continue;
                    switch (c2)
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
                        if (pos < (n - 1))
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
                // enter
                if (key == 10)
                {
                    // FIXME:
                    ans = options[pos];
                    std::cout << _utils::mvDown(n - (pos + 1));
                    // mv to end options
                    for (int i = 0; i < n - 1; i++)
                    {
                        std::cout << _utils::clearLn(_utils::EOL) << _utils::mvUp(1);
                    }
                    std::cout << ans << std::endl;
                    break;
                }
            }
        }
        _utils::rawModeOff();
        return ans;
    }

}