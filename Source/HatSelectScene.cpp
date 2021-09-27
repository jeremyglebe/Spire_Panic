#include "HatSelectScene.h"
#include "Game.h"
#include "Controller.h"
#include "GamepadWrapper.h"
#include "BaseLevelScene.h"
#include "DesolateDesertLevel.h"
#include "LevelSelectScene.h"

HatSelectScene::HatSelectScene(CGame* game, bool coOp) : Scene(game) {
	this->coOp = coOp;
	hatsMenu = {
		{"Loaf", WIZARD_HAT},
		{"Doepler", VIKING_HAT},
		{"Neuro", CYBORG_HAT}
	};
	selection.first = 0;
	selection.second = 0;
}

HatSelectScene::~HatSelectScene() {}

void HatSelectScene::create() {
	if (coOp) {
		// Co-operative
		hats.first = addObject(new HUDObject(this, WIZARD_HAT, -.5f, 0, 0));
		hats.first->m_fXScale = 4.0f;
		hats.first->m_fYScale = 4.0f;
		// Player 2
		hats.second = addObject(new HUDObject(this, WIZARD_HAT, .5f, 0, 0));
		hats.second->m_fXScale = 4.0f;
		hats.second->m_fYScale = 4.0f;
	}
	else {
		// Singleplayer
		hats.first = addObject(new HUDObject(this, WIZARD_HAT, 0, 0, 0));
		hats.first->m_fXScale = 4.0f;
		hats.first->m_fYScale = 4.0f;
		hats.second = nullptr;
	}
}

void HatSelectScene::update() {
	hats.first->m_nSpriteIndex = hatsMenu[selection.first].second;
	// Control the selection with keyboard
	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_LEFT"])) selection.first--;
	else if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_RIGHT"])) selection.first =
		(selection.first + 1) % (int)hatsMenu.size();
	// Control the selection with controller
	if (controllerOwner == 1) {
		if (gamepad.isPressed("LSTICK_LEFT")) selection.first--;
		else if (gamepad.isPressed("LSTICK_RIGHT")) selection.first =
			(selection.first + 1) % (int)hatsMenu.size();
	}
	// If it's below zero, loop selection around
	if (selection.first < 0) selection.first = (int)hatsMenu.size() - 1;

	// Controlling the menu for co-op player
	if (coOp) {
		hats.second->m_nSpriteIndex = hatsMenu[selection.second].second;
		// Control the selection with keyboard
		if (m_pKeyboard->TriggerDown(controlMap["PLAYER2_LEFT"])) selection.second--;
		else if (m_pKeyboard->TriggerDown(controlMap["PLAYER2_RIGHT"])) selection.second =
			(selection.second + 1) % (int)hatsMenu.size();
		// Control the selection with controller
		if (controllerOwner == 2) {
			if (gamepad.isPressed("LSTICK_LEFT")) selection.second--;
			else if (gamepad.isPressed("LSTICK_RIGHT")) selection.second =
				(selection.second + 1) % (int)hatsMenu.size();
		}
		// If it's below zero, loop selection around
		if (selection.second < 0) selection.second = (int)hatsMenu.size() - 1;
	}

	// Check for selection confirmation
	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_ACCEPT"])) {
		ready.first = true;
	}
	if (m_pKeyboard->TriggerDown(controlMap["PLAYER2_ACCEPT"])) {
		ready.second = true;
	}
	if (gamepad.isPressed("BUTTON_A")) {
		if (controllerOwner == 1) ready.first = true;
		else if (controllerOwner == 2) ready.second = true;
	}

	// Proceed when both players have readied.
	if (ready.first && (ready.second || !coOp)) startLevel();
}

void HatSelectScene::render() {
	Scene::render();
	m_pRenderer->setFont("title_font");
	m_pRenderer->DrawScreenText("Choose your hat!", Vector2(330, 10), Colors::Black);
	m_pRenderer->setFont("font");
	m_pRenderer->DrawScreenText("(and with it, your destiny...)", Vector2(470, 100), Colors::Black);
	// Position hat name text of Player 1
	float x = hats.first->m_vPos.x - (7.0f * hatsMenu[selection.first].first.size());
	m_pRenderer->DrawScreenText(hatsMenu[selection.first].first.c_str(), Vector2(x, 420), Colors::Black);
	// Display the player's ready status
	m_pRenderer->setFont("prompt_font");
	if (ready.first) {
		if (coOp)
			m_pRenderer->DrawScreenText("READY!", Vector2(290, 530), Colors::Black);
		else
			m_pRenderer->DrawScreenText("READY!", Vector2(540, 530), Colors::Black);
	}
	else
	{
		string prompt = "PRESS ";
		if (controllerOwner == 1) {
			prompt += "a";
		}
		else {
			prompt += keyboardKeys[controlMap["PLAYER1_ACCEPT"]];
		}
		prompt += " TO CONFIRM YOUR CHOICE!";
		if (coOp)
			m_pRenderer->DrawScreenText(prompt.c_str(), Vector2(150, 530), Colors::Black);
		else
			m_pRenderer->DrawScreenText(prompt.c_str(), Vector2(450, 530), Colors::Black);
	}
	if (coOp) {
		m_pRenderer->setFont("font");
		// Position hat name text of Player 2
		x = hats.second->m_vPos.x - (7.0f * hatsMenu[selection.second].first.size());
		m_pRenderer->DrawScreenText(hatsMenu[selection.second].first.c_str(), Vector2(x, 420), Colors::Black);
		m_pRenderer->setFont("prompt_font");
		// Display the player's ready status
		if (ready.second) {
			m_pRenderer->DrawScreenText("READY!", Vector2(915, 530), Colors::Black);
		}
		else
		{
			string prompt = "PRESS ";
			if (controllerOwner == 2) {
				prompt += "a";
			}
			else {
				prompt += keyboardKeys[controlMap["PLAYER2_ACCEPT"]];
			}
			prompt += " TO CONFIRM YOUR CHOICE!";
			m_pRenderer->DrawScreenText(prompt.c_str(), Vector2(775, 530), Colors::Black);
		}
	}
	m_pRenderer->setFont("font");
}

void HatSelectScene::startLevel() {
	game->startScene(new LevelSelectScene(game, 
		hatsMenu[selection.first].second,
		hatsMenu[selection.second].second, coOp));
}