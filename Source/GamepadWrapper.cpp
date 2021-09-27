#include "ABORT.h"
#include "GamepadWrapper.h"
#include "Controller.h"

GamepadWrapper::GamepadWrapper() {
	// Range button keycodes
	rangeKeys = {
		// Sticks as 4-directional buttons
		"LSTICK_UP",
		"LSTICK_DOWN",
		"LSTICK_LEFT",
		"LSTICK_RIGHT",
		"RSTICK_UP",
		"RSTICK_DOWN",
		"RSTICK_LEFT",
		"RSTICK_RIGHT",
		// Triggers
		"BUTTON_LT",
		"BUTTON_RT"
	};
	// Toggle button keycodes
	toggleKeys = {
		// Main action buttons
		"BUTTON_A",
		"BUTTON_B",
		"BUTTON_X",
		"BUTTON_Y",
		// Shoulder buttons
		"BUTTON_LB",
		"BUTTON_RB"
	};
	// Set the starting status of each of the range keys
	for (auto code : rangeKeys) {
		down[code] = false;
		debounced[code] = false;
	}
	// Set the starting status of each of the toggle keys
	for (auto code : toggleKeys) {
		down[code] = false;
	}
}

bool GamepadWrapper::connected() { return m_pController->IsConnected(); }

void GamepadWrapper::getState() {
	// Handle debouncing for range buttons
	for (auto code : rangeKeys) {
		if (down[code] && !debounced[code])
			debounced[code] = true;
		else if (!down[code] && debounced[code])
			debounced[code] = false;
	}

	// Get the current state of the controller object we're wrapping
	m_pController->GetState();

	// Range buttons
	Vector2 LThumb = m_pController->GetLThumb();
	Vector2 RThumb = m_pController->GetRThumb();
	down["LSTICK_UP"] = LThumb.y > 0.1f;
	down["LSTICK_DOWN"] = LThumb.y < -0.1f;
	down["LSTICK_LEFT"] = LThumb.x < -0.1f;
	down["LSTICK_RIGHT"] = LThumb.x > 0.1f;
	down["RSTICK_UP"] = RThumb.y > 0.1f;
	down["RSTICK_DOWN"] = RThumb.y < -0.1f;
	down["RSTICK_LEFT"] = RThumb.x < -0.1f;
	down["RSTICK_RIGHT"] = RThumb.x > 0.1f;
	down["BUTTON_LT"] = m_pController->GetLTrigger() > 0.1f;
	down["BUTTON_RT"] = m_pController->GetLTrigger() > 0.1f;

	// Toggle buttons
	down["BUTTON_A"] = m_pController->GetButtonAToggle();
	down["BUTTON_B"] = m_pController->GetButtonBToggle();
	down["BUTTON_X"] = m_pController->GetButtonXToggle();
	down["BUTTON_Y"] = m_pController->GetButtonYToggle();
	down["BUTTON_LB"] = m_pController->GetButtonLSToggle();
	down["BUTTON_RB"] = m_pController->GetButtonRSToggle();
}

bool GamepadWrapper::isHeld(std::string keycode) {
	// isHeld can't be used with toggle buttons, kill it!
	if (toggleKeys.count(keycode)) {
		std::string error = "Sorry, toggle buttons like ["
			+ keycode
			+ "] can't be used with isHeld()!\n"
			+ "(There's no way to see if they're held down, as they're "
			+ "automatically debounced)\n:(\n";
		ABORT(error.c_str());
	}
	return down[keycode];
}

bool GamepadWrapper::isPressed(std::string keycode) {
	// Toggle buttons just return whether they're marked as down
	// (because they're automatically debounced)
	if (toggleKeys.count(keycode)) {
		return down[keycode];
	}
	// Other keys check for debouncing
	return down[keycode] && !debounced[keycode];
}

bool GamepadWrapper::isReleased(std::string keycode) {
	// isReleased can't be used with toggle buttons, kill it!
	if (toggleKeys.count(keycode)) {
		std::string error = "Sorry, toggle buttons like ["
			+ keycode
			+ "] can't be used with isReleased()!\n"
			+ "(There's no way to check when they're released)\n:(\n";
		ABORT(error.c_str());
	}
	return !down[keycode] && debounced[keycode];
}