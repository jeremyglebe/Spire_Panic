#pragma once
#include "BadCharacter.h"
class BadGoblin :
	public BadCharacter
{
public:
	BadGoblin(Scene* scene, float x = 0, float y = 0, float z = 1.15);
	~BadGoblin() override;
private:
};

