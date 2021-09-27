#include "WaveSpawner.h"
#include "BaseLevelScene.h"
#include "BadGoblin.h"
#include "BadMinotaur.h"
#include "WolfBaddie.h"

WaveSpawner::WaveSpawner(BaseLevelScene* level) {
	this->level = level;
}

void WaveSpawner::update() {
	// For each of the spawn descriptors, update the spawn state
	for (auto& spawnState : spawns) {
		// Any spawn descriptor for which we have not reached the first wave should be marked not ready
		if (wave < spawnState.startWave) spawnState.ready = false;
		// If these spawns are ready
		if (spawnState.ready) {
			int waveOffset = wave - spawnState.startWave + 1;
			int maxSpawns = (waveOffset * spawnState.addPerWave) + (int)pow(spawnState.multiplyPerWave, waveOffset);

			// If we haven't exceeded this waves max number of spawns and if it's time to spawn
			if (spawnState.numberSpawned < maxSpawns && m_pStepTimer->GetTotalSeconds() > spawnState.nextSpawn) {

				// Get the spawn locations from the level
				vector<PathPoint> spawnOptions = level->getSpawnPoints();
				int numSpawnPoints = (int)spawnOptions.size();
				// Choose a random spawn location
				PathPoint spawnPoint = spawnOptions[m_pRandom->randn(0, numSpawnPoints - 1)];

				// Retrieve paths for starting points from level
				map<string, PathPoint> paths = level->getPaths();

				// Create the baddie object
				BadCharacter* baddie = level->addBaddie(spawnSwitch(spawnState.key));
				// A little bit of random offset on spawn to keep enemies from stacking on each other
				float yoffset = m_pRandom->randf();
				float xoffset = m_pRandom->randf();
				// Move the enemy to a spawn point
				baddie->setPosition({ spawnPoint.x + xoffset, spawnPoint.y + yoffset, 1.15f });
				// Set the enemie's destination to be their starting point initially
				baddie->setDestinationByID(spawnPoint.next);

				// Increase the counter of number of enemies spawned
				spawnState.numberSpawned++;
				if (spawnState.numberSpawned >= maxSpawns) spawnState.ready = false;

				// Calculating time until next spawn
				float untilNextSpawn = spawnState.timeToSpawn
					* powf(spawnState.timeMultiplyPerSpawn, (float)spawnState.numberSpawned)
					+ spawnState.timeAddPerSpawn * spawnState.numberSpawned;
				// Hard lower limit on time to spawn
				if (untilNextSpawn < 1.0f) untilNextSpawn = 1.0f;
				// Setting the time of next spawn
				spawnState.nextSpawn = m_pStepTimer->GetTotalSeconds() + untilNextSpawn;
			}
		}
	}
}

int WaveSpawner::getWaveNumber() {
	return wave;
}

bool WaveSpawner::isWaveComplete() {
	for (const auto& spawnState : spawns) {
		if (spawnState.ready) {
			return false;
		}
	}
	return true;
}

void WaveSpawner::addSpawnDesc(SpawnDesc spawnType) {
	spawns.push_back(spawnType);
}

void WaveSpawner::nextWave() {
	// Update the wave number
	wave++;
	// Reset each of the spawn state's counters and ready them
	for (auto& spawnState : spawns) {
		spawnState.numberSpawned = 0;
		spawnState.ready = true;
	}
}

BadCharacter* WaveSpawner::spawnSwitch(string key) {
	if (key == "goblin") {
		return new BadGoblin(level);
	}
	else if (key == "minotaur") {
		return new BadMinotaur(level);
	}
	else if (key == "wolf") {
		return new WolfBaddie(level);
	}
	else {
		ABORT("Tried to spawn a baddie with an invalid key!");
		// Return won't be used, just want to avoid the stupid compiler warning
		return nullptr;
	}
}