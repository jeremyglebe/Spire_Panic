#pragma once
#include <string>

class SpawnDesc
{
public:
	string key = "goblin";
	int startWave = 1;
	int addPerWave = 1;
	float multiplyPerWave = 1;
	int numberSpawned = 0;
	float timeToSpawn = 5.0f;
	float timeAddPerSpawn = -0.01f;
	float timeMultiplyPerSpawn = 1.0f;
	float nextSpawn = 0.0f;
	bool ready = true;
};

