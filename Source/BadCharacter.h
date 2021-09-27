#pragma once
#include <map>
#include "Character.h"
#include "PlayerCharacter.h"

// Forward declarations
class BaseLevelScene;

class BadCharacter :
	public Character
{
public:
	bool isStopped = false;
	bool isAttacking = false;
	BadCharacter(Scene* scene, float x = 0.0f, float y = 0.0f, float z = 1.15);
	virtual ~BadCharacter() override;
	virtual void update() override;
	void onAnimationComplete(AnimationDesc& anim) override;
	bool readyToAttack();
	void setDestinationCoordinates(float x, float y);
	void setDestinationByID(string id);
	// Timers to track when the baddie can be hit, so they aren't spammed
	float timeCanHit = 0.0f;
	float invincibleTime = 0.1f;
protected:
	float speed = 70.0f;
	// Level that the baddie belongs to
	BaseLevelScene* level = nullptr;
	// Time between attacks
	float attackCooldown = 0.6f;
	// Next time that the enemy can attack
	float nextAttackTime = 0.0f;
	// Copy of the level's list of path poitns
	map<string, PathPoint> paths;
	// Coordinates of the destination
	Vector2 destinationCoordinates = { 0,0 };
	// ID of the destination in the level's list of pathpoints
	string destinationID = "";
	// Distance from the destination
	float distanceToDestination = 100000.0f;
	// Starts running the appropriate animation if it's not already
	void update_PlayAnimations();
	// Determines when the baddie reaches destination and what destination to switch to 
	void update_DestinationLogic();
	// Moves the baddie towards a given set of coordinates
	void update_Move(Vector2 towards);
	// Moves the enemy towards their destination
	void update_StateMarch();
	// Handles enemy behavior at game over
	void update_StateDying();
	// Handles the enemy attacking the spire
	void update_StateAttackSpire();
	// Combat behavior for when players are nearby
	void update_StateCombat(PlayerCharacter* player);
};

