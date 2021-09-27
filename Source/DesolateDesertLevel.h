#pragma once
#include "BaseLevelScene.h"
class DesolateDesertLevel :
	public BaseLevelScene
{
public:
	DesolateDesertLevel(CGame* game, eSpriteType ply1Hat = WIZARD_HAT, eSpriteType ply2Hat = WIZARD_HAT, bool coop = false);
	~DesolateDesertLevel();
};

