#include "WolfBaddie.h"
#include "BaseLevelScene.h"

WolfBaddie::WolfBaddie(Scene* scene, float x, float y, float z) :
	BadCharacter(scene, x, y, z)
{
	hp = 6;
	speed = 100.0f;
	// Defining animations for the bad guy
	animations["WALK_NE"] = AnimationDesc(WOLF_WALK, 0, 3, 0.15f);
	animations["WALK_NW"] = AnimationDesc(WOLF_WALK, 4, 7, 0.15f);
	animations["WALK_SW"] = AnimationDesc(WOLF_WALK, 8, 11, 0.15f);
	animations["WALK_SE"] = AnimationDesc(WOLF_WALK, 12, 15, 0.15f);
	animations["ATTACK_NE"] = AnimationDesc(WOLF_ATTACK, 0, 3, 0.15f);
	animations["ATTACK_NW"] = AnimationDesc(WOLF_ATTACK, 4, 7, 0.15f);
	animations["ATTACK_SW"] = AnimationDesc(WOLF_ATTACK, 8, 11, 0.15f);
	animations["ATTACK_SE"] = AnimationDesc(WOLF_ATTACK, 12, 15, 0.15f);
}
WolfBaddie::~WolfBaddie() {}
