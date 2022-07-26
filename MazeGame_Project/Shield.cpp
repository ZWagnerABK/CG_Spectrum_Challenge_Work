#include "Shield.h"
#include <iostream>

Shield::Shield(int x, int y) : PlacableActor(x, y)
{
	
}


void Shield::Draw()
{
	std::cout << '#';
}