#include "Bullet.h"

Bullet::Bullet(Scene* scene, float x, float y, float z,float level) :
	GameObject(scene, BULLET1, x, y, z)
{
	tags.insert("projectile");
	m_sAabb = BoundingBox({ x,y,z }, { 5,5,5 });
	damage = 10;
	lvl = level;
	if (lvl == 0) {
		damage = 5;
	}
	if (lvl == 1) {
		damage = 8;
	}
	if (lvl == 2) {
		damage = 12;
	}
}
