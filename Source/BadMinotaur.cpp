#include "BadMinotaur.h"
#include "BaseLevelScene.h"

BadMinotaur::BadMinotaur(Scene* scene, float x, float y, float z) :
	BadCharacter(scene, x, y, z)
{
	// Minotaur has big HP
	hp = 35;
	// Minotaur is slow
	speed = 40.0f;
	// Defining animations for the bad guy
	animations["WALK_NE"] = AnimationDesc(MINOTAUR_WALK, 0, 5, 0.15f);
	animations["WALK_NW"] = AnimationDesc(MINOTAUR_WALK, 6, 11, 0.15f);
	animations["WALK_SW"] = AnimationDesc(MINOTAUR_WALK, 12, 17, 0.15f);
	animations["WALK_SE"] = AnimationDesc(MINOTAUR_WALK, 18, 23, 0.15f);
	animations["ATTACK_NE"] = AnimationDesc(MINOTAUR_ATTACK, 0, 7, 0.15f);
	animations["ATTACK_NW"] = AnimationDesc(MINOTAUR_ATTACK, 8, 15, 0.15f);
	animations["ATTACK_SW"] = AnimationDesc(MINOTAUR_ATTACK, 16, 23, 0.15f);
	animations["ATTACK_SE"] = AnimationDesc(MINOTAUR_ATTACK, 24, 31, 0.15f);
}
BadMinotaur::~BadMinotaur() {}
