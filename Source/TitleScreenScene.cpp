#include "TitleScreenScene.h"
#include "HatSelectScene.h"
#include "OptionsScene.h"
#include "Game.h"
#include "Abort.h"
#include "Controller.h"

TitleScreenScene::TitleScreenScene(CGame* game) : Scene(game) {
	menu.push_back("Play Singleplayer");
	menu.push_back("Play Cooperative");
	menu.push_back("Options/Hints");
	menu.push_back("Exit");
}

TitleScreenScene::~TitleScreenScene() {}

void TitleScreenScene::create() {
	printf("TitleScreenScene::create()\n");
	// Play a song
	m_pAudio->loop(MENU_SONG);
	// Create the title screen image
	float x = m_pRenderer->getCenter().x;
	float y = m_pRenderer->getCenter().y;
	addObject(new GameObject(this, TITLE_SCREEN, x, y, 0));
	// Create a background for the text prompt
	HUDObject* box = addObject(new HUDObject(this, BLACK_SQUARE, -0.49f, -0.92f));
	box->m_fAlpha = 0.3f;
	box->relativeXScale = 220.0f;
	box->relativeYScale = 25.0f;
	box->adjust();
}

void TitleScreenScene::update() {
	_handleKeyboard();
	if (controllerOwner > -1)
		_handleController();
}

void TitleScreenScene::render() {
	Scene::render();
	m_pRenderer->setFont("title_font");
	m_pRenderer->DrawScreenText("Spire Panic!", Vector2(710, 80), Colors::OrangeRed);
	m_pRenderer->setFont("font");
	for (int i = 0; i < menu.size(); i++) {
		if (i == selection) {
			m_pRenderer->setFont("selection_font");
			m_pRenderer->DrawScreenText(menu[i].c_str(), Vector2(780.0f, 180.0f + (40.0f * i)), Colors::DarkOrange);
			m_pRenderer->setFont("font");
		}
		else {
			m_pRenderer->DrawScreenText(menu[i].c_str(), Vector2(780.0f, 180.0f + (40.0f * i)), Colors::Orange);
		}
	}
	m_pRenderer->setFont("prompt_font");
	string controllerPrompt = "h USE i m TO NAVIGATE MENU.  PRESS a TO CONFIRM";
	string keyboardPrompt = "KEYBOARD USE " + keyboardKeys[controlMap["PLAYER1_UP"]]
		+ "/" + keyboardKeys[controlMap["PLAYER1_DOWN"]] + " TO NAVIGATE MENU.  PRESS "
		+ keyboardKeys[controlMap["PLAYER1_ACCEPT"]] + " TO CONFIRM";
	m_pRenderer->DrawScreenText(controllerPrompt.c_str(), { 20,660 }, Colors::LightGreen);
	m_pRenderer->DrawScreenText(keyboardPrompt.c_str(), { 20,690 }, Colors::LightCyan);
}

void TitleScreenScene::_handleKeyboard() {
	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_UP"]))
		if (selection > 0) selection--;
	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_DOWN"]))
		if (selection < menu.size() - 1) selection++;

	// Making a selection
	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_ACCEPT"])) {
		_accept();
	}
}

void TitleScreenScene::_handleController() {
	// Selecting a menu item
	if (gamepad.isPressed("LSTICK_UP"))
		if (selection > 0) selection--;
	if (gamepad.isPressed("LSTICK_DOWN"))
		if (selection < menu.size() - 1) selection++;
	// Confirming a selection
	if (gamepad.isPressed("BUTTON_A")) {
		_accept();
	}
}

void TitleScreenScene::_accept() {
	if (menu[selection] == "Play Singleplayer") {
		if (controllerOwner == 2)
			controllerOwner = 1;
		game->startScene(new HatSelectScene(game));
	}
	else if (menu[selection] == "Play Cooperative") {
		game->startScene(new HatSelectScene(game, true));
	}
	else if (menu[selection] == "Options/Hints") {
		game->startScene(new OptionsScene(game));
	}
	else if (menu[selection] == "Exit") {
		game->Exit();
	}
}