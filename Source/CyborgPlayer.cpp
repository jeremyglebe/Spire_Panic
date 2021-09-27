#include "CyborgPlayer.h"
#include "Scene.h"
#include "LaserBeam.h"

CyborgPlayer::CyborgPlayer(Scene* scene, float x, float y, float z) :
	PlayerCharacter(scene, x, y, z)
{
	hat = scene->addObject(new GameObject(scene, CYBORG_HAT, m_vPos.x, m_vPos.y, m_vPos.z));
	maxSpeed = 350.0f;
	// Projectile usage qualities
	shootCooldown = 0.25f;
	shootTime = 0.0f;
	projectileSpeed = 800.0f;
}

// Creates the fireball as the projectile
GameObject* CyborgPlayer::_createProjectile() {
	return scene->addObject(new LaserBeam(scene, m_vPos.x, m_vPos.y, m_vPos.z));
}
