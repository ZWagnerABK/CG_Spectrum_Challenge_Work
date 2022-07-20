#include <iostream>
#include <math.h>

void getUserInput();
void getAndDisplayFactors(int number);

int main()
{
    std::cout << "Hello!  This program asks the user to input a positive, non-zero integer number and will find and display its factors." << std::endl;

    getUserInput();

    return 0;
}

void getUserInput()
{
    int input = -1;

    do
    {
        std::cout << "\nEnter a positive, non-zero integer (type 0 to end the program): " << std::endl;
        std::cin >> input;

        if (std::cin.fail() || input < 0)
        {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            input = -1;
        }
        else if (input > 0)
        {
            getAndDisplayFactors(input);
        }

    } while (input != 0);
}

void getAndDisplayFactors(int number)
{
    std::cout << "\nThe factors for " << number << "(represented in pairs) are:\n";

    //For the range of inputs,  1 and the number given will be the first factors.
    //Also for sake of simplicity,  will display the factors in pairs.
    std::cout << "1 * " << number << std::endl;

    for (int i = 2; i <= sqrt(number); i++)
    {
        if (number % i == 0)
            std::cout << i << " * " << number / i << std::endl;
    }
}