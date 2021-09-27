#pragma once
#include "TiledScene.h"
#include "ShopMenu.h"
#include "PlayerCharacter.h"
#include "BadCharacter.h"
#include "Spire.h"
#include "Tower.h"
#include "Fireball.h"
#include "SpawnDesc.h"
#include "WaveSpawner.h"


// Forward declarations for circular dependency
class CGame;

class BaseLevelScene :
	public TiledScene
{
public:
	BaseLevelScene(CGame* game, eSpriteType ply1Hat = WIZARD_HAT, eSpriteType ply2Hat = WIZARD_HAT, bool coop = false);
	virtual ~BaseLevelScene() override;
	virtual void create() override;
	virtual void update() override;
	virtual void render() override;
	// Starts timer to end the game
	void endGame();
	bool getGameOver() { return gameOver; }

	// Set the camera to follow a target
	void camFollowVision(GameObject* target, GameObject* target2 = nullptr);
	// Set the camera to center on the tilemap
	void camMapVision();

	BadCharacter* addBaddie(BadCharacter* baddie);
	int getCredits();
	void addCredits(int amount);
	void removeCredits(int amount);
	void destroyTower(PlayerCharacter* upgrader);
	void upgradeTower(PlayerCharacter* upgrader);

	PlayerCharacter* getPlayer(); // Returns the current player object as a pointer
	pair<PlayerCharacter*, PlayerCharacter*> BaseLevelScene::getPlayers();
	GameObject* getClosestTower(PlayerCharacter* player);//returns the tower closest to the PC
	float distToTower(PlayerCharacter* player);
	vector<BadCharacter*> getBaddies(); //returns the vector containing enemy objects
	Spire* getSpire() { return spire; }
	void setSpire(Spire* sp) { spire = sp; }

	// Checks collision boxes of every object against every other object
	void BroadPhaseClsn();
	// Finds objects which are flagged to be removed and deletes them, removes from game lists
	void cullObjects() override;

protected:
	string tileMapFileName = "simple";
	eSpriteType tileSetSprite = SIMPLE_TILE;
	pair<ShopMenu*, ShopMenu*> towerMenus;
	bool gameOver = false;
	float endTime = 0;
	int credits = 50;
	bool coOp;
	WaveSpawner* waves = nullptr;
	// References to player characters
	pair<PlayerCharacter*, PlayerCharacter*> players = { nullptr, nullptr };
	pair<eSpriteType, eSpriteType> hats;
	// Reference to spire which must be defended
	Spire* spire = nullptr;
	// List of enemies
	vector<BadCharacter*> baddies;
	// 1-2 objects that the camera follows
	pair<GameObject*, GameObject*> followTargets = { nullptr, nullptr };
	// Default "true size" camera position
	Vector3 defPos;
	// Whether the camera is following
	bool shouldFollow = false;
	// Function to create the wave spawning descriptors
	virtual void createSpawnDescriptors();
	// Controlling the game
	void handleControls();
	void handleMouse();
	// Creates a player object
	void _createPlayer(PlayerCharacter*& player, eSpriteType hat, string id = "PLAYER1");
	// Collision handling
	void NarrowPhaseClsn(GameObject* obj1, GameObject* obj2);
	void TilemapClsn(GameObject* obj);
};

