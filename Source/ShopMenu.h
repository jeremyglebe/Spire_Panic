#pragma once
#include "HUDObject.h"
class ShopMenu :
	public HUDObject,
	public CComponent
{
public:
	ShopMenu(Scene* scene, float xPercentage, float yPercentage, string playerID = "PLAYER1");
	void renderText();
	bool isOpen();
	void toggle();
	void buy();
	void nextSelection();
	void prevSelection();
	void handleKeyboard();
	void handleController();
private:
	string playerID;
	bool open = false;
	HUDObject* selectedTower = nullptr;
	int selection = 0;
	int numChoices = 2;
	string selectionName = "";
	string selectionDescription = "";
	int cost = 0;
	void updateSelection();
};

