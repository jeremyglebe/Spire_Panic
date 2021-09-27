#include "Game.h"
#include "LevelSelectScene.h"
#include "BaseLevelScene.h"
#include "DesolateDesertLevel.h"
#include "PlainsLevel.h"

LevelSelectScene::LevelSelectScene(CGame* game, eSpriteType ply1Hat, eSpriteType ply2Hat, bool coOp) : Scene(game) {

	this->coOp = coOp;

	LevelMenu = {
		{"Log Town", BASELEVEL},
		{"Desolate Desert", DESERTLEVEL},
		{"Forgotten Plains", PLAINSLEVEL}
	};

	selection.first = 0;
	selection.second = 0;
	p1Hat = ply1Hat;
	p2Hat = ply2Hat;
}

LevelSelectScene::~LevelSelectScene() {};

void LevelSelectScene::create() {
	level = addObject(new HUDObject(this, BASELEVEL, 0, .05f, 0));
	level->m_fXScale = 0.7f;
	level->m_fYScale = 0.7f;

}

void LevelSelectScene::update() {
	level->m_nSpriteIndex = LevelMenu[selection.first].second;

	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_LEFT"])) selection.first--;
	else if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_RIGHT"])) selection.first =
		(selection.first + 1) % (int)LevelMenu.size();

	// Control the selection with controller
	if (controllerOwner == 1) {
		if (gamepad.isPressed("LSTICK_LEFT")) selection.first--;
		else if (gamepad.isPressed("LSTICK_RIGHT")) selection.first =
			(selection.first + 1) % (int)LevelMenu.size();
		if (gamepad.isPressed("BUTTON_A")) {
			startLevel();
		}
	}

	// If it's below zero, loop selection around
	if (selection.first < 0) selection.first = (int)LevelMenu.size() - 1;

	if (m_pKeyboard->TriggerDown(controlMap["PLAYER1_ACCEPT"])) {
		startLevel();
	}
}

void LevelSelectScene::render() {
	Scene::render();

	m_pRenderer->setFont("title_font");
	m_pRenderer->DrawScreenText("Level Select", Vector2(400, 0), Colors::Black);
	string prompt = "PRESS ";
	if (controllerOwner == 1) {
		prompt += "a";
	}
	else {
		prompt += keyboardKeys[controlMap["PLAYER1_ACCEPT"]];
	}
	prompt += " TO BEGIN!\nDEFEND THE SPIRE!";
	m_pRenderer->setFont("font");
	m_pRenderer->DrawScreenText(LevelMenu[selection.first].first.c_str(), Vector2(580, 600), Colors::Black);
	m_pRenderer->setFont("prompt_font");
	m_pRenderer->DrawScreenText(prompt.c_str(), Vector2(540, 650), Colors::Black);
	m_pRenderer->setFont("font");
}

void LevelSelectScene::startLevel() {
	m_pAudio->stop();
	if (selection.first == 0) {
		game->startScene(new BaseLevelScene(game,
			p1Hat, p2Hat, coOp));
	}
	else if (selection.first == 1) {
		game->startScene(new DesolateDesertLevel(game,
			p1Hat, p2Hat, coOp));
	}
	else if (selection.first == 2) {
		game->startScene(new PlainsLevel(game,
			p1Hat, p2Hat, coOp));
	}
}

