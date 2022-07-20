#include <iostream>
#include <math.h>

// Function Declarations
float NumberCubed(float number);

int main()
{
    float number;

    std::cout << "Hello!  This program takes a number and cubes it." << std::endl;

    std::cout << "Enter a number to be cubed: ";
    std::cin >> number;

    std::cout << "Your number " << number << " cubed is " << NumberCubed(number) << std::endl;
}

float NumberCubed(float number)
{
    return pow(number, 3);
}
