#include "BaseLevelScene.h"
#include "Game.h"
#include "Random.h"
#include "HUDObject.h"
#include "VikingPlayer.h"
#include "WizardPlayer.h"
#include "CyborgPlayer.h"
#include "MageTower.h"
#include "RangeTower.h"
#include "BadGoblin.h"
#include "BadMinotaur.h"
#include "Coin.h"

BaseLevelScene::BaseLevelScene(CGame* game, eSpriteType ply1Hat, eSpriteType ply2Hat, bool coOp) :
	TiledScene(game)
{
	this->hats.first = ply1Hat;
	this->hats.second = ply2Hat;
	this->coOp = coOp;
	waves = new WaveSpawner(this);

	// Create the icon to indicate gold amounts
	HUDObject* coins = addObject(new HUDObject(this, COIN_BAG, -0.92f, 0.9f));
	coins->relativeXScale = 1.5f;
	coins->relativeYScale = 1.5f;
	coins->m_fAlpha = 0.7f;

	// Create the tower shop menu
	towerMenus.first = (ShopMenu*)addObject(new ShopMenu(this, -0.5f, -0.3f));
	towerMenus.second = (ShopMenu*)addObject(new ShopMenu(this, 0.5f, -0.3f, "PLAYER2"));

	// Spawn descriptors that can be changed in inherited classes
	createSpawnDescriptors();
}

BaseLevelScene::~BaseLevelScene() {
	delete waves;
}

void BaseLevelScene::create() {
	// Play some music
	m_pAudio->loop(MAP_SONG);
	// Call parent create() method to create background
	Scene::create();
	// Track the original camera position for easy scaling
	defPos = m_pRenderer->GetCameraPos();
	// Create the simple tilemap
	loadTileMap(tileMapFileName, tileSetSprite);
	backgroundSprite->m_vPos = { (tilemapWidth / 2.0f) * tileSize, (tilemapHeight / 2.0f) * tileSize, 10.0f };
	backgroundSprite->m_fYScale = (float)(tilemapHeight * tileSize);
	backgroundSprite->m_fXScale = (float)(tilemapWidth * tileSize);
	// Place the player on the map
	_createPlayer(players.first, hats.first);
	if (coOp) _createPlayer(players.second, hats.second, "PLAYER2");
	// Set the camera to follow the player/s
	if (coOp)
		camFollowVision(players.first, players.second);
	else
		camFollowVision(players.first);
	// Create the spire object
	spire = (Spire*)addObject(new Spire(this, paths["end"].x, paths["end"].y));
}

void BaseLevelScene::update() {
	// Handling controls specific to the scene
	handleControls();
	// Core scene update, including updating objects
	Scene::update();
	// Runs checks for object and tilemap collisions
	BroadPhaseClsn();
	// Update the camera position if we're following the player
	if (shouldFollow) {
		if (followTargets.second) {
			Vector3 p1 = followTargets.first->getPosition();
			Vector3 p2 = followTargets.second->getPosition();
			Vector2 between = (p1 + p2) / 2.0f;
			float dx = abs(p1.x - p2.x) * .0025f;
			float dy = abs(p1.y - p2.y) * .005f;
			float zMod = dx > dy ? dx : dy;
			if (zMod > 1.0f)
				m_pRenderer->SetCameraPos({ between.x, between.y, (.66f * defPos.z) * zMod });
			else
				m_pRenderer->SetCameraPos({ between.x, between.y, (.66f * defPos.z) });
		}
		else {
			m_pRenderer->SetCameraPos(Vector3(followTargets.first->m_vPos.x, followTargets.first->m_vPos.y, defPos.z / 1.5f));
		}
	}
	// After changing camera position, make sure to adjust any HUD elements
	adjustHUD();
	// Delete objects that need to be culled
	cullObjects();
	// Check end of game conditions
	if (gameOver && m_pStepTimer->GetTotalSeconds() > endTime) {
		m_pAudio->stop();
		m_pRenderer->SetCameraPos(defPos);
		game->startScene(new TitleScreenScene(game));
	}

	// Handle spawning waves
	if (!gameOver) {
		waves->update();
		// If the wave is done spawning and all the baddies are killed, progress to the next wave
		if (waves->isWaveComplete() && !baddies.size()) {
			waves->nextWave();
		}
	}
}

