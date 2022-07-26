#pragma once
#include "PlacableActor.h"

class Shield : public PlacableActor
{
	public:
		Shield(int x, int y);

		virtual ActorType GetType() override { return ActorType::Shield; };
		virtual void Draw() override;
};