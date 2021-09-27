#pragma once
#include "Tower.h"


class MageTower :
	public Tower {

public:
	MageTower(Scene* scene,eSpriteType t, float level, float x, float y, float z = 1.14);
	virtual void update();
	float getLevel();
private:
	float lvl;

};
