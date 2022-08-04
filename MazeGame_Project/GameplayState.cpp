#include "GameplayState.h"

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <assert.h>

#include "Enemy.h"
#include "Key.h"
#include "Door.h"
#include "Money.h"
#include "Goal.h"
#include "AudioManager.h"
#include "Utility.h"
#include "Shield.h"
#include "StateMachineExampleGame.h"
#include "CollisionHelper.h"

using namespace std;

constexpr int kArrowInput = 224;
constexpr int kLeftArrow = 75;
constexpr int kRightArrow = 77;
constexpr int kUpArrow = 72;
constexpr int kDownArrow = 80;
constexpr int kEscapeKey = 27;
constexpr int kSpaceBar = 32;
constexpr int kStaticHUDTopBottomBorderWidth = 75;

GameplayState::GameplayState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_DidBeatLevel(false)
	, m_skipFrameCount(0)
	, m_currentLevel(0)
	, m_pLevel(nullptr)
{
	m_LevelNames.push_back("Shield_Testing_Level.txt");
	m_LevelNames.push_back("MazeGame_Level1.txt");
	m_LevelNames.push_back("MazeGame_Level2.txt");
	m_LevelNames.push_back("MazeGame_Level3.txt");
}

GameplayState::~GameplayState()
{
	delete m_pLevel;
	m_pLevel = nullptr;
}

bool GameplayState::Load()
{
	if (m_pLevel)
	{
		delete m_pLevel;
		m_pLevel = nullptr;
	}

	m_pLevel = new Level();
	
	return m_pLevel->Load(m_LevelNames.at(m_currentLevel), m_player.GetXPositionPointer(), m_player.GetYPositionPointer());

}

void GameplayState::Enter()
{
	Load();
}

void GameplayState::ProcessInput()
{
	int input = _getch();
	int arrowInput = 0;
	int newPlayerX = m_player.GetXPosition();
	int newPlayerY = m_player.GetYPosition();

	// One of the arrow keys were pressed
	if (input == kArrowInput)
	{
		arrowInput = _getch();
	}

	if ((input == kArrowInput && arrowInput == kLeftArrow) ||
		(char)input == 'A' || (char)input == 'a')
	{
		newPlayerX--;
	}
	else if ((input == kArrowInput && arrowInput == kRightArrow) ||
		(char)input == 'D' || (char)input == 'd')
	{
		newPlayerX++;
	}
	else if ((input == kArrowInput && arrowInput == kUpArrow) ||
		(char)input == 'W' || (char)input == 'w')
	{
		newPlayerY--;
	}
	else if ((input == kArrowInput && arrowInput == kDownArrow) ||
		(char)input == 'S' || (char)input == 's')
	{
		newPlayerY++;
	}
	else if (input == kEscapeKey)
	{
		m_DidBeatLevel = true;
		m_CVSleep.notify_all();
		m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
	}
	else if ((char)input == 'Z' || (char)input == 'z')
	{
		m_player.DropKey();
	}

	// If position never changed
	if (newPlayerX == m_player.GetXPosition() && newPlayerY == m_player.GetYPosition() && input != kSpaceBar)
	{
		//return false;
	}
	else
	{
		HandleCollision(newPlayerX, newPlayerY, true);
	}
}

void GameplayState::CheckBeatLevel()
{
	if (m_DidBeatLevel)
	{
		++m_skipFrameCount;
		if (m_skipFrameCount > kFramesToSkip)
		{
			m_DidBeatLevel = false;
			m_skipFrameCount = 0;
			++m_currentLevel;
			if (m_currentLevel == m_LevelNames.size())
			{
				Utility::WriteHighScore(m_player.GetMoney());

				AudioManager::GetInstance()->PlayWinSound();

				m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Win);
			}
			else
			{
				// On to the next level
				Load();
				m_player.ResetItems();
			}

		}
	}
}

bool GameplayState::Update(bool processInput)
{
	if (!m_DidBeatLevel && processInput)
	{
		// TODO:  Add Draw Thread seperate from Update
		std::thread RedrawThread([this] { this->RedrawThreadingInitial(); });
		std::thread ProcessInputThread([this] { this->ProcessInputThreadingInitial(); });

		RedrawThread.join();
		ProcessInputThread.join();
	}

	CheckBeatLevel();

	return false;
}

void GameplayState::RedrawThreadingInitial()
{
	while (!m_DidBeatLevel && m_player.GetLives() >= 0)
	{
		std::unique_lock<std::mutex> CVSleepGuard(m_CVSleepGuard);
		m_CVSleep.wait_for(CVSleepGuard, 1000ms, [this] { return this->m_DidBeatLevel == true; });

		HandleCollision(m_player.GetXPosition(), m_player.GetYPosition());
		Draw();
	}
}

