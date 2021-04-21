#include <iostream>
#include "asker/asker.hpp"

int main()
{
   std::cout << "test-1: confirm prompt" << std::endl;
   bool exit;
   exit = asker::confirm("yo! want to exit?");
   std::cout << "wants to exit:" << exit << std::endl;
   
   std::cout << "test-2: input prompt" << std::endl;
   int m;//default value
   m=asker::input<int>("yo! enter a number:",true);
   std::cout << "user name is:" << m << std::endl;
   return 0;
}
