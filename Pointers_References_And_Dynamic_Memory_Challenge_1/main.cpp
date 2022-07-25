#include <iostream>

void GetUserInput(int& storage, std::string inputMessage);

std::string g_GenericInputErrorMessage = "Error, input invalid!";

int main()
{
    std::cout << "Hello!  This is a program to show my understanding of pointers by taking in input variables and displaying them by pointers.\n\n";

    int a = 0;
    int b = 0;
    std::string inputMessage = "";

    inputMessage = "Enter a value for 'a': ";
    GetUserInput(a, inputMessage);
    inputMessage = "Enter a value for 'b': ";
    GetUserInput(b, inputMessage);

    int* ptrA = &a;
    int* ptrB = &b;

    std::cout << "\nThis is the value of 'a' stored in a pointer: " << *ptrA << std::endl;
    std::cout << "\nThis is the value of 'b' stored in a pointer: " << *ptrB << std::endl;
}

void GetUserInput(int& storage, std::string inputMessage)
{
    std::cout << inputMessage;
    std::cin >> storage;

    while (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        std::cout << g_GenericInputErrorMessage << std::endl;
        std::cout << inputMessage;
        std::cin >> storage;
    }
}