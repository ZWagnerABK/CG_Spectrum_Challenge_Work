#include "Bomb.h"
#include <iostream>

Bomb::Bomb(int x, int y) : PlacableActor(x, y)
{
	
}


void Bomb::Draw()
{
	std::cout << '#';
}