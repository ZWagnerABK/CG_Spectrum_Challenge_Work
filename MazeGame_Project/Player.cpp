#include <iostream>

#include "Player.h"
#include "Key.h"
#include "Bomb.h"
#include "AudioManager.h"

using namespace std;

constexpr int kStartingNumberOfLives = 3;

Player::Player()
	: PlacableActor(0, 0)
	, m_pCurrentKey(nullptr)
	, m_pBomb(nullptr)
	, m_money(0)
	, m_lives(kStartingNumberOfLives)
{

}

bool Player::HasKey()
{
	return m_pCurrentKey != nullptr;
}

bool Player::HasKey(ActorColor color)
{
	return true;// HasKey() && m_pCurrentKey->GetColor() == color;
}

void Player::PickupKey(Key* key)
{
	m_pCurrentKey = key;
}

void Player::UseKey()
{
	if (m_pCurrentKey)
	{
		m_pCurrentKey->Remove();
		m_pCurrentKey = nullptr;
	}
}

void Player::DropKey()
{
	if (m_pCurrentKey)
	{
		AudioManager::GetInstance()->PlayKeyDropSound();
		m_pCurrentKey->Place(m_pPosition->x, m_pPosition->y);
		m_pCurrentKey = nullptr;
	}
}


void Player::PickupBomb(Bomb* bomb)
{
	m_pBomb = bomb;
}

bool Player::HasBomb()
{
	return m_pBomb != nullptr;
}

void Player::UseBomb()
{
	if (m_pBomb)
	{
		m_pBomb->Remove();
		m_pBomb = nullptr;
	}
}

void Player::ResetItems()
{
	if (m_pBomb)
	{
		m_pBomb->Remove();
		m_pBomb = nullptr;
	}

	if (m_pCurrentKey)
	{
		m_pCurrentKey->Remove();
		m_pCurrentKey = nullptr;
	}
}

void Player::Draw()
{
	cout << "@";
}
