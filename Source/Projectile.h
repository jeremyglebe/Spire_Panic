#pragma once
#include "GameObject.h"

class Projectile : virtual public GameObject
{
public:
	Projectile() {}
	int damage = 1;
	float range = 100.0f;
	float traveled = 0.0f;
};

