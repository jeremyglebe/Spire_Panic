/// \file Common.h
/// \brief Interface for the class CCommon.

#pragma once

#include <map>
#include <string>
#include "SndList.h"
#include "ParticleEngine.h"
#include "GamepadWrapper.h"

//forward declarations to make the compiler less stroppy
class CRenderer;

/// \brief The common variables class.
///
/// CCommon is a singleton class that encapsulates 
/// things that are common to different game components,
/// including game state variables.
/// Making it a singleton class means
/// that we can avoid passing its member variables
/// around as parameters, which makes the code
/// minisculely faster, and more importantly, reduces
/// function clutter.
class CCommon {
protected:
	static CRenderer* m_pRenderer; ///< Pointer to the renderer.
	// Controller wrapper
	static GamepadWrapper gamepad;
	// Game controls, name->keycode map
	static map<string, WPARAM> controlMap;
	// All keyboard keys that can be used in game
	static map<WPARAM, string> keyboardKeys;
	// Who is using the controller, -1=Disconnected, 0=No one, 1=Player 1, 2=Player 2
	static int controllerOwner;
}; //CCommon

// Common structures
struct PathPoint {
	float x = 0;
	float y = 0;
	string next = "null";
	string name = "";
};