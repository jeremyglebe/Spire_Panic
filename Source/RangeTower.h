#pragma once
#include "Tower.h"


class RangeTower : 
	public Tower {

public:
	RangeTower(Scene* scene,eSpriteType t, float level, float x, float y,float z =1.14);
	virtual void update();
	float getLevel();
private:
	float lvl;
};
