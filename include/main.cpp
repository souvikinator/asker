#include <iostream>
#include "asker/asker.hpp"

int main()
{
   // list select demo
   std::cout << "test-1: select list" << std::endl;
   std::string selection;
   std::string options[5] = {"option-1", "op", "option-3", "option-4"};
   selection = asker::selectList("select options:", options);
   std::cout << "user selected: " << selection << std::endl;
   std::cout << std::endl;

   // confirm prompt demo
   std::cout << "test-2: confirm prompt" << std::endl;
   bool exit = asker::confirm("yo! want to exit?");
   std::cout << "wants to exit: " << exit << std::endl;
   std::cout << std::endl;

   // input demo
   std::cout << "test-3: input prompt" << std::endl;
   int m; //default value
   m = asker::input<int>("yo! enter a number:", true);
   std::cout << "user entered number: " << m << std::endl;
   return 0;
}
