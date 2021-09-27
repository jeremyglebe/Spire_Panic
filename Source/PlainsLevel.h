#pragma once
#include "BaseLevelScene.h"
class PlainsLevel :
    public BaseLevelScene
{
public:
    PlainsLevel(CGame* game, eSpriteType ply1Hat = WIZARD_HAT, eSpriteType ply2Hat = WIZARD_HAT, bool coop = false);
    ~PlainsLevel();
};

