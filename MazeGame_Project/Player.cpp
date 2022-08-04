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
		m_pCurrentKey->Place(m_pPosition->x, m_pPosition->y);
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

void Player::Draw()
{
	cout << "@";
}
