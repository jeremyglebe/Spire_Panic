#include "Character.h"

Character::Character(Scene* scene, eSpriteType spriteKey, float x, float y, float z) :
	GameObject(scene, spriteKey, x, y, z)
{}
Character::~Character() {}

void Character::setVelocity(float speed, float angleInDegrees) {
	// The desired speed
	this->speed = speed;
	// Set the player's angle (we use this elsewhere as well)
	angle = angleInDegrees;
	// Add the force to the character
	addForce(speed, angleInDegrees, PseudoForce::Instant);
}

void Character::calculateAnimationDirection() {
	string oldDirection = animationDirection;
	int n = (int)angle;
	// If not one of the four cardinal directions
	if (n % 90 != 0) {
		if (n < 90) animationDirection = "NE";
		else if (n < 180) animationDirection = "NW";
		else if (n < 270) animationDirection = "SW";
		else animationDirection = "SE";
	}
	// Handle each of the cardinal directions
	else if (n == 90) {
		// If going up and not facing up, face up
		if (animationDirection == "SW") animationDirection = "NW";
		else if (animationDirection == "SE") animationDirection = "NE";
	}
	else if (n == 270) {
		// If going down and not facing down, face down
		if (animationDirection == "NE") animationDirection = "SE";
		else if (animationDirection == "NW") animationDirection = "SW";
	}
	else if (n == 0) {
		// If going right and not facing right, face right
		if (animationDirection == "NW") animationDirection = "NE";
		else if (animationDirection == "SW") animationDirection = "SE";
	}
	else if (n == 180) {
		// If going left and not facing left, face left
		if (animationDirection == "NE") animationDirection = "NW";
		else if (animationDirection == "SE") animationDirection = "SW";
	}

	// Maybe call a callback function
	if (animationDirection != oldDirection) onChangedAnimationDirection();
}

void Character::onChangedAnimationDirection() {}

void Character::damage(int dmg) {
	hp -= dmg;
}