#pragma once
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <termios.h>
#include <unistd.h>

namespace asker
{
    namespace color{
        const std::string red="\033[31m";
        const std::string blue="\033[34m";
        const std::string green="\033[32m";
        const std::string yellow="\033[33m";
        const std::string white="\033[37m";
        const std::string grey="\033[30;1m";
        const std::string reset="\033[0m";
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
            if (mode != 1 && mode != 2 && mode != 3)
            {
                throw "Invalid mode provided in clearLn";
            }
            return "\033[" + std::to_string(mode) + "K";
        }

        //* show error and move cursor to previous line
        inline void showErr(const std::string &msg)
        {
            std::cout << color::red << msg << color::reset << _utils::mvUp(1) << _utils::mvleft(1000);
        }

        //* move cursor  to the end of the display message
        inline void mvToInput(const int input_msg_length)
        {
            std::cout << _utils::mvRight(input_msg_length);
        }

        //* prints message in proper format
        inline void printMsg(const std::string &msg)
        {
            std::cout << color::green << "? " << color::blue << msg + " " << color::reset;
        }

        //* hide cursor
        inline void hideCursor(){
            std::cout<<"\033[?25l";
        }

        //* show cursor
        inline void showCursor(){
            std::cout<<"\033[?25h";
        }
        //* returns arrow key code else -1
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

    //TODO: add default value option, add input type check
    template <typename T>
    inline T input(const std::string &msg, bool required = false)
    {
        std::string raw_ans;
        const int in_msg_len = msg.length() + 3; // "? " + msg + " "
        T ans;
        _utils::printMsg(msg);
        while (getline(std::cin, raw_ans) && raw_ans.length() == 0 && required)
        {
            _utils::showErr("! this is a required field");
            _utils::mvToInput(in_msg_len);
        }
        // clear error in the next line
        std::cout << _utils::clearLn(_utils::EOL);
        std::cout << color::reset;
        std::istringstream sstream(raw_ans);
        // FIXME: add type check
        sstream >> ans;
        return ans;
    }

    //* yes/no ?
    inline bool confirm(const std::string &msg)
    {
        bool res = true;
        char ans;
        ans = input<char>(msg + " (y/n) ");
        if (ans != 'y' && ans != 'Y' && ans != '\0')
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
        std::cout << color::grey << "(up and down arrow key)" << color::reset << std::endl;
        // print all options
        for (int i = 0; i < n; i++)
        {
            //empty string? then continue
            if (options[i].empty())
                continue;
            if (i == 0)
                std::cout << color::yellow << "> " << color::reset << options[i] << std::endl;
            else
                std::cout << "  " << options[i] << std::endl;
            max_pos += 1;
        }
        // move cursor to first option and extreme left
        std::cout << _utils::mvUp(max_pos) << _utils::mvleft(1000);
        while (std::cin.get(key) && key != '\n')
        {
            int arrKey = _utils::getArrowKey(key);
            switch (arrKey)
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
                    std::cout << color::reset << " " << _utils::mvleft(1000);
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
        std::cout << _utils::mvDown(max_pos - (pos + 1));
        // clear all options
        for (int i = 0; i < max_pos; i++)
        {
            std::cout << _utils::clearLn(_utils::EOL) << _utils::mvUp(1);
        }
        std::cout << _utils::clearLn(_utils::EOL);
        std::cout << color::green << "? " << color::blue << msg << " " << color::reset << ans << std::endl;
        _utils::rawModeOff();
        return ans;
    }

}