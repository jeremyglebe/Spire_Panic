#include "GameDefines.h"

#pragma once
class AnimationDesc
{
public:
	AnimationDesc(){}
	AnimationDesc(eSpriteType sprite, int startFrame, int endFrame, float frameTime) :
		sprite(sprite),
		startFrame(startFrame),
		endFrame(endFrame),
		frameTime(frameTime) {}
	bool operator==(AnimationDesc& other) {
		if (sprite != other.sprite) return false;
		if (startFrame != other.startFrame) return false;
		if (endFrame != other.endFrame) return false;
		if (frameTime != other.frameTime) return false;
		return true;
	}
	eSpriteType sprite = BLACK_SQUARE;
	unsigned int startFrame = 0;
	unsigned int endFrame = 0;
	float frameTime = 0.5;
};

