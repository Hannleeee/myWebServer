#include <iostream>

int stoi(char *str) {
    int num = 0;
    while (*str != '\0') {
        num = 10*num + (*str - '0');
        ++str;
    }
    return num;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: operand1 operator operand2\n";
        exit(1);
    }

    int a = stoi(argv[1]);
    int b = stoi(argv[3]);
    switch (*argv[2])
    {
    case '+':
        std::cout << a+b << "\n";
        break;
    
    case '-':
        std::cout << a-b << "\n";
        break;
    
    case '*':
        std::cout << a*b << "\n";
        break;
    
    case '/':
        std::cout << a/b << "\n";
        break;

    default:
        break;
    }
    exit(0);
}