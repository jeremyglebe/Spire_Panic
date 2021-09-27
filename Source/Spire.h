#pragma once
#include "GameObject.h"
#include "HUDObject.h"

class Spire :
	public GameObject
{
public:
	int hp;
	Spire(Scene* scene, float x, float y, float z = 1.15);
	void update() override;
	void onAnimationComplete(AnimationDesc& anim) override;
	// The area in which enemies can attack
	BoundingBox attackZone;
protected:
	int maxHP = 1000;
	AnimationDesc explodeAnimation;
	GameObject* peak;
	HUDObject* healthBar;
	bool detonated = false;
};

