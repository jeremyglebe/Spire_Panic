#include "PlayerCharacter.h"
#include "StepTimer.h"
#include "Bullet.h"
#include "Scene.h"
#include "Controller.h"
#include "BaseLevelScene.h"

PlayerCharacter::PlayerCharacter(Scene* scene, float x, float y, float z, string playerID) :
	Character(scene, HUMAN_IDLE, x, y, z)
{
	// Set the player's ID (in case we play co op)
	this->playerID = playerID;
	// Create collision box for the player
	m_sAabb = BoundingBox({ x,y + 4,z }, { 16,16,16 });
	// Descriptor tags
	tags.insert("player");
	// Defining animations for the player
	animations["IDLE_NE"] = AnimationDesc(HUMAN_IDLE, 0, 3, 0.15f);
	animations["IDLE_NW"] = AnimationDesc(HUMAN_IDLE, 4, 7, 0.15f);
	animations["IDLE_SW"] = AnimationDesc(HUMAN_IDLE, 8, 11, 0.15f);
	animations["IDLE_SE"] = AnimationDesc(HUMAN_IDLE, 12, 15, 0.15f);
	animations["WALK_NE"] = AnimationDesc(HUMAN_WALK, 0, 3, 0.15f);
	animations["WALK_NW"] = AnimationDesc(HUMAN_WALK, 4, 7, 0.15f);
	animations["WALK_SW"] = AnimationDesc(HUMAN_WALK, 8, 11, 0.15f);
	animations["WALK_SE"] = AnimationDesc(HUMAN_WALK, 12, 15, 0.15f);
	// Start default animation
	startAnimation(animations["IDLE_SE"], true);
}

void PlayerCharacter::update() {
	// Normal behavior for the player character
	if (!isDying && !isDead) {
		// Animate the character
		stepAnimation();
		// Move the character
		applyForces();
		// Slightly adjust our bounding box for the player from just the player position
		m_sAabb.Center.y += 4;
		// Update which animation is currently playing
		_updateAnimation();
		// Clear away expired forces
		updateForces();
	}
	// If the player is dying
	else if (isDying) {
		// Fade the character
		if (m_fAlpha > 0.05f) {
			m_fAlpha -= 0.05f;
		}
		// Once the character is done fading, drop the hat (and fade)
		else {
			m_fAlpha = 0.0f;
			if (!_hatFalling) {
				addForce({ 0.0f, -16.0f }, 2.0f);
				_hatFalling = true;
			}
			if (hat->m_fAlpha > 0.05f) {
				hat->m_fAlpha -= 0.01f;
			}
		}
		// Move the character
		applyForces();
		// Clear away expired forces
		updateForces();
	}
	else if (isDead) {
		// Set the time died (negative means it has not been set yet)
		if (timeDied < 0.0f) {
			timeDied = m_pStepTimer->GetTotalSeconds();
		}
		// The hat is no longer falling
		_hatFalling = false;
		// Make sure player and hat are invisible
		m_fAlpha = 0.05f;
		hat->m_fAlpha = 0.05f;
		// Player should move to spawn point
		m_vPos.x = ((BaseLevelScene*)scene)->getPaths()["plySpawn"].x;
		m_vPos.y = ((BaseLevelScene*)scene)->getPaths()["plySpawn"].y;
		m_sAabb.Center.x = m_vPos.x;
		m_sAabb.Center.y = m_vPos.y + 4;
		// Move the hat to the player
		_updateHat();
		// Respawn after 20 seconds
		if (m_pStepTimer->GetTotalSeconds() > timeDied + 20.0f) {
			hp = maxHP;
			isDead = false;
			m_fAlpha = 1.0f;
			hat->m_fAlpha = 1.0f;
			timeDied = -1.0f;
		}
	}
	// Update whether we are dying
	if (hp <= 0) {
		if (hat->m_fAlpha > 0.1f) {
			isDying = true;
			isDead = false;
		}
		else {
			isDying = false;
			isDead = true;
		}
	}
}

void PlayerCharacter::applyForces() {
	GameObject::applyForces();
	// Update the player's hat
	_updateHat();
}

void PlayerCharacter::shoot(float angle_in_degrees) {
	angle = angle_in_degrees;
	if (m_pStepTimer->GetTotalSeconds() > shootTime) {
		// Convert the angle to radians for mathematics
		float angle_in_radians = angle_in_degrees * (MY_PI / 180);
		// Calculate the x component of the velocity
		float xSpeed = projectileSpeed * cos(angle_in_radians);
		// Calculate the y component of the velocity
		float ySpeed = projectileSpeed * sin(angle_in_radians);
		// Set the player's velocity
		Vector2 projectileVelocity = { xSpeed, ySpeed };
		// Create the bullet object
		GameObject* b = _createProjectile();
		//b->setVelocity(projectileVelocity);
		b->addForce(projectileVelocity, 1000.0f);
		// Rotate the projectile. Some projectiles this doesn't really matter, but
		// assume that 0rads is to the right and adjust just in case
		b->m_fRoll = angle_in_radians;
		// Update the next time I can shoot
		shootTime = m_pStepTimer->GetTotalSeconds() + shootCooldown;
	}
}

void PlayerCharacter::setPlayerID(string id) { playerID = id; }

void PlayerCharacter::handleKeyboard() {
	if (!isDying && !isDead) {
		_handleMovementKeys();
		_handleDirectionKeys();
		if (m_pKeyboard->TriggerDown(controlMap[playerID + "_UPGRADE"]))
			((BaseLevelScene*)scene)->upgradeTower(this);
		if (m_pKeyboard->TriggerDown(controlMap[playerID + "_DESTROY"]))
			((BaseLevelScene*)scene)->destroyTower(this);
	}
}

