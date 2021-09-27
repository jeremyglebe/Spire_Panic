#include "Tower.h"
#include "Bullet.h"
#include "Fireball.h"


Tower::Tower(Scene* scene, eSpriteType spriteKey, float x, float y, float z) :
	GameObject(scene, spriteKey, x, y, z)
{
	objScene = scene;
	setRoll(XM_2PI);
	m_sAabb = BoundingBox({ x,y - 17,z }, { 12,8,4 });

	// Descriptor tags
	tags.insert("tower");
	tags.insert("unpassable");
}
void Tower::update() {
	stepAnimation();
	if (m_pStepTimer->GetTotalSeconds() > m_fGunTimer + 1) {
		target = calcTarget();
		BadCharacter* baddie = dynamic_cast<BadCharacter*>(target);

		m_fGunTimer = m_pStepTimer->GetTotalSeconds();
		if (this->is("tower") && this->is("mage")) {		//add conditions for other towers here, set bullet type
			Vector3 pos = this->getPosition();
			//creating bullet and setting its bounding box
			if (this->is("lvl0") && this->distToTarget() <= 150) {
				GameObject* b = objScene->addObject(new Fireball(objScene, pos.x, pos.y, pos.z, 0));
				fire(b);
			}
			if (this->is("lvl1") && this->distToTarget() <= 250) {
				GameObject* b = objScene->addObject(new Fireball(objScene, pos.x, pos.y, pos.z, 1));
				fire(b);
			}
			else if (this->is("lvl2") && this->distToTarget() <= 350) {
				GameObject* b = objScene->addObject(new Fireball(objScene, pos.x, pos.y, pos.z, 2));
				fire(b);
			}
		}
		else if (this->is("tower") && this->is("range")) {
			Vector3 pos = this->getPosition();
			//creating bullet and setting its bounding box
			if (this->is("lvl0") && this->distToTarget() <= 200) {
				GameObject* b = objScene->addObject(new Bullet(objScene, pos.x, pos.y, pos.z, 0));
				fire(b);
			}
			if (this->is("lvl1") && this->distToTarget() <= 300) {
				GameObject* b = objScene->addObject(new Bullet(objScene, pos.x, pos.y, pos.z, 1));
				fire(b);
			}
			else if (this->is("lvl2") && this->distToTarget() <= 400) {
				GameObject* b = objScene->addObject(new Bullet(objScene, pos.x, pos.y, pos.z, 2));
				fire(b);
			}
		}

	}
}

GameObject* Tower::calcTarget() {
	Vector3 towerPos;
	Vector3 enemyPos;

	GameObject* enemy = nullptr;

	float difX = 1000, difY = 1000, dif = 1000;
	for (GameObject* obj : ((BaseLevelScene*)scene)->getBaddies()) {
		enemyPos = obj->getPosition();		//enemy position
		towerPos = this->getPosition();		//tower position
		BadCharacter* baddie = dynamic_cast<BadCharacter*>(obj);
		difX = abs(enemyPos.x - towerPos.x);		//pythagorean to find nearest enemy object
		difY = abs(enemyPos.y - towerPos.y);
		if (sqrt(pow(difX, 2) + pow(difY, 2)) < dif && (!baddie->isDying())) {
			dif = sqrt(pow(difX, 2) + pow(difY, 2));
			enemy = obj;
		}

	}

	if (((BaseLevelScene*)scene)->getBaddies().size() == 0) {
		return nullptr;
	}
	else
		return enemy;
}

float Tower::distToTarget() {
	Vector3 towerPos;
	Vector3 enemyPos;
	float difX, difY, dist;
	if (target != NULL)
		enemyPos = target->getPosition();
	towerPos = this->getPosition();
	//get tower and target pos
	difX = abs(enemyPos.x - towerPos.x);
	difY = abs(enemyPos.y - towerPos.y);
	dist = sqrt(pow(difX, 2) + pow(difY, 2));
	//pythagorean to find nearest enemy object calculate dist between target and tower

	return dist;

}

void Tower::fire(GameObject* b) {


	//find target for tower
	//target = calcTarget();

	if (target != nullptr) {

		m_pAudio->play(GUN_SOUND);


		//set bullet direction to enemy direction
		const Vector3 dir = b->getPosition() - target->getPosition();
		b->m_fRoll = atan2f(dir.y, dir.x) - XM_PI / 2.0f;
		const Vector2 view = b->getViewVect();			//once the roll angle of bullet is set, we can use the view vector to set
														//the direction of bullet velocity
		//b->setVelocity(500.0f * view);
		b->addForce(500.0f * view, 1000.0f);
		b->setRoll(b->m_fRoll);
	}
	else {
		scene->deleteObject(b);
	}

}
void Tower::setRoll(float angle) {
	GameObject::setRoll(angle);
}

void Tower::boom() {
	if (!detonated) {
		AnimationDesc explodeAnimation = AnimationDesc(EXPLOSION, 0, 4, 0.15f);
		startAnimation(explodeAnimation);
		m_pAudio->play(EXPLOSION_SOUND);
		m_fXScale = 4.0f;
		m_fYScale = 4.0f;
		detonated = true;
	}
}

void Tower::onAnimationComplete(AnimationDesc& anim) {
	AnimationDesc explodeAnimation = AnimationDesc(EXPLOSION, 0, 4, 0.15f);
	if (anim == explodeAnimation) {
		kill();
	}
}