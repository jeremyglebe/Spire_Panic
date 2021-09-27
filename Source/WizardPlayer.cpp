#include "WizardPlayer.h"
#include "Scene.h"
#include "Fireball.h"

WizardPlayer::WizardPlayer(Scene* scene, float x, float y, float z) :
	PlayerCharacter(scene, x, y, z)
{
	hat = scene->addObject(new GameObject(scene, WIZARD_HAT, m_vPos.x, m_vPos.y, m_vPos.z));
	maxSpeed = 250;
	// Projectile usage qualities
	shootCooldown = .25;
	shootTime = 0;
	projectileSpeed = 400;
}

// Creates the fireball as the projectile
GameObject* WizardPlayer::_createProjectile() {
	GameObject* obj = scene->addObject(new Fireball(scene, m_vPos.x, m_vPos.y, m_vPos.z));
	Fireball* fireball = dynamic_cast<Fireball*>(obj);
	fireball->radius = 48.0f;
	return obj;
}
