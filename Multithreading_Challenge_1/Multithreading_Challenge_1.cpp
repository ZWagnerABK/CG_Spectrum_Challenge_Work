#include <iostream>
#include <thread>
#include <mutex>


bool DidQuit = false;
bool ShouldDecrementLife = false;
std::mutex Mutex;

struct Character
{
    float Position = 0.0f;
    std::atomic<int> Score = 0;
    //int Score = 0;
    int Lives = 1;

    void DisplayStats()
    {
        std::cout << "Lives: " << Lives << std::endl;
    }
};

Character Player;

void UpdateCharacter1()
{
    while (!DidQuit)
    {
        std::cout << "There" << std::endl;
        if (ShouldDecrementLife)
        {
            std::lock_guard<std::mutex> Guard(Mutex);
            if (Player.Lives > 0)
            {

                //std::this_thread::sleep_for(std::chrono::milliseconds(500));
                //std::this_thread::yield();
                --Player.Lives;
            }
        }
    }
}

void UpdateCharacter2()
{
    while (!DidQuit)
    {
        std::cout << "Here" << std::endl;
        if (ShouldDecrementLife)
        {
            std::lock_guard<std::mutex> Guard(Mutex);
            if (Player.Lives > 0)
            {

                //this_thread::sleep_for(chrono::milliseconds(500));
                //std::this_thread::yield();
                --Player.Lives;
            }
        }
    }
}

void ProcessInput()
{
    while (!DidQuit)
    {
        std::cout << "'a' to decrement player life" << std::endl;
        std::cout << "'d' to display player stats" << std::endl;
        std::cout << "'q' to quit" << std::endl;

        char UserInput;
        std::cin >> UserInput;

        switch (UserInput)
        {
        case 'a':
            ShouldDecrementLife = true;
            break;
        case 'd':
            Player.DisplayStats();
            break;
        case 'q':
            DidQuit = true;
            break;
        default:
            break;
        }

        DidQuit = (UserInput == 'q');
    }
}

int main()
{
    std::thread InputHandler(ProcessInput);
    std::thread CharacterUpdate1(UpdateCharacter1);
    std::thread CharacterUpdate2(UpdateCharacter2);

    InputHandler.join();

    return 0;
}