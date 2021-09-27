#pragma once
#include "GameObject.h"

class Explosive : virtual public GameObject
{
public:
	Explosive() {}
	float radius = 20.0f;
	int splash = 1;
	bool detonated = false;
};