void BaseLevelScene::render() {
	TiledScene::render();
	// If we are set to draw collision boxes, do that now
	if (drawMapClsn) {
		m_pRenderer->DrawBoundingBox(spire->m_sAabb);
		m_pRenderer->DrawBoundingBox(players.first->m_sAabb);
		for (auto obj : objectList) {
			if (obj->is("tower")) {
				m_pRenderer->DrawBoundingBox(obj->m_sAabb);
			}
			if (obj->is("projectile")) {
				m_pRenderer->DrawBoundingBox(obj->m_sAabb);
			}
			if (obj->is("baddie")) {
				m_pRenderer->DrawBoundingBox(obj->m_sAabb);
			}
		}
	}
	if (!gameOver) {
		string hpString = "Spire";
		float offset = hpString.size() * 7.5f;
		m_pRenderer->DrawScreenText(hpString.c_str(), { 640.0f - offset, 26.0f }, Colors::White);

		string waveString = "Wave " + to_string(waves->getWaveNumber());
		offset = waveString.size() * 7.5f;
		m_pRenderer->DrawScreenText(waveString.c_str(), { 640.0f - offset, 66.0f }, Colors::White);

		string credString = to_string(credits);
		m_pRenderer->DrawScreenText(credString.c_str(), { 44.0f,30.0f }, Colors::Gold);

		if (getPlayers().first->isDead) {
			string respawnTimer = to_string(int((getPlayers().first->timeDied + 20) - m_pStepTimer->GetTotalSeconds()));
			m_pRenderer->DrawScreenText("Oh Dear, You Have Died... Respawn in:", Vector2(430, 95), Colors::IndianRed);
			m_pRenderer->DrawScreenText(respawnTimer.c_str(), Vector2(630, 120), Colors::IndianRed);
		}
		if (coOp && getPlayers().second->isDead) {
			string respawnTimer = to_string(int((getPlayers().second->timeDied + 20) - m_pStepTimer->GetTotalSeconds()));
			m_pRenderer->DrawScreenText("Oh Dear, You Have Died... Respawn in:", Vector2(430, 160), Colors::DarkCyan);
			m_pRenderer->DrawScreenText(respawnTimer.c_str(), Vector2(630, 185), Colors::IndianRed);
		}

	}
	else {
		m_pRenderer->setFont("title_font");
		m_pRenderer->DrawCenteredText("Game Over", Colors::IndianRed);
		m_pRenderer->setFont("font");
		string timeLeft = to_string(int(endTime - m_pStepTimer->GetTotalSeconds()));
		float x = 640.0f - timeLeft.size() * 8.0f;
		m_pRenderer->DrawScreenText(timeLeft.c_str(), { x, 420.0f }, Colors::IndianRed);
	}
	// Render the text from the shop menus
	towerMenus.first->renderText();
	towerMenus.second->renderText();

	// Create prompts for both players
	string p1Prompt = "";
	string p2Prompt = "";

	if (controllerOwner == 1) {
		p1Prompt += "y";
	}
	else {
		p1Prompt += keyboardKeys[controlMap["PLAYER1_MENU"]];
	}
	if (controllerOwner == 2) {
		p2Prompt = "y";
	}
	else {
		p2Prompt += keyboardKeys[controlMap["PLAYER2_MENU"]];
	}
	p1Prompt += " TOWER MENU\n";
	p2Prompt += " TOWER MENU\n";

	if (controllerOwner == 1) {
		p1Prompt += "x";
	}
	else {
		p1Prompt += keyboardKeys[controlMap["PLAYER1_UPGRADE"]];
	}
	if (controllerOwner == 2) {
		p2Prompt += "x";
	}
	else {
		p2Prompt += keyboardKeys[controlMap["PLAYER2_UPGRADE"]];
	}
	p1Prompt += " UPGRADE\n";
	p2Prompt += " UPGRADE\n";

	if (controllerOwner == 1) {
		p1Prompt += "b";
	}
	else {
		p1Prompt += keyboardKeys[controlMap["PLAYER1_DESTROY"]];
	}
	if (controllerOwner == 2) {
		p2Prompt += "b";
	}
	else {
		p2Prompt += keyboardKeys[controlMap["PLAYER2_DESTROY"]];
	}
	p1Prompt += " DESTROY\n";
	p2Prompt += " DESTROY\n";

	// Draw the prompts for both players
	m_pRenderer->setFont("prompt_font");
	m_pRenderer->DrawScreenText(p1Prompt.c_str(), { 20,625 }, Colors::GhostWhite);
	if (coOp) {
		// Draw second player prompts
		m_pRenderer->DrawScreenText(p2Prompt.c_str(), { 1100,625 }, Colors::GhostWhite);
	}
	m_pRenderer->setFont("font");
}

