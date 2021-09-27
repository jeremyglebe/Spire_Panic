#pragma once
#include "BadCharacter.h"
class WolfBaddie :
    public BadCharacter
{
public:
    WolfBaddie(Scene* scene, float x = 0, float y = 0, float z = 1.15);
    ~WolfBaddie() override;
};

