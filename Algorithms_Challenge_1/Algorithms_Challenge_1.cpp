#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <string>

#define CHRONO_TIME_POINT std::chrono::steady_clock::time_point

void FillArray(int ArrayToFill[], int SizeOfArray);
void BubbleSort(int ArrayToSort[], int SizeOfArray);
void DisplayDuration(CHRONO_TIME_POINT start, CHRONO_TIME_POINT stop);

constexpr int kArraySize1 = 100;
constexpr int kArraySize2 = 1000;
constexpr int kArraySize3 = 10000;
constexpr int kArraySize4 = 50000;

int main()
{
    std::cout << "Hello! This program uses the chrono library to determine the speed difference in the bubble sort algorithm as you increase the entries in the array being sorted." << std::endl;
    std::cout << "Note: This was tested in Debug mode as in Release mode the times tend to be under 1 milisecond for all tests.  Probably because of the statically defined memory.\n\n";

    int DataSet1[kArraySize1];
    int DataSet2[kArraySize2];
    int DataSet3[kArraySize3];
    //int* DataSet4 = new int[kArraySize4]();
    int DataSet4[kArraySize4];

    srand(time(nullptr));

    FillArray(DataSet1, kArraySize1);
    FillArray(DataSet2, kArraySize2);
    FillArray(DataSet3, kArraySize3);
    FillArray(DataSet4, kArraySize4);

    std::cout << "Testing Array 1 (100 items):" << std::endl;
    auto start = std::chrono::steady_clock::now();
    BubbleSort(DataSet1, kArraySize1);
    auto stop = std::chrono::steady_clock::now();

    DisplayDuration(start, stop);

    std::cout << "Testing Array 2 (1000 items):" << std::endl;
    start = std::chrono::steady_clock::now();
    BubbleSort(DataSet2, kArraySize2);
    stop = std::chrono::steady_clock::now();

    DisplayDuration(start, stop);

    std::cout << "Testing Array 3 (10000 items):" << std::endl;
    start = std::chrono::steady_clock::now();
    BubbleSort(DataSet3, kArraySize3);
    stop = std::chrono::steady_clock::now();

    DisplayDuration(start, stop);

    std::cout << "Testing Array 4 (50000 items):" << std::endl;
    start = std::chrono::steady_clock::now();
    BubbleSort(DataSet4, kArraySize4);
    stop = std::chrono::steady_clock::now();

    DisplayDuration(start, stop); 

    //delete DataSet4;
    //DataSet4 = nullptr;
}

void FillArray(int ArrayToFill[], int SizeOfArray)
{
    for (int i = 0; i < SizeOfArray; ++i)
    {
        ArrayToFill[i] = rand() % SizeOfArray + 1;
    }
}

void BubbleSort(int ArrayToSort[], int SizeOfArray)
{
    for (int i = 0; i < SizeOfArray; i++)
    {
        for (int j = 0; j < SizeOfArray - 1; j++)
        {
            if (ArrayToSort[j] > ArrayToSort[j + 1])
            {
                int temp = ArrayToSort[j];
                ArrayToSort[j] = ArrayToSort[j + 1];
                ArrayToSort[j + 1] = temp;
            }
        }
    }
}

void DisplayDuration(CHRONO_TIME_POINT start, CHRONO_TIME_POINT stop)
{
    std::chrono::duration<double> dur = stop - start;

    std::string originalDur = std::to_string(dur.count());

    std::string newDur = "";

    //Get into milliseconds in decimal form.  Conversion to Milliseconds with chrono libraries cuts off right part of the milliseconds.
    //Pretty sure this would break on longer testing.  Would need to come back and rebuild for that.
    if (originalDur[0] != '0')
        newDur += originalDur[0];

    int curIndex = originalDur.find('.') + 1;

    if (originalDur[curIndex] == '0')
    {
        curIndex = curIndex + 1;
        
        if (originalDur[curIndex] == '0')
        {
            curIndex = curIndex + 1;
        }
        else
        {
            newDur += originalDur[curIndex];
            curIndex = curIndex + 1;
        }
    }
    else
    {
        newDur += originalDur[curIndex];
        curIndex = curIndex + 1;
        newDur += originalDur[curIndex];
        curIndex = curIndex + 1;
    }

    newDur += originalDur[curIndex];
    newDur += ".";
    curIndex = curIndex + 1;

    newDur += originalDur.substr(curIndex, 10);

    std::cout << "Operation took " << newDur << " milliseconds.\n\n";
}