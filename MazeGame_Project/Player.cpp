#include <iostream>

#include "Player.h"
#include "Key.h"
#include "Shield.h"
#include "AudioManager.h"

using namespace std;

constexpr int kStartingNumberOfLives = 3;

Player::Player()
	: PlacableActor(0, 0)
	, m_pCurrentKey(nullptr)
	, m_pShield(nullptr)
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
	return HasKey() && m_pCurrentKey->GetColor() == color;
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

		switch (m_lastDirection)
		{
			case LAST_DIRECTION::NONE:
				m_pCurrentKey->Place(m_pPosition->x, m_pPosition->y);
				break;
			case LAST_DIRECTION::UP:
				m_pCurrentKey->Place(m_pPosition->x, m_pPosition->y - 1);
				break;
			case LAST_DIRECTION::DOWN:
				m_pCurrentKey->Place(m_pPosition->x, m_pPosition->y + 1);
				break;
			case LAST_DIRECTION::LEFT:
				m_pCurrentKey->Place(m_pPosition->x - 1, m_pPosition->y);
				break;
			case LAST_DIRECTION::RIGHT:
				m_pCurrentKey->Place(m_pPosition->x + 1, m_pPosition->y);
				break;
			default:
				m_pCurrentKey->Place(m_pPosition->x, m_pPosition->y);
		}
		
		m_pCurrentKey = nullptr;
	}
}

void Player::PickupShield(Shield* shield)
{
	m_pShield = shield;
}

bool Player::HasShield()
{
	return m_pShield != nullptr;
}

void Player::UseShield()
{
	if (m_pShield)
	{
		m_pShield->Remove();
		m_pShield = nullptr;
	}
}

void Player::ResetItems()
{
	if (m_pShield)
	{
		m_pShield->Remove();
		m_pShield = nullptr;
	}

	if (m_pCurrentKey)
	{
		m_pCurrentKey->Remove();
		m_pCurrentKey = nullptr;
	}
}

void Player::SetPosition(int x, int y)
{
	if (x > m_pPosition->x && y == m_pPosition->y)
	{
		m_lastDirection = LAST_DIRECTION::LEFT;
	}
	else if (x == m_pPosition->x && y > m_pPosition->y)
	{
		m_lastDirection = LAST_DIRECTION::UP;
	}
	else if (x == m_pPosition->x && y < m_pPosition->y)
	{
		m_lastDirection = LAST_DIRECTION::DOWN;
	}
	else
	{
		m_lastDirection = LAST_DIRECTION::RIGHT;
	}

	PlacableActor::SetPosition(x, y);
}

void Player::Draw()
{
	cout << "@";
}
