#pragma once
#include "PlayerCharacter.h"
class WizardPlayer :
    public PlayerCharacter
{
public:
    WizardPlayer(Scene* scene, float x, float y, float z = 1.15);
protected:
    GameObject* _createProjectile() override;
};

