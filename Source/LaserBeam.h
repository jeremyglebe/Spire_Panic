#pragma once
#include "Projectile.h"
#include "Piercing.h"
class LaserBeam :
	public Projectile,
	public Piercing
{
public:
	LaserBeam(Scene* scene, float x = 0, float y = 0, float z = 0);
	void update() override;
};

