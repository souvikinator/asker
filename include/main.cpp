#include <iostream>
#include "asker/asker.hpp"

int main()
{
   std::cout << "test-1: confirm prompt" << std::endl;
   bool exit;
   exit = asker::confirm("yo! want to exit?");
   std::cout << "wants to exit:" << exit << std::endl;
   
   std::cout << "test-2: input prompt" << std::endl;
   std::string name;
   name = asker::input("yo! what yo name?");
   std::cout << "user name is:" << name << std::endl;
   return 0;
}
