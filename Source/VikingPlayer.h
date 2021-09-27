#pragma once
#include "PlayerCharacter.h"
class VikingPlayer :
    public PlayerCharacter
{
public:
    VikingPlayer(Scene* scene, float x, float y, float z = 1.15);
protected:
    GameObject* _createProjectile() override;
};

