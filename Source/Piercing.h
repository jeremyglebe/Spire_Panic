#pragma once
#include "GameObject.h"

class Piercing : virtual public GameObject
{
public:
	Piercing() {}
	float lifespan = 5.0f;
	virtual void update() override {
		GameObject::update();
		if (m_pStepTimer->GetTotalSeconds() > creationTime + lifespan) {
			kill();
		}
	}
};

