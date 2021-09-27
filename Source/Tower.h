#pragma once

#include "GameObject.h"
#include "BaseLevelScene.h"
#include "stepTimer.h"

class Tower : public GameObject {
public:
	Tower(Scene* scene, eSpriteType spriteKey, float x, float y, float z = 1.14);
	virtual void update()override;
	GameObject* calcTarget();
	void fire(GameObject* b);
	void setRoll(float angle);
	//void splashDmg();
	float distToTarget();
	// Make this tower blow up
	void boom();
	void onAnimationComplete(AnimationDesc& anim) override;
private:
	GameObject* target = nullptr;
	float m_fGunTimer = 0.0f;
	Scene* objScene;
	float level = 1;
	bool detonated = false;
};