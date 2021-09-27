#pragma once
#include <string>
#include <utility>
#include <vector>
#include "Scene.h"

class CGame;

class OptionsScene :
	public Scene
{
public:
	OptionsScene(CGame* game);
	void update() override;
	void render() override;
private:
	vector<pair<string, WPARAM>> controlsList;
	bool controlsListSelected;
	int selectedControl;
	bool pending;
	void render_controls();
};

