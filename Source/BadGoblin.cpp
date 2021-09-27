#include "BadGoblin.h"
#include "BaseLevelScene.h"

BadGoblin::BadGoblin(Scene* scene, float x, float y, float z) :
	BadCharacter(scene, x, y, z)
{
	hp = 14;
	// Defining animations for the bad guy
	animations["WALK_NE"] = AnimationDesc(GOBLIN_WALK, 0, 3, 0.15f);
	animations["WALK_NW"] = AnimationDesc(GOBLIN_WALK, 4, 7, 0.15f);
	animations["WALK_SW"] = AnimationDesc(GOBLIN_WALK, 8, 11, 0.15f);
	animations["WALK_SE"] = AnimationDesc(GOBLIN_WALK, 12, 15, 0.15f);
	animations["ATTACK_NE"] = AnimationDesc(GOBLIN_ATTACK, 0, 3, 0.15f);
	animations["ATTACK_NW"] = AnimationDesc(GOBLIN_ATTACK, 4, 7, 0.15f);
	animations["ATTACK_SW"] = AnimationDesc(GOBLIN_ATTACK, 8, 11, 0.15f);
	animations["ATTACK_SE"] = AnimationDesc(GOBLIN_ATTACK, 12, 15, 0.15f);
}
BadGoblin::~BadGoblin() {}
