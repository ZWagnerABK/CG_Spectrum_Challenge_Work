#pragma once
#include "PlacableActor.h"

class Bomb : public PlacableActor
{
	public:
		Bomb(int x, int y);

		virtual ActorType GetType() override { return ActorType::Bomb; };
		virtual void Draw() override;

	protected:

	private:
};