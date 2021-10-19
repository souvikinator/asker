#include <functional>
#include <iostream>

#include "../include/asker/asker.hpp"

// input demo with validation
bool custom_validation(std::string ans) {
  if (ans != "helloworld") return false;
  return true;
}

int main() {
  char pw[MAXLen] = {0};
  char *p = pw;
  FILE *fp = stdin;
  ssize_t nchar = 0;

  // confirm prompt demo
  std::cout << "demo-1: confirm prompt" << std::endl;
  bool exit = asker::confirm("yo! want to exit?");
  std::cout << "wants to exit: " << exit << std::endl;
  std::cout << std::endl;

  // list select demo
  std::cout << "demo-2: select list" << std::endl;
  std::string selection;
  std::string options[5] = {"option-1", "op", "option-3", "option-4"};
  selection = asker::selectList("select options:", options);
  std::cout << "user selected: " << selection << std::endl;
  std::cout << std::endl;

  std::cout << "demo-2: select list" << std::endl;
  std::vector<std::string> checkSelection;
  checkSelection = asker::checkList("select options:", options);
  std::cout << "user selected: [";
  for (int i = 0; i < checkSelection.size(); i++) {
    std::cout << "{" << checkSelection[i] << "}";
  }
  std::cout << "]";
  std::cout << std::endl;

  // basic input demo
  std::cout << "demo-3: basic input prompt" << std::endl;
  std::string m;  // default value
  m = asker::input("yo! enter name:");
  std::cout << "user entered: " << m << std::endl;
  std::cout << std::endl;

  // input demo with required field
  std::cout << "demo-4: required input prompt" << std::endl;
  m = asker::input("yo! enter name:", true);
  std::cout << "user entered: " << m << std::endl;
  std::cout << std::endl;

  std::cout << "demo-5: input prompt with validation" << std::endl;
  // NOTE: can also pass anonymous function in place of custom_validation
  m = asker::input("yo! enter name:", custom_validation, false);
  std::cout << "user entered: " << m << std::endl;
  std::cout << std::endl;

  // input password demo
  std::cout << "demo-6: masked input" << std::endl;
  std::string pass = asker::maskedInput("Enter password:", true);
  std::cout << "password=" << pass << std::endl;

  // input password demo with different mask char
  std::cout << "demo-6: masked input (with different mask character)"
            << std::endl;
  pass = asker::maskedInput("Enter password:", true, '#');
  std::cout << "password=" << pass << std::endl;

  // input using text editor
  std::cout << "demo-7: editor input" << std::endl;
  m = asker::editor("Enter some text in the editor:");
  std::cout << "user entered: " << m << std::endl;
  return 0;
}
