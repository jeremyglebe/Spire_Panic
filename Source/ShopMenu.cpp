#include "ShopMenu.h"
#include "Scene.h"
#include "BaseLevelScene.h"
#include "RangeTower.h"
#include "MageTower.h"

ShopMenu::ShopMenu(Scene* scene, float xPercentage, float yPercentage, string playerID)
	: HUDObject(scene, MENU_BOX, xPercentage, yPercentage)
{
	this->playerID = playerID;
	relativeXScale = 0.4f;
	relativeYScale = 0.4f;
	m_fAlpha = 0.0f;
	// The tower to be displayed in the menu
	selectedTower = scene->addObject(new HUDObject(scene, RANGE1, xPercentage, yPercentage + 0.1f, -0.1f));
	selectedTower->m_fAlpha = 0.0f;
	updateSelection();
}

bool ShopMenu::isOpen() { return open; }

void ShopMenu::toggle() {
	if (open) {
		open = false;
		m_fAlpha = 0.0f;
		selectedTower->m_fAlpha = 0.0f;
	}
	else {
		open = true;
		m_fAlpha = 1.0f;
		selectedTower->m_fAlpha = 1.0f;
	}
}

void ShopMenu::nextSelection() {
	selection += 1;
	selection %= numChoices;
	updateSelection();
}

void ShopMenu::prevSelection() {
	selection -= 1;
	if (selection < 0) selection = numChoices - 1;
	updateSelection();
}

void ShopMenu::updateSelection() {
	switch (selection) {
	case 0:
		selectedTower->m_nSpriteIndex = RANGE1;
		selectionName = "Gun Tower";
		selectionDescription = "Shoots bullets. Questions?     ";
		cost = 50;
		break;
	case 1:
		selectedTower->m_nSpriteIndex = MAGE1;
		selectionName = "Magic Tower";
		selectionDescription = "Crowd control   for pyromaniacs";
		cost = 100;
		break;
	default:
		break;
	}
}

void ShopMenu::handleKeyboard() {
	if (m_pKeyboard->TriggerDown(controlMap[playerID + "_MENU"])) {
		toggle();
	}
	if (open) {
		if (m_pKeyboard->TriggerDown(controlMap[playerID + "_RIGHT"])) {
			nextSelection();
		}
		if (m_pKeyboard->TriggerDown(controlMap[playerID + "_LEFT"])) {
			prevSelection();
		}
		if (m_pKeyboard->TriggerDown(controlMap[playerID + "_ACCEPT"])) {
			buy();
		}
	}
}

void ShopMenu::handleController() {
	if (gamepad.isPressed("BUTTON_Y")) {
		toggle();
	}
	if (open) {
		if (gamepad.isPressed("LSTICK_RIGHT")) {
			nextSelection();
		}
		if (gamepad.isPressed("LSTICK_LEFT")) {
			prevSelection();
		}
		if (gamepad.isPressed("BUTTON_A")) {
			buy();
		}
	}
}

void ShopMenu::buy() {
	BaseLevelScene* level = (BaseLevelScene*)scene;
	// Check if we have enough money
	if (level->getCredits() >= cost) {
		PlayerCharacter* player = playerID == "PLAYER1" ? level->getPlayers().first : level->getPlayers().second;
		Vector3 pos = player->getPosition();
		// Determine which tower to place
		switch (selection) {
		case 0:
			scene->addObject(new RangeTower(scene, RANGE1, 0, pos.x, pos.y - 32.0f));
			break;
		case 1:
			scene->addObject(new MageTower(scene, MAGE1, 0, pos.x, pos.y - 32.0f));
			break;
		default:
			break;
		}
		level->removeCredits(cost);
		toggle();
	}
}

void ShopMenu::renderText() {
	if (open) {
		// Center x and y coordinates
		float centerX = 640 + (xPercentage * 640);
		float centerY = 360 - (yPercentage * 360);
		// I have absolutely no idea why this is necessary. I looked for the answer for days.
		// I have scoured ancient tomes. I have summoned spirits and delved into regions of space that
		// pre-date mankind in search of the reason why I need to do this. I don't know. For whatever reason,
		// the x position is slightly off only when the menu is on the left side of the screen. I've
		// given up entirely. We make this ugly little adjustment to preserve what remains of my sanity.
		if (xPercentage < 0) centerX += 14;
		// Draw the name of the tower
		float nameX = centerX - (selectionName.size() * 7.5f);
		m_pRenderer->DrawScreenText(selectionName.c_str(), { nameX, centerY - 120 }, Colors::GhostWhite);
		// Draw the description of the tower
		string desc1 = selectionDescription.substr(0, 16);
		string desc2 = selectionDescription.substr(16);
		float descX = centerX - (desc1.size() * 6.5f);
		m_pRenderer->DrawScreenText(desc1.c_str(), { descX, centerY + 24 }, Colors::GhostWhite);
		descX = centerX - (desc2.size() * 6.5f);
		m_pRenderer->DrawScreenText(desc2.c_str(), { descX, centerY + 50 }, Colors::GhostWhite);
		// Draw the cost of the tower
		string costStr = "Buy for " + to_string(cost) + " coins";
		float costX = centerX - (costStr.size() * 6.5f);
		m_pRenderer->DrawScreenText(costStr.c_str(), { costX, centerY + 80 }, Colors::GhostWhite);
	}
}