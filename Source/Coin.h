#pragma once
#include "GameObject.h"

class Coin :
	public GameObject
{
public:
	Coin(Scene* scene, int value, float x, float y, float z = 1.15f);
	void update() override;
	int getValue();
private:
	int value = 1;
	float scale = 1.0f;
	float timeCreated;
};