void PlayerCharacter::handleController() {
	if (!isDying && !isDead) {
		// Get thumb vectors
		Vector2 leftThumb = m_pController->GetLThumb();
		Vector2 rightThumb = m_pController->GetRThumb();
		// Check if sticks are significantly pressed
		bool leftThumbUsed = abs(leftThumb.x) + abs(leftThumb.y) > 0.05f;
		bool rightThumbUsed = abs(rightThumb.x) + abs(rightThumb.y) > 0.05f;
		// Left stick handler
		if (leftThumbUsed) {
			// Calculate angles
			float leftThumbAngle = atan2f(leftThumb.y, leftThumb.x);
			float leftThumbAngleDegrees = leftThumbAngle * (180.0f / MY_PI);
			if (leftThumbAngleDegrees < 0) leftThumbAngleDegrees += 360.0f;
			// Set the player's velocity
			if (leftThumbUsed) setVelocity(maxSpeed, leftThumbAngleDegrees);
		}
		else setVelocity(0, angle);
		// Right stick handler
		if (rightThumbUsed) {
			// Calculate angles
			float rightThumbAngle = atan2f(rightThumb.y, rightThumb.x);
			float rightThumbAngleDegrees = rightThumbAngle * (180.0f / MY_PI);
			if (rightThumbAngleDegrees < 0) rightThumbAngleDegrees += 360.0f;
			// Control the player's shooting
			if (rightThumbUsed) shoot(rightThumbAngleDegrees);
		}

		// Check if the tower upgrade button is pressed
		if (gamepad.isPressed("BUTTON_X")) {
			((BaseLevelScene*)scene)->upgradeTower(this);
		}
		if (gamepad.isPressed("BUTTON_B")) {
			((BaseLevelScene*)scene)->destroyTower(this);
		}
	}
}

GameObject* PlayerCharacter::_createProjectile() {
	return scene->addObject(new Bullet(scene, m_vPos.x, m_vPos.y, m_vPos.z));
}

void PlayerCharacter::_updateAnimation() {
	string key = "";

	if (speed > 0) key += "WALK_";
	else key += "IDLE_";

	calculateAnimationDirection();
	key += animationDirection;

	if (!isPlayingAnimation(animations[key]))
		startAnimation(animations[key], true);
}

void PlayerCharacter::_handleMovementKeys() {
	// Handles movement keys for the character
	if (m_pKeyboard->Down(controlMap[playerID + "_LEFT"]))
	{
		// Left-Up
		if (m_pKeyboard->Down(controlMap[playerID + "_UP"]))
			setVelocity(maxSpeed, 135);
		// Left-Down
		else if (m_pKeyboard->Down(controlMap[playerID + "_DOWN"]))
			setVelocity(maxSpeed, 225);
		// Left
		else
			setVelocity(maxSpeed, 180);
	}
	else if (m_pKeyboard->Down(controlMap[playerID + "_RIGHT"]))
	{
		// Right-Up
		if (m_pKeyboard->Down(controlMap[playerID + "_UP"]))
			setVelocity(maxSpeed, 45);
		// Right-Down
		else if (m_pKeyboard->Down(controlMap[playerID + "_DOWN"]))
			setVelocity(maxSpeed, 315);
		// Right
		else
			setVelocity(maxSpeed, 0);
	}
	// Down
	else if (m_pKeyboard->Down(controlMap[playerID + "_DOWN"]))
		setVelocity(maxSpeed, 270);
	// Up
	else if (m_pKeyboard->Down(controlMap[playerID + "_UP"]))
		setVelocity(maxSpeed, 90);
	else
		speed = 0;
}

void PlayerCharacter::_handleDirectionKeys() {
	// Moving of the character
	if (m_pKeyboard->Down(controlMap[playerID + "_AIM_WEST"]))
	{
		// Left-Up
		if (m_pKeyboard->Down(controlMap[playerID + "_AIM_NORTH"]))
			shoot(135);
		// Left-Down
		else if (m_pKeyboard->Down(controlMap[playerID + "_AIM_SOUTH"]))
			shoot(225);
		// Left
		else
			shoot(180);
	}
	else if (m_pKeyboard->Down(controlMap[playerID + "_AIM_EAST"]))
	{
		// Right-Up
		if (m_pKeyboard->Down(controlMap[playerID + "_AIM_NORTH"]))
			shoot(45);
		// Right-Down
		else if (m_pKeyboard->Down(controlMap[playerID + "_AIM_SOUTH"]))
			shoot(315);
		// Right
		else
			shoot(0);
	}
	// Down
	else if (m_pKeyboard->Down(controlMap[playerID + "_AIM_SOUTH"]))
		shoot(270);
	// Up
	else if (m_pKeyboard->Down(controlMap[playerID + "_AIM_NORTH"]))
		shoot(90);
}


void PlayerCharacter::_updateHat() {
	// Update the hat's position
	hat->m_vPos = { m_vPos.x, m_vPos.y + 15, m_vPos.z - 0.01f };
	hat->m_fRoll = 0;
	// Check what direction the player is facing
	if (animationDirection[0] == 'S') hat->m_nCurrentFrame = 0;
	else if (animationDirection[0] == 'N') hat->m_nCurrentFrame = 1;
	// And left/right direction
	if (animationDirection[1] == 'E') {
		hat->m_fYaw = 0;
	}
	else if (animationDirection[1] == 'W') {
		hat->m_fYaw = MY_PI;
	}
	// Bounce the hat if we're moving
	if (speed > 0) {
		// Hat bounces a little with player movement
		hat->m_vPos.y += 9 * (m_nCurrentFrame % 2);
	}
	if (abs(getNetForce().x) > 0.01f) {
		// Hat tilts back a bit against the "wind" lol
		hat->m_fRoll = .15f;
	}
}