void BaseLevelScene::_createPlayer(PlayerCharacter*& player, eSpriteType hat, string id) {
	// Place the player on the map
	if (hat == WIZARD_HAT)
		player = (PlayerCharacter*)addObject(new WizardPlayer(this, paths["plySpawn"].x, paths["plySpawn"].y));
	else if (hat == VIKING_HAT)
		player = (PlayerCharacter*)addObject(new VikingPlayer(this, paths["plySpawn"].x, paths["plySpawn"].y));
	else if (hat == CYBORG_HAT)
		player = (PlayerCharacter*)addObject(new CyborgPlayer(this, paths["plySpawn"].x, paths["plySpawn"].y));
	player->setPlayerID(id);
}

void BaseLevelScene::camFollowVision(GameObject* target, GameObject* target2) {
	shouldFollow = true;
	followTargets.first = target;
	followTargets.second = target2;
}

void BaseLevelScene::camMapVision() {
	if (tileSize > 0) {
		shouldFollow = false;
		followTargets.first = nullptr;
		followTargets.second = nullptr;
		m_pRenderer->SetCameraPos(Vector3((tilemapWidth / 2.0f) * tileSize, (tilemapHeight / 2.0f) * tileSize, -70.0f * tilemapHeight));
	}
}

void BaseLevelScene::handleControls() {
	// No need to get the keyboard state, it's done in the Game!
	// Just check values on the keyboard :)

	// Handle Player 1 controls
	if (controllerOwner == 1) {
		if (!towerMenus.first->isOpen())
			// We only handle player controls if their menu isn't open
			players.first->handleController();
		else
			players.first->setVelocity(0, players.first->getAngle());
		towerMenus.first->handleController();
	}
	else {
		if (!towerMenus.first->isOpen())
			// We only handle player controls if their menu isn't open
			players.first->handleKeyboard();
		else
			players.first->setVelocity(0, players.first->getAngle());
		towerMenus.first->handleKeyboard();
	}
	// Handle Player 2 controls
	if (coOp) {
		if (controllerOwner == 2) {
			if (!towerMenus.second->isOpen())
				// We only handle player controls if their menu isn't open
				players.second->handleController();
			else
				players.second->setVelocity(0, players.second->getAngle());
			towerMenus.second->handleController();
		}
		else {
			if (!towerMenus.second->isOpen())
				// We only handle player controls if their menu isn't open
				players.second->handleKeyboard();
			else
				players.second->setVelocity(0, players.second->getAngle());
			towerMenus.second->handleKeyboard();
		}
	}

	// Toggle camera mode
	if (m_pKeyboard->TriggerDown(controlMap["DEBUG_ZOOM"])) {
		// If we currently are zoomed in, zoom out
		if (shouldFollow) camMapVision();
		// If we are zoomed out, zoom in
		else {
			// Singleplayer or CoOp cam?
			if (coOp) {
				camFollowVision(players.first, players.second);
			}
			else {
				camFollowVision(players.first);
			}
		}
	}

	// Toggle drawing of tilemap collision boxes
	if (m_pKeyboard->TriggerDown(controlMap["DEBUG_SHOW_CLSN"]))
		drawMapClsn = drawMapClsn ? false : true;
}

