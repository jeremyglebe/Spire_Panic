#pragma once
#include <utility>
#include <string>
#include <vector>
#include "Scene.h"

class CGame;

class LevelSelectScene :
	public Scene
{
public:
	LevelSelectScene(CGame* game, eSpriteType ply1Hat, eSpriteType ply2Hat, bool coOp = false);
	~LevelSelectScene();
	void create() override;
	void update() override;
	void render() override;
	void startLevel();
protected:
	vector<pair<string, eSpriteType>> LevelMenu;
	HUDObject* level;
	pair<int, int> selection;
	pair<bool, bool> ready;
	GameObject* controllerPrompt;
	bool coOp;
	bool controllerReady = true;
	eSpriteType p1Hat;
	eSpriteType p2Hat;
};