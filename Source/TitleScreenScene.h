#pragma once
#include "Scene.h"

// Forward declarations of classes that will exist
// (We can't just include it b/c of circular dependency
class CGame;

class TitleScreenScene :
	public Scene
{
public:
	TitleScreenScene(CGame* game);
	virtual ~TitleScreenScene() override;
	virtual void create() override;
	virtual void update() override;
	virtual void render() override;
private:
	vector<string> menu;
	int selection = 0;
	void _accept();
	void _handleKeyboard();
	void _handleController();
	GameObject* physBall = nullptr;
};

