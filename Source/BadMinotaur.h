#pragma once
#include "BadCharacter.h"
class BadMinotaur :
	public BadCharacter
{
public:
	BadMinotaur(Scene* scene, float x = 0, float y = 0, float z = 1.15);
	~BadMinotaur() override;
private:
};

