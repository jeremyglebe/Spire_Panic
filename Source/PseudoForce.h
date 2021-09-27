#pragma once
#include "Component.h"
#include "Common.h"

class PseudoForce :
	public CComponent,
	public CCommon
{
public:
	PseudoForce(Vector2 initial, float decayTime);
	Vector2 get();
	bool isDead();
	Vector2 initial;
	float startTime;
	float decayTime;
	static const float Instant;
};

