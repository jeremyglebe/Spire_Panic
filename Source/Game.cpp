/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

CGame::~CGame() {
	delete m_pRenderer;
	if (activeScene) {
		delete activeScene;
		activeScene = nullptr;
	}
} //destructor

/// Initialize the renderer and the object manager, load 
/// images and sounds, and begin the game.
void CGame::Initialize() {
	m_pRenderer = new CRenderer;
	m_pRenderer->Initialize(NUM_SPRITES);
	m_pRenderer->LoadImages(); //load images from xml file list
	m_pAudio->Load(); //load the sounds for this game
	
	if (gamepad.connected()) controllerOwner = 2;

	// Start the initial scene, maybe this should be a main menu?
	// (We pass a pointer to this object because the scene has a circular
	// reference for things such as swapping itself with another scene)
	startScene(new TitleScreenScene(this));
} //Initialize

// Code executed at every new frame
void CGame::ProcessFrame() {
	if (willExit)
		// Exit via destroying the game window
		DestroyWindow(m_Hwnd);

	m_pAudio->BeginFrame(); //notify audio player that frame has begun
	// Get key/control states
	m_pKeyboard->GetState();
	if (controllerOwner > -1) gamepad.getState();

	// Check for debugging keypresses
	if (m_pKeyboard->TriggerDown(controlMap["DEBUG_SHOW_FPS"])) displayFramerate = displayFramerate ? false : true;

	m_pStepTimer->Tick([&]() {
		// Code executed at every update (tick)
		if (activeScene) activeScene->update();
	});

	// Render a frame of animation
	RenderFrame();
} //ProcessFrame

/// Release all of the DirectX12 objects by deleting the renderer.
void CGame::Release() {
	delete m_pRenderer;
	m_pRenderer = nullptr; //for safety
} //Release

void CGame::Exit() {
	willExit = true;
}

void CGame::startScene(Scene* scene) {
	delete activeScene;
	activeScene = scene;
	activeScene->create();
}

/// The renderer is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.
void CGame::RenderFrame() {
	m_pRenderer->BeginFrame();

	if (activeScene) activeScene->render();
	else printf("No scene :(\n");

	if (displayFramerate) {
		string s = to_string(m_pStepTimer->GetFramesPerSecond()) + " fps";
		const Vector2 pos(m_nWinWidth - 128.0f, 30.0f);
		m_pRenderer->DrawScreenText(s.c_str(), pos, Colors::LightGreen);
	}

	m_pRenderer->EndFrame();

} //RenderFrame
