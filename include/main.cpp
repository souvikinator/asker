#include <iostream>
#include <functional>
#include "asker/asker.hpp"

int main()
{
   char pw[MAXLen] = {0};
   char *p = pw;
   FILE *fp = stdin;
   ssize_t nchar = 0;
   
   // confirm prompt demo
   std::cout << "test-2: confirm prompt" << std::endl;
   bool exit = asker::confirm("yo! want to exit?");
   std::cout << "wants to exit: " << exit << std::endl;
   std::cout << std::endl;

   // list select demo
   std::cout << "test-1: select list" << std::endl;
   std::string selection;
   std::string options[5] = {"option-1", "op", "option-3", "option-4"};
   selection = asker::selectList("select options:", options);
   std::cout << "user selected: " << selection << std::endl;
   std::cout << std::endl;

   //basic input demo
   std::cout << "test-3: input prompt" << std::endl;
   std::string m; //default value
   m = asker::input<std::string>("yo! enter name:");
   std::cout << "user entered number: " << m << std::endl;
   std::cout << std::endl;

   // input demo with validator
   
   // input demo with required field
	
   // input demo password input 
   std::cout << "test-4 password input" << std::endl;
   std::cout << "Enter password: ";
   nchar = asker::getpasswd (&p, MAXLen, '*', fp);
   std::cout << "\nyou entered : "<< p << std::endl;
   std::cout << std::endl;

   // input demo password hidden
   std::cout << "test-5 password input" << std::endl;
   std::cout << "Enter password: ";
   nchar = asker::getpasswd (&p, MAXLen, 0, fp);
   std::cout << "\nyou entered : "<< p << std::endl;   
   std::cout << std::endl;

   return 0;
}
