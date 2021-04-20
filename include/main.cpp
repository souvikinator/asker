#include <iostream>
#include "asker.hpp"

int main()
{
   /**
   * compile and run ./main
   * test by entering y or n
   * test by entering any other char ;)
   */
   std::cout<<"test-1: confirm prompt"<<std::endl;
   char exit;
   exit=asker::confirm("want to exit?");
   std::cout<<"user chose:"<<exit<<std::endl;
   return 0;
}
