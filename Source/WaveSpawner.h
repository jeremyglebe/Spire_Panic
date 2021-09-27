#pragma once
#include <string>
#include <vector>
#include "Component.h"
#include "BadCharacter.h"
#include "SpawnDesc.h"

class BaseLevelScene;

class WaveSpawner : CComponent
{
public:
	WaveSpawner(BaseLevelScene* level);
	void update();
	int getWaveNumber();
	bool isWaveComplete();
	void addSpawnDesc(SpawnDesc spawnType);
	void nextWave();
	BadCharacter* spawnSwitch(string key);
private:
	BaseLevelScene* level;
	vector<SpawnDesc> spawns;
	int wave = 1;
};

