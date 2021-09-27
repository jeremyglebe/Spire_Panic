#pragma once
#include "SpriteDesc.h"
#include "GameObject.h"

class HUDObject :
    public CSpriteDesc3D, public CCommon
{
public:
    HUDObject(Scene* scene, eSpriteType spriteKey, float xPercentage, float yPercentage, float relativeZ = 0.0f);
    // Correct the position of HUD object based on camera
    // May be called separately from update() in case camera adjusts after update()
    void adjust(); 
    // X coordinate as a percentage of the HUD width, relative to the center of camera
    float xPercentage;
    // Y coordinate as a percentage of the HUD height, relative to the center of camera
    float yPercentage;
    // Relative Z coordinate from the base HUD Z value
    float relativeZ;
    // Scale relative to the position of the camera
    float relativeXScale;
    float relativeYScale;
protected:
    Scene* scene = nullptr;
    const float xMaxScale = .39f;
    const float yMaxScale = .22f;
    const float zBaseScale = .00095f;
};

