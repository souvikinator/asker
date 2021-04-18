#include<iostream>
#include<termios.h>
#include "termcolor.hpp"

int main(){
   std::cout<<termcolor::red<<"hello world"<<std::endl; 
   return 0;
}
