#pragma once
#include <utility>
#include <string>
#include <vector>
#include "Scene.h"

class CGame;

class HatSelectScene :
	public Scene
{
public:
	HatSelectScene(CGame* game, bool coOp = false);
	~HatSelectScene();
	void create() override;
	void update() override;
	void render() override;
	void startLevel();
protected:
	vector<pair<string, eSpriteType>> hatsMenu;
	pair<HUDObject*, HUDObject*> hats;
	pair<int, int> selection;
	pair<bool, bool> ready;
	bool coOp;
	bool controllerReady = true;
};

