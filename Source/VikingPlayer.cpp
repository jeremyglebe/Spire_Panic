#include "VikingPlayer.h"
#include "Scene.h"
#include "ThrowingAxe.h"

VikingPlayer::VikingPlayer(Scene* scene, float x, float y, float z) :
	PlayerCharacter(scene, x, y, z)
{
	hat = scene->addObject(new GameObject(scene, VIKING_HAT, m_vPos.x, m_vPos.y, m_vPos.z));
	maxSpeed = 200.0f;
	// Projectile usage qualities
	shootCooldown = 0.6f;
	shootTime = 0;
	projectileSpeed = 300.0f;
}

// Creates the fireball as the projectile
GameObject* VikingPlayer::_createProjectile() {
	GameObject* obj = scene->addObject(new ThrowingAxe(scene, m_vPos.x, m_vPos.y, m_vPos.z));
	ThrowingAxe* axe = dynamic_cast<ThrowingAxe*>(obj);
	return obj;
}