void BaseLevelScene::handleMouse() {

}

void BaseLevelScene::cullObjects() {
	size_t sz = objectList.size();
	for (int i = 0; i < sz; i++) {
		if (objectList[i]->getWillCull()) {
			// If object is a baddie, remove it from the baddies list
			if (objectList[i]->is("baddie")) {
				for (int j = 0; j < baddies.size(); j++)
					if (objectList[i] == baddies[j]) {
						baddies.erase(baddies.begin() + j);
						break;
					}
			}
		}
	}
	Scene::cullObjects();
}

// Provides the player character pointer
PlayerCharacter* BaseLevelScene::getPlayer() {
	return players.first;
}
pair<PlayerCharacter*, PlayerCharacter*> BaseLevelScene::getPlayers() {
	return players;
}

BadCharacter* BaseLevelScene::addBaddie(BadCharacter* baddie) {
	addObject((GameObject*)baddie);
	baddies.push_back(baddie);
	return baddie;
}

vector<BadCharacter*> BaseLevelScene::getBaddies() {
	return baddies;
}

//returns the tower that is nearest to the player. used for deleting a tower now but could have some more future functionality
GameObject* BaseLevelScene::getClosestTower(PlayerCharacter* player) {
	Vector3 playerPos = player->getPosition();
	Vector3 towerPos;
	GameObject* tow = NULL;
	float difX = 1000, difY = 1000, dif = 1000;
	for (GameObject*& obj : objectList) {
		if (obj->is("tower")) {
			towerPos = obj->getPosition();
			difX = abs(playerPos.x - towerPos.x);
			difY = abs(playerPos.y - towerPos.y);
			if (sqrt(pow(difX, 2) + pow(difY, 2)) < dif) {
				dif = sqrt(pow(difX, 2) + pow(difY, 2));
				tow = obj;
			}
		}
	}
	if (tow == NULL)
		return NULL;
	else
		return tow;
}

float BaseLevelScene::distToTower(PlayerCharacter* player) {	//returns distance from player to closest tower
	GameObject* tow = getClosestTower(player);
	Vector3 towerPos = tow->getPosition();
	Vector3 playerPos = player->getPosition();
	float difX, difY, dist;
	difX = abs(playerPos.x - towerPos.x);
	difY = abs(playerPos.y - towerPos.y);
	dist = sqrt(pow(difX, 2) + pow(difY, 2));

	return dist;
}

void BaseLevelScene::endGame() {
	if (!gameOver) {
		gameOver = true;
		endTime = m_pStepTimer->GetTotalSeconds() + 15.0f;
	}
}

//creates all possible pairs of objects to pass to narrow phase
void BaseLevelScene::BroadPhaseClsn() {
	for (auto i : objectList) {
		for (auto j : objectList) {
			if (i != j && i->m_sAabb.Intersects(j->m_sAabb))
				NarrowPhaseClsn(i, j);
		}
	}
	for (const auto& box : clsnBoxes) {
		for (auto obj : objectList) {
			if (obj->m_sAabb.Intersects(box))
				TilemapClsn(obj);
		}
	}
}


