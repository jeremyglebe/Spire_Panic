#include "LaserBeam.h"

LaserBeam::LaserBeam(Scene* scene, float x, float y, float z) :
	GameObject(scene, REDLINE, x, y, z)
{
	tags.insert("projectile");
	tags.insert("piercing");
	lifespan = 2.0f;
	damage = 2;
	m_fXScale = 20.0f;
	m_fYScale = 1.3f;
	m_sAabb.Extents = { 5.0f, 5.0f, 5.0f };
	m_pAudio->play(LASER_SOUND);
}

void LaserBeam::update() {
	Piercing::update();
	// Move the bouding box of the laser to the end of the laser
	Vector2 directionVector = getNetForce();
	directionVector.Normalize();
	Vector2 transform = directionVector * 10.0f;
	m_sAabb.Center.x += transform.x;
	m_sAabb.Center.y += transform.y;
}