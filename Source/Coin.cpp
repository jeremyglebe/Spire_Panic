#include "Coin.h"

Coin::Coin(Scene* scene, int value, float x, float y, float z)
	: GameObject(scene, COIN, x, y, z)
{
	this->value = value;
	timeCreated = m_pStepTimer->GetTotalSeconds();
	tags.insert("coins");
}

void Coin::update() {
	GameObject::update();
	if (value < 4) m_nSpriteIndex = COIN;
	else if (value < 8) m_nSpriteIndex = COIN_STACK;
	else m_nSpriteIndex = COIN_BAG;
	// Update the scale
	float timePassed = m_pStepTimer->GetTotalSeconds() - timeCreated;
	float maxTimeElapse = 2 * (float)value;
	float percentPassed = timePassed / maxTimeElapse;
	scale = 1 - percentPassed;
	m_fXScale = scale;
	m_fYScale = scale;
	// Delete the coins after some time
	if (scale < 0.1) kill();
}

int Coin::getValue() { return value; }