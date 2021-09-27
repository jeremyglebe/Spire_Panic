#pragma once
#include <map>
#include <set>
#include <string>
#include <Component.h>

class GamepadWrapper :
	public CComponent
{
public:
	GamepadWrapper();
	bool connected();
	void getState();
	bool isHeld(std::string keycode);
	bool isPressed(std::string keycode);
	bool isReleased(std::string keycode);
private:
	std::set<std::string> rangeKeys;
	std::set<std::string> toggleKeys;
	std::map<std::string, bool> down;
	std::map<std::string, bool> debounced;
};
