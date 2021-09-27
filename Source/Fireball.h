#pragma once
#include "AnimationDesc.h"
#include "GameObject.h"
#include "Projectile.h"
#include "Explosive.h"
#include "Tower.h"

class Fireball :
	public Projectile,
	public Explosive
{
public:
	Fireball(Scene* scene, float x = 0, float y = 0, float z = 0, float level =0);
	void update() override;
	void kill() override;
	void onAnimationComplete(AnimationDesc& anim) override;
	void setScale(float x,float y);
protected:
	AnimationDesc explodeAnimation;
	float xScale;
	float yScale;
	float lvl;
};