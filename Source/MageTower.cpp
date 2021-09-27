#include "MageTower.h"

MageTower::MageTower(Scene* scene,eSpriteType t, float level, float x, float y, float z) :
	Tower(scene, t, x, y, z) {

	tags.insert("mage");
	if (level == 0)
		tags.insert("lvl0");
	else if (level == 1)
		tags.insert("lvl1");
	else if (level == 2)
		tags.insert("lvl2");
	lvl = level;
}

void MageTower::update() {
	Tower::update();
}

float MageTower::getLevel() {
	return lvl;
}