#pragma once
#include "PlacableActor.h"

class Key;
class Shield;

class Player : public PlacableActor
{
public:

	enum LAST_DIRECTION
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};


	Player();

	bool HasKey();
	bool HasKey(ActorColor color);
	void PickupKey(Key* key);
	void UseKey();
	void DropKey();
	Key* GetKey() { return m_pCurrentKey; }

	bool HasShield();
	void PickupShield(Shield* bomb);
	void UseShield();
	Shield* GetShield() { return m_pShield; }

	void AddMoney(int money) { m_money += money; }
	int GetMoney() { return m_money; }

	int GetLives() { return m_lives; }
	void DecrementLives() { m_lives--; }

	void ResetItems();

	void SetPosition(int x, int y) override;

	virtual ActorType GetType() override { return ActorType::Player; }
	virtual void Draw() override;

private:
	Key* m_pCurrentKey;
	Shield* m_pShield;
	int m_money;
	int m_lives;

	int m_deltaX = 1;
	int m_deltaY = 1;

	LAST_DIRECTION m_lastDirection = LAST_DIRECTION::NONE;
};
