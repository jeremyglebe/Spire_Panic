/// \file Game.h
/// \brief Interface for the game class CGame.

#pragma once

#include "Component.h"
#include "Common.h"
#include "Settings.h"
#include "TitleScreenScene.h"

/// \brief The game class.
class CGame :
	public CWindowDesc,
	public CComponent,
	public CSettings,
	public CCommon {

private:
	Scene* activeScene = nullptr;
	bool displayFramerate = false;
	bool willExit = false;
	void RenderFrame();
public:
	~CGame(); ///< Destructor.

	void Initialize(); ///< Initialize the game.
	void ProcessFrame(); ///< Process an animation frame.
	void Release(); ///< Release the renderer.
	void Exit();

	// Starts new active scene and terminates the previous
	// Only use this if you are switching scenes entirely, not stacking!
	void startScene(Scene* scene);

}; //CGame
