#pragma once
#include "PlacableActor.h"

class Key;
class Bomb;

class Player : public PlacableActor
{
public:
	Player();

	bool HasKey();
	bool HasKey(ActorColor color);
	void PickupKey(Key* key);
	void UseKey();
	void DropKey();
	Key* GetKey() { return m_pCurrentKey; }

	bool HasBomb();
	void PickupBomb(Bomb* bomb);
	void UseBomb();
	Bomb* GetBomb() { return m_pBomb; }

	void AddMoney(int money) { m_money += money; }
	int GetMoney() { return m_money; }

	int GetLives() { return m_lives; }
	void DecrementLives() { m_lives--; }

	void ResetItems();

	virtual ActorType GetType() override { return ActorType::Player; }
	virtual void Draw() override;

private:
	Key* m_pCurrentKey;
	Bomb* m_pBomb;
	int m_money;
	int m_lives;
};
