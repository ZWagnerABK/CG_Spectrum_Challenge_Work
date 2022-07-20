#include <iostream>

bool getUserInput(int& input);
bool isLeapYear(int year);

int main()
{
    std::cout << "Hello!  This program finds whether a year entered by a user is a leap year or not." << std::endl << std::endl;
    int input;

    while (!getUserInput(input))
    {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }

    if (isLeapYear(input))
    {
        std::cout << input << " is a leap year!" << std::endl;
    }
    else
    {
        std::cout << input << " is not a leap year!" << std::endl;
    }

}

bool isLeapYear(int year)
{
    if (year % 4 == 0 && year % 400 == 0)
    {
        return true;
    }
    else if (year % 4 == 0 && year % 100 != 0)
    {
        return true;
    }

    return false;
}

bool getUserInput(int& input)
{
    std::cout << "Enter the current year: " << std::endl;
    std::cin >> input;

    if (std::cin.good())
    {
        return true;
    }
    else
    {
        return false;
    }
}
