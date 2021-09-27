#include "OptionsScene.h"
#include "Game.h"

OptionsScene::OptionsScene(CGame* game) : Scene(game)
{
	controlsListSelected = true;
	selectedControl = 0;
	pending = false;
	// Create the controls list
	for (auto c : controlMap) controlsList.push_back({ c.first, c.second });
}

void OptionsScene::update() {
	if (pending) {
		for (auto key : keyboardKeys) {
			if (m_pKeyboard->TriggerDown(key.first)) {
				// Update the control
				controlMap[controlsList[selectedControl].first] = key.first;
				controlsList[selectedControl].second = key.first;
				// Cease pending
				pending = false;
			}
		}
	}
	else if (controlsListSelected) {
		// Handle the player moving through the list
		if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_UP"])) selectedControl--;
		else if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_DOWN"])) selectedControl++;
		if (selectedControl >= (int)controlsList.size()) selectedControl = 0;
		else if (selectedControl < 0) selectedControl = (int)controlsList.size() - 1;
		// Set to pending if the player wants to change a control
		if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_ACCEPT"])) pending = true;
	}
	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_BACK"])) {
		game->startScene(new TitleScreenScene(game));
	}
	if (gamepad.isPressed("BUTTON_B")) {
		game->startScene(new TitleScreenScene(game));
	}
}

void OptionsScene::render() {
	Scene::render();
	m_pRenderer->setFont("font");
	render_controls();
	string prompts = "USE "
		+ keyboardKeys[controlMap["PLAYER1_UP"]] + " AND "
		+ keyboardKeys[controlMap["PLAYER1_DOWN"]]
		+ " TO SELECT A CONTROL\n"
		+ "PRESS "
		+ keyboardKeys[controlMap["PLAYER1_ACCEPT"]]
		+ " TO MODIFY THE SELECTED CONTROL\n"
		+ "PRESS "
		+ keyboardKeys[controlMap["PLAYER1_BACK"]];
	if (gamepad.connected())
		prompts += " OR b";
	prompts += " TO EXIT THIS MENU\n";
	m_pRenderer->setFont("prompt_font");
	m_pRenderer->DrawScreenText(prompts.c_str(), { 15,15 }, Colors::LimeGreen);
	// Draw some hints on the left
	string hints = "GAME HELP:\n";
	hints += "Don't place towers in the path of enemies,\n";
	hints += "they'll be destroyed!\n";
	hints += "Tower upgrade prices aren't listed, but each\n";
	hints += "upgrade is twice the cost of the previous stage.\n";
	m_pRenderer->setFont("font");
	m_pRenderer->DrawScreenText(hints.c_str(), { 15, 300.0f }, Colors::LimeGreen);
}

void OptionsScene::render_controls() {
	float keysXOffset = 800.0f;
	// Draw the names of controls
	for (int r = 0; r < controlsList.size(); r++) {
		auto code = controlsList[r].first;
		auto key = controlsList[r].second;
		// Calculate the Y position
		int offset = selectedControl - r;
		float y = 344.0f - (offset * 32);
		// Draw the text for the selected control
		if (r == selectedControl) {
			m_pRenderer->DrawScreenText(code.c_str(),
				{ keysXOffset, y }, Colors::White);
		}
		// Draw the text for all the unselected controls
		else {
			m_pRenderer->DrawScreenText(code.c_str(),
				{ keysXOffset, y }, Colors::LimeGreen);
		}
		if (!pending || r != selectedControl) {
			// Draw the key currently mapped to the control
			m_pRenderer->DrawScreenText(
				("[ " + keyboardKeys[key] + " ]").c_str(),
				{ 1050.0f, y }, Colors::LimeGreen);
		}
		else {
			// Draw pending text
			m_pRenderer->DrawScreenText("[ Press any key ]",
				{ 1050.0f, y }, Colors::Red);
		}
	}
}