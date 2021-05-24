#pragma once
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// macros
#define MAXLen 32
namespace asker
{
    namespace color
    {
        const std::string red = "\033[31m";
        const std::string blue = "\033[34m";
        const std::string green = "\033[32m";
        const std::string yellow = "\033[33m";
        const std::string white = "\033[37m";
        const std::string grey = "\033[30;1m";
        const std::string reset = "\033[0m";
    }
    namespace _utils
    {
        const int EOL = 2; //End of line
        const int CURSOR_TO_EOL = 0;
        const int CURSOR_TO_SOL = 1; //start of line

        //* to move cursor n char
        inline std::string mvUp(int n)
        {
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

        //* clear line
        inline std::string clearLn(int mode)
        {
            char cl[8];
            if (mode < 1 && mode > 3)
            {
                throw "Invalid mode provided in clearLn";
            }
            return "\033[" + std::to_string(mode) + "K";
        }

        //* show error and move cursor to previous line
        inline void showErr(const std::string &msg)
        {
            // clear existing error
            std::cout << _utils::clearLn(_utils::EOL);
            std::cout << color::red << msg << color::reset;
            std::cout << _utils::mvUp(1) << _utils::mvleft(1000);
        }

        //* prints message in proper format
        inline void printMsg(const std::string &msg)
        {
            // clear previous message
            std::cout << _utils::clearLn(_utils::EOL);
            std::cout << color::green << "? " << color::blue << msg + " " << color::reset;
        }

        //* hide cursor
        inline void hideCursor()
        {
            std::cout << "\033[?25l";
        }

        //* show cursor
        inline void showCursor()
        {
            std::cout << "\033[?25h";
        }
        //* returns key code
        // TODO: convert to general key detection function
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
            showCursor();
        }
        inline void rawModeOn()
        {
            tcgetattr(STDIN_FILENO, &orig_termios);
            atexit(rawModeOff);
            struct termios raw = orig_termios;
            raw.c_lflag &= ~(ECHO | ICANON);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
            hideCursor();
        }
    }

    //* input prompt with required field
    inline std::string input(
        const std::string &msg, bool required = false)
    {
        std::string ans;
        _utils::printMsg(msg);
        while (getline(std::cin, ans))
        {
            if (required && ans.length() == 0)
            {
                _utils::showErr("! this is a required field");
                _utils::printMsg(msg);
            }
            else
            {
                break;
            }
        }
        // clear error in the next line
        std::cout << _utils::clearLn(_utils::EOL) << color::reset;
        return ans;
    }

    //* Input prompt with validation
    inline std::string input(
        const std::string &msg, std::function<bool(std::string)> validate, bool required = false)
    {
        std::string ans;
        _utils::printMsg(msg);
        while (getline(std::cin, ans))
        {
            if (!validate(ans))
            {
                _utils::showErr("! invalid input");
                _utils::printMsg(msg);
            }
            else if (required && ans.empty())
            {
                _utils::showErr("! this is a required field");
                _utils::printMsg(msg);
            }
            else
            {
                break;
            }
        }
        // clear error in the next line
        std::cout << _utils::clearLn(_utils::EOL) << color::reset;
        return ans;
    }

    //* confirm prompt
    inline bool confirm(const std::string &msg)
    {
        bool res = true;
        std::string ans;
        ans = input(msg + " (y/n) ");
        if (ans[0] != 'y' && ans[0] != 'Y' && ans[0] != '\0')
            res = false;
        return res;
    }

    //* select from list of options
    template <size_t n>
    inline std::string selectList(const std::string &msg, const std::string (&options)[n])
    {
        _utils::rawModeOn();
        char key;
        std::string ans;
        int pos = 0, max_pos = 0;
        _utils::printMsg(msg);
        std::cout << color::grey << "(up and down arrow key)" << color::reset << "\n";
        // print all options
        for (int i = 0; i < n; i++)
        {
            if (options[i].empty())
                continue;
            if (i == 0)
                std::cout << color::yellow << "> " << color::reset << options[i] << "\n";
            else
                std::cout << "  " << options[i] << "\n";
            max_pos += 1;
        }
        // move cursor to first option
        std::cout << _utils::mvUp(max_pos);
        while (std::cin.get(key) && key != '\n')
        {
            int arrow_Key = _utils::getArrowKey(key);
            switch (arrow_Key)
            {
            case 65: //up
                if (pos > 0)
                {
                    //change ">" to " " of current option
                    std::cout << color::reset << " " << _utils::mvleft(1000);
                    pos -= 1;
                    //change " " to ">" of next option
                    std::cout << _utils::mvUp(1) << color::yellow << ">" << color::reset;
                    std::cout << _utils::mvleft(1000);
                }
                break;
            case 66: //down
                if (pos < max_pos - 1)
                {
                    //change ">" to " " of current option
                    std::cout << color::reset << " " << _utils::mvleft(1);
                    pos += 1;
                    //change " " to ">" of next option
                    std::cout << _utils::mvDown(1) << color::yellow << ">" << color::reset;
                    std::cout << _utils::mvleft(1000);
                }
                break;
            default:
                break;
            }
        }
        ans = options[pos];
        // mv to end options
        int diff = max_pos - (pos + 1);
        if (diff > 0)
            std::cout << _utils::mvDown(diff);
        // clear all the options
        while (max_pos--)
        {
            std::cout << _utils::clearLn(_utils::EOL) << _utils::mvUp(1);
        }
        std::cout << _utils::clearLn(_utils::EOL); //clear message line
        std::cout << color::green << "? " << color::blue << msg << " " << color::reset << ans << "\n";
        _utils::rawModeOff();
        return ans;
    }

    //* password input
    // FIXME: backspace not working
    std::string maskedInput(const std::string &msg, bool required = false, char symbol = '*')
    {
        std::string ans;
        char c;
        _utils::printMsg(msg);
        _utils::rawModeOn();
        while (c = std::getchar())
        {
            if (c != '\n')
            {
                ans += c;
                std::cout << symbol;
            }
            // FIXME: raw-mode improper printing
            else if (c == '\n' && required && ans.empty())
            {
                _utils::showErr("! This is a required field");
                _utils::printMsg(msg);
            }
            else if (c == '\n' && !ans.empty())
            {
                _utils::rawModeOff();
                break;
            }
        }
        // clear error in the next line
        // std::cout << "\nrequired=" << required << ",ans.length()=" << ans.length() << std::endl;
        std::cout << _utils::clearLn(_utils::EOL) << color::reset;
        return ans;
    }
}
