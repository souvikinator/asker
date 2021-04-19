#pragma once
#include<iostream>
#include "termcolor.hpp"

namespace asker{
    // TODO: make it a template?
    char inline confirm(std::string msg,char op1,char op2){
        char* fmtMsg=new char(sizeof(msg));
        char res;
        std::sprintf(fmtMsg,"%s [%c/%c] ",msg.c_str(),op1,op2);
        std::cout<<termcolor::green<<"? "<<termcolor::reset<<fmtMsg<<termcolor::blue;
        std::cin>>res;
        //* add check if res= op1 or op2
        //! res is neither op1 not op2=> show error in next line=>
        //! cursor back to input line
        std::cout<<termcolor::reset;
        return res;
    }
}