void GameplayState::ProcessInputThreadingInitial()
{
	while (!m_DidBeatLevel && m_player.GetLives() >= 0)
	{
		ProcessInput();
		Draw();
	}
}

void GameplayState::HandleCollision(int newPlayerX, int newPlayerY, bool processInput)
{
	std::lock_guard<std::mutex> CollisionGuard(m_CollisionGuard);
	if (!m_DidBeatLevel && !processInput)
	{
		PlacableActor* collidedActor = m_pLevel->UpdateActors(newPlayerX, newPlayerY);
		if (collidedActor != nullptr)
		{
			CollisionHelper::CollisionInfo collisionState =
			{
				m_player,
				m_DidBeatLevel,
				newPlayerX,
				newPlayerY,
				StateMachineExampleGame::SceneName::None
			};

			CollisionHelper::HandleCollision(collidedActor, collisionState);
			m_DidBeatLevel = collisionState.didBeatLevel;
			m_CVSleep.notify_all();

			if (collisionState.newScene != StateMachineExampleGame::SceneName::None)
			{
				m_pOwner->LoadScene(collisionState.newScene);
			}
		}
	}
	else if (!m_DidBeatLevel && processInput)
	{
		PlacableActor* collidedActor = m_pLevel->CheckForCollidedActor(newPlayerX, newPlayerY);
		if (collidedActor != nullptr)
		{
			CollisionHelper::CollisionInfo collisionState =
			{
				m_player,
				m_DidBeatLevel,
				newPlayerX,
				newPlayerY,
				StateMachineExampleGame::SceneName::None
			};

			CollisionHelper::HandleCollision(collidedActor, collisionState);
			m_DidBeatLevel = collisionState.didBeatLevel;
			m_CVSleep.notify_all();

			if (collisionState.newScene != StateMachineExampleGame::SceneName::None)
			{
				m_pOwner->LoadScene(collisionState.newScene);
			}
		}
		else if (m_pLevel->IsSpace(newPlayerX, newPlayerY)) // no collision
		{
			m_player.SetPosition(newPlayerX, newPlayerY);
		}
		else if (m_pLevel->IsWall(newPlayerX, newPlayerY))
		{
			// wall collision, do nothing
		}
	}
}

void GameplayState::Draw()
{
	std::lock_guard<std::mutex> DrawGuard(m_DrawGuard);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");

	m_pLevel->Draw();

	// Set cursor position for player 
	COORD actorCursorPosition;
	actorCursorPosition.X = m_player.GetXPosition();
	actorCursorPosition.Y = m_player.GetYPosition();
	SetConsoleCursorPosition(console, actorCursorPosition);
	m_player.Draw();

	// Set the cursor to the end of the level
	COORD currentCursorPosition;
	currentCursorPosition.X = 0;
	currentCursorPosition.Y = m_pLevel->GetHeight();
	SetConsoleCursorPosition(console, currentCursorPosition);

	DrawHUD(console);
}

void GameplayState::DrawHUD(const HANDLE& console)
{
	cout << endl;

	// Top Border
	/*for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;*/

	//Making Top Border static for now
	for (int i = 0; i < kStaticHUDTopBottomBorderWidth; ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;

	// Left Side border
	cout << Level::WAL;

	cout << " wasd-move " << Level::WAL << " z-drop key " << Level::WAL << " spacebar-wait " << Level::WAL;
	

	cout << " $:" << m_player.GetMoney() << " " << Level::WAL;
	cout << " lives:" << m_player.GetLives() << " " << Level::WAL;
	cout << " key:";
	if (m_player.HasKey())
	{
		m_player.GetKey()->Draw();
		cout << Level::WAL;
	}
	else
	{
		cout << " " << Level::WAL;
	}

	cout << " shield:";
	if (m_player.HasShield())
	{
		m_player.GetShield()->Draw();
	}
	else
	{
		cout << " ";
	}

	// RightSide border
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console, &csbi);

	COORD pos;
	//pos.X = m_pLevel->GetWidth() - 1;
	pos.X = kStaticHUDTopBottomBorderWidth - 1;
	pos.Y = csbi.dwCursorPosition.Y;
	SetConsoleCursorPosition(console, pos);

	cout << Level::WAL;
	cout << endl;

	// Bottom Border
	/*for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;*/

	//Make Bottom Border Static for now
	for (int i = 0; i < kStaticHUDTopBottomBorderWidth; ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;
}