// Handler for collisions, check for specific object types and handle collisions for them
void BaseLevelScene::NarrowPhaseClsn(GameObject* obj1, GameObject* obj2) {
	// Stop the player (or enemies) from moving into solid objects
	if (obj1->is("player") && obj2->is("unpassable")) {
		obj1->absoluteStop();
	}
	if (obj1->is("player") && obj2->is("baddie")) {
		PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(obj1);
		if (!player->isDead) {
			obj1->pushBack();
			obj2->pushBack();
		}
	}
	// Enemies running into the spire
	if (obj1->is("baddie") && obj2->is("spire")) {
		obj1->absoluteStop();
	}
	// Enemies running into a tower
	if (obj1->is("baddie") && obj2->is("tower")) {
		Tower* tower = dynamic_cast<Tower*>(obj2);
		tower->boom();
	}
	// Enemies running into each other
	if (obj1->is("baddie") && obj2->is("baddie")) {
		if (obj2->timeExisted() > 1.5f) {
			// Get the vector between the two objects positions
			Vector2 between = obj2->getPosition() - obj1->getPosition();
			// Get just the direction between the two objects
			between.Normalize();
			// Change the direction of the bad guy a little bit
			if (between.x > between.y) between.x *= 10.0f;
			else between.y *= 10.0f;
			// Force the second object to go around
			obj2->addForce(between, 0.05f);
			// Prevent object overlap
			obj2->pushBack();
		}
	}
	// Collecting of coins
	if (obj1->is("player") && obj2->is("coins")) {
		m_pAudio->play(COIN_GRAB);
		addCredits(((Coin*)obj2)->getValue());
		obj2->kill();
	}
	// Behavior when a projectile hits an enemy
	if (obj1->is("projectile") && obj2->is("baddie")) {
		Projectile* p = dynamic_cast<Projectile*>(obj1);
		BadCharacter* baddie = dynamic_cast<BadCharacter*>(obj2);
		if (!baddie->isDying() && m_pStepTimer->GetTotalSeconds() > baddie->timeCanHit) {
			baddie->damage(p->damage);
			// Knockback from the projectile
			if (obj1->is("piercing")) {
				// Piercing projectiles have much lower knockback
				obj2->addForce(obj1->getNetForce() * 0.05f, 0.25f);
			}
			else
			{
				obj2->addForce(obj1->getNetForce() * 0.25, 0.25f);
			}
			// Don't kill if object is explosive, or piercing, they'll handle expiration elsewhere
			if (!obj1->is("explosive") && !obj1->is("piercing"))
				obj1->kill();
			// Update when the baddie can next be hit
			baddie->timeCanHit = m_pStepTimer->GetTotalSeconds() + baddie->invincibleTime;
		}
	}
	// When explosive hits baddie, kill baddies within certain radius of fireball
	if (obj1->is("explosive") && obj2->is("baddie")) {
		if (!dynamic_cast<BadCharacter*>(obj2)->isDying()) {
			// Object as an explosive
			Explosive* expl = dynamic_cast<Explosive*>(obj1);
			// Get all bad guys in the scene
			for (BadCharacter* obj : this->getBaddies()) {
				if (obj != obj2 && obj->inRadius(obj1->getPosition(), expl->radius) && !obj->isDying() && !expl->detonated) {
					// Damage the bad guy
					obj->damage(expl->splash);
					// Get the vector between the explosion center and the object
					Vector2 force = obj->getPosition() - obj1->getPosition();
					force.Normalize();
					// Scale the knockback force
					force *= 35.0f;
					// Apply knockback to enemy
					obj->addForce(force, 0.25f);
				}
			}
			// Destroy the explosive
			obj1->kill();
		}
	}
	// Projectiles should die when hitting the spire
	if (obj1->is("projectile") && obj2->is("spire")) obj1->kill();
}

void BaseLevelScene::TilemapClsn(GameObject* obj) {
	if (obj->is("player") || obj->is("baddie")) {
		obj->absoluteStop();
	}
	if (obj->is("projectile")) obj->kill();
}

int BaseLevelScene::getCredits() {
	return credits;
}
void BaseLevelScene::addCredits(int amount) {
	credits += amount;
}
void BaseLevelScene::removeCredits(int amount) {
	credits -= amount;
}

