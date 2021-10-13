# Asker Documentation

## asker::confirm

confirm prompt

Usage:

```cpp
bool exit = asker::confirm("yo! want to exit?");
std::cout << "wants to exit: " << exit << std::endl;
```

## asker::selectList

list select

Usage:

```cpp
std::string selection;
std::string options[5] = {"option-1", "op", "option-3", "option-4"};
selection = asker::selectList("select options:", options);
std::cout << "user selected: " << selection << std::endl;
std::cout << std::endl;
```

## asker::checkList

checklist

Usage:

```cpp
std::vector<std::string> checkSelection;
checkSelection = asker::checkList("select options:", options);
```

## asker::input

### Basic input

Usage:

```cpp
std::string m;
m = asker::input("yo! enter name:");
```

### Required field input

Usage:

```cpp
std::cout << "demo-4: required input prompt" << std::endl;
m = asker::input("yo! enter name:", true);
```

### Input with validation

Usage:

```cpp
std::cout << "demo-5: input prompt with validation" << std::endl;
m = asker::input("yo! enter name:", custom_validation, false);
```

### Input Password

Usage:

```cpp
std::cout << "demo-6: masked input" << std::endl;
std::string pass = asker::maskedInput("Enter password:", true);
```

### Input password with mask character

Usage:

```cpp
std::cout << "demo-6: masked input (with different mask character)" << std::endl;
pass = asker::maskedInput("Enter password:", true, '#');
```

