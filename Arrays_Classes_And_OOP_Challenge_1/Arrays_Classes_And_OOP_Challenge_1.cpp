#include <iostream>

int findLargestElement(int container[], int size);
void populateExampleArray(int container[], int size);

int main()
{
    std::cout << "Hello!  This program will display the largest number in an array of integers generated randomly within a range of 1 - 100.\n";
    //Needed to cycle the seed using the current time at execution in seconds as a seeding value.
    srand(time(0));

    int exampleSize = (rand() % 100) + 1;
    int* example = new int[exampleSize];

    populateExampleArray(example, exampleSize);
    int maxElement = findLargestElement(example, exampleSize);

    std::cout << "\nThe largest element in the array is " << maxElement << "!\n";
}

void populateExampleArray(int container[], int size)
{
    std::cout << "\nPopulating the array (Size " << size << ") of elements : ";
    std::cout << "\n[";
    for (int i = 0; i < size; i++)
    {
        int randomNumber = (rand() % 100) + 1;

        if (i + 1 < size)
            std::cout << randomNumber << ", ";
        else
            std::cout << randomNumber;

        container[i] = randomNumber;
    }

    std::cout << "]\n\n";
    std::cout << "Finished Populating!\n";
}

int findLargestElement(int container[], int size)
{
    int max = container[0];

    for (int i = 1; i < size; i++)
    {
        if (container[i] > max)
            max = container[i];
    }

    return max;
}