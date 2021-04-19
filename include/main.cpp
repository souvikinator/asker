#include<iostream>
#include "asker.hpp"
int main()
{
   std::cout<<"test-1: confirm prompt"<<std::endl;
   char exit;
   exit=asker::confirm("want to exit?",'y','n');
   std::cout<<"user chose:"<<exit<<std::endl;
   return 0;
}
