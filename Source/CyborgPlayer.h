#pragma once
#include "PlayerCharacter.h"
class CyborgPlayer :
    public PlayerCharacter
{
public:
    CyborgPlayer(Scene* scene, float x, float y, float z = 1.15);
protected:
    GameObject* _createProjectile() override;
};

