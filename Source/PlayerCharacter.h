#pragma once

#include <cmath>
#include "Character.h"

class PlayerCharacter :
	public Character
{
public:
	PlayerCharacter(Scene* scene, float x = 0, float y = 0, float z = 1.15, string playerID = "PLAYER1");
	void update() override; // Character's update loop, runs every frame
	void applyForces() override;
	void shoot(float angle_in_degrees);
	float getMaxSpeed() { return maxSpeed; }
	void setPlayerID(string id);
	// Keyboard handler just for the player, for use in the scenes keyboard handler
	virtual void handleKeyboard();
	virtual void handleController();
	// Health and death
	int maxHP = 100;
	int hp = 100;
	bool isDying = false;
	bool isDead = false;
	// The time the player died, stored for respawn purposes
	float timeDied = -1.0f;
protected:
	string playerID;
	float shootCooldown;
	float shootTime;
	float projectileSpeed;
	float maxSpeed;
	
	GameObject* hat = nullptr;
	// A flag used for one small part of the death animation
	bool _hatFalling = false;
	virtual GameObject* _createProjectile();
	void _updateAnimation();
	void _updateHat();
	// Handles the movement keys for the player
	void _handleMovementKeys();
	// Handles the directional (shooting) keys for the player
	void _handleDirectionKeys();
};
