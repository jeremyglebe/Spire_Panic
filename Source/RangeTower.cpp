#include "RangeTower.h"

RangeTower::RangeTower(Scene* scene, eSpriteType t, float level, float x, float y,float z) :
	Tower(scene, t, x, y, z) {
	
	tags.insert("range");
	if (level == 0)
		tags.insert("lvl0");
	else if (level == 1)
		tags.insert("lvl1");
	else if(level == 2)
		tags.insert("lvl2");
	lvl = level;
}

void RangeTower::update() {
	Tower::update();
}
float RangeTower::getLevel() {
	return lvl;
}