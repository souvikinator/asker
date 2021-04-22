#include <iostream>
#include "asker/asker.hpp"

int main()
{

   std::cout<<"test-1: select prompt"<<std::endl;
   std::string selection;
   std::string options[4]={"option-1","option-2","options-3","option-4"};
   selection=asker::selectList("select options:",options);
   std::cout<<"user selected:"<<selection<<std::endl;
   std::cout<<std::endl;
   std::cout << "test-2: confirm prompt" << std::endl;
   bool exit;
   exit = asker::confirm("yo! want to exit?");
   std::cout << "wants to exit:" << exit << std::endl;
   // std::cout<<std::endl;
   // std::cout << "test-3: input prompt" << std::endl;
   // int m;//default value
   // m=asker::input<int>("yo! enter a number:",true);
   // std::cout << "user name is:" << m << std::endl;
   return 0;
}
