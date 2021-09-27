#pragma once
#include <map>
#include <string>
#include "GameObject.h"
#include "AnimationDesc.h"

class Character :
	public GameObject
{
public:
	Character(Scene* scene, eSpriteType spriteKey, float x = 0.0f, float y = 0.0f, float z = 0.0f);
	virtual ~Character() override;
	float getAngle() { return angle; } // Get the current angle in degrees
	void setVelocity(float speed, float angle_in_degrees); // Set character velocity
	void damage(int dmg);
	bool isDying() { return dying; }
protected:
	// Health of character
	int hp = 10;
	bool dying = false;

	// Position & motion
	float angle = 0; // Angle of the character in degrees
	float speed = 0; // Speed of the character regardless of direction

	// Animations
	map<string, AnimationDesc> animations;
	string animationDirection = "SE"; // Characters only have 4 animation directions, NE, NW, SW, SE
	void calculateAnimationDirection();
	virtual void onChangedAnimationDirection();
};