void BaseLevelScene::upgradeTower(PlayerCharacter* upgrader) {
	GameObject* tow = getClosestTower(upgrader);
	GameObject* newTow;
	if (tow != NULL && distToTower(upgrader) <= 75) {
		if (tow->is("range")) {
			if (tow->is("lvl0")) {
				if (credits >= 100) {
					newTow = addObject(new RangeTower(this, RANGE2, 1, tow->getPosition().x, tow->getPosition().y));
					newTow->m_fXScale = 1.7f;
					newTow->m_fYScale = 1.7f;
					tow->kill();
					credits -= 100;
				}
			}
			if (tow->is("lvl1")) {
				if (credits >= 200) {
					newTow = addObject(new RangeTower(this, RANGE3, 2, tow->getPosition().x, tow->getPosition().y));
					newTow->m_fXScale = 1.7f;
					newTow->m_fYScale = 1.7f;
					tow->kill();
					credits -= 200;
				}
			}
		}
		if (tow->is("mage")) {
			if (tow->is("lvl0")) {
				if (credits >= 200) {
					newTow = addObject(new MageTower(this, MAGE2, 1, tow->getPosition().x, tow->getPosition().y));
					tow->kill();
					credits -= 200;
				}
			}
			if (tow->is("lvl1")) {
				if (credits >= 400) {
					newTow = addObject(new MageTower(this, MAGE3, 2, tow->getPosition().x, tow->getPosition().y));
					tow->kill();
					newTow->m_fXScale = 0.7f;
					newTow->m_fYScale = 0.7f;
					credits -= 400;
				}
			}
		}
	}
}

void BaseLevelScene::destroyTower(PlayerCharacter* upgrader) {
	GameObject* tow = getClosestTower(upgrader);
	if (tow != NULL && distToTower(upgrader) <= 75) {
		if (tow->is("range")) {
			credits += 3;
		}
		if (tow->is("mage")) {
			credits += 8;
		}
		deleteObject(tow);
	}
}

void BaseLevelScene::createSpawnDescriptors() {
	// Define some spawn descriptors for the wave spawner
	SpawnDesc goblins;
	goblins.addPerWave = 10;
	goblins.multiplyPerWave = 1.6f;
	goblins.nextSpawn = m_pStepTimer->GetTotalSeconds();
	goblins.numberSpawned = 0;
	goblins.ready = true;
	goblins.startWave = 1;
	goblins.timeAddPerSpawn = -0.01f;
	goblins.timeMultiplyPerSpawn = 0.9f;
	goblins.timeToSpawn = 3.0f;
	SpawnDesc minotaurs;
	minotaurs.key = "minotaur";
	minotaurs.addPerWave = 3;
	minotaurs.multiplyPerWave = 1.3f;
	minotaurs.nextSpawn = m_pStepTimer->GetTotalSeconds() + 6.0f;
	minotaurs.numberSpawned = 0;
	minotaurs.ready = true;
	minotaurs.startWave = 2;
	minotaurs.timeAddPerSpawn = -3.0f;
	minotaurs.timeMultiplyPerSpawn = 1.0f;
	minotaurs.timeToSpawn = 12.0f;
	SpawnDesc wolves;
	wolves.key = "wolf";
	wolves.addPerWave = 3;
	wolves.multiplyPerWave = 1.3f;
	wolves.nextSpawn = m_pStepTimer->GetTotalSeconds() + 6.0f;
	wolves.numberSpawned = 0;
	wolves.ready = true;
	wolves.startWave = 2;
	wolves.timeAddPerSpawn = -3.0f;
	wolves.timeMultiplyPerSpawn = 1.0f;
	wolves.timeToSpawn = 12.0f;
	// Add the spawn descriptions
	waves->addSpawnDesc(goblins);
	waves->addSpawnDesc(minotaurs);
	waves->addSpawnDesc(wolves);
}
