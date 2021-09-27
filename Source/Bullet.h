#pragma once
#include "GameObject.h"
#include "Projectile.h"

class Bullet :
    public Projectile
{
public:
    Bullet(Scene* scene, float x = 0, float y = 0, float z = 0,float level = 0);
    float lvl;
};

