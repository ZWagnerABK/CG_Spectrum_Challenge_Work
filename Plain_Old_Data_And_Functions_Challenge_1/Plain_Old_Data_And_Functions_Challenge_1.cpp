#include <iostream>

int main()
{
    float num1;
    float num2;
    float num3;

    std::cout << "Hello!  This program will find the Sum and Average of 3 numbers." << std::endl << std::endl;

    std::cout << "Enter the first number: ";
    std::cin >> num1;

    std::cout << "Enter the second number: ";
    std::cin >> num2;

    std::cout << "Enter the third number: ";
    std::cin >> num3;

    float sum = num1 + num2 + num3;
    float average = sum / 3.0f;

    std::cout << "The Sum of your entered numbers is " << sum << std::endl;
    std::cout << "The Average of your entered numbers is " << average << std::endl;

}