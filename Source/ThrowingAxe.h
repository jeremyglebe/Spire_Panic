#pragma once
#include "Projectile.h"
class ThrowingAxe :
	public Projectile
{
public:
	ThrowingAxe(Scene* scene, float x = 0, float y = 0, float z = 0);
	void update() override;
};

