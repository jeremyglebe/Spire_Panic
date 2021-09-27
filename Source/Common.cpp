/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// This file contains declarations and initial values
/// for CCommon's static member variables.

#include "Common.h"

CRenderer* CCommon::m_pRenderer = nullptr;
GamepadWrapper CCommon::gamepad;

// Default controls
map<string, WPARAM> CCommon::controlMap = {
	// Default movement and menu navigation controls
	{"PLAYER1_UP", 'W'},
	{"PLAYER1_DOWN", 'S'},
	{"PLAYER1_LEFT", 'A'},
	{"PLAYER1_RIGHT", 'D'},
	// Default aiming and directional activation keys
	{"PLAYER1_AIM_NORTH", VK_UP},
	{"PLAYER1_AIM_SOUTH", VK_DOWN},
	{"PLAYER1_AIM_WEST", VK_LEFT},
	{"PLAYER1_AIM_EAST", VK_RIGHT},
	// Default accept and special action keys
	{"PLAYER1_ACCEPT", 'E'},
	{"PLAYER1_BACK", VK_ESCAPE},
	{"PLAYER1_MENU", 'Q'},
	{"PLAYER1_UPGRADE", 'C'},
	{"PLAYER1_DESTROY" , 'X'},
	// Player 2 move keys
	{"PLAYER2_UP", 'T'},
	{"PLAYER2_DOWN", 'G'},
	{"PLAYER2_LEFT", 'F'},
	{"PLAYER2_RIGHT", 'H'},
	// Player 2 aiming keys
	{"PLAYER2_AIM_NORTH", 'I'},
	{"PLAYER2_AIM_SOUTH", 'K'},
	{"PLAYER2_AIM_WEST", 'J'},
	{"PLAYER2_AIM_EAST", 'L'},
	// Player 2 accept key
	{"PLAYER2_ACCEPT", 'Y'},
	{"PLAYER2_BACK", 'U'},
	{"PLAYER2_MENU", 'R'},
	{"PLAYER2_UPGRADE" , 'N'},
	{"PLAYER2_DESTROY" , 'B'},
	// Debugging keys
	{"DEBUG_ZOOM", 'Z'},
	{"DEBUG_SHOW_CLSN", VK_F4},
	{"DEBUG_SHOW_FPS", VK_F4}
};

int CCommon::controllerOwner = -1;

map<WPARAM, string> CCommon::keyboardKeys = {
	{'A',"A"}, {'B',"B"}, {'C',"C"}, {'D',"D"}, {'E',"E"}, {'F',"F"}, {'G',"G"}, {'H',"H"}, {'I',"I"},
	{'J',"J"}, {'K',"K"}, {'L',"L"}, {'M',"M"}, {'N',"N"}, {'O',"O"}, {'P',"P"}, {'Q',"Q"}, {'R',"R"},
	{'S',"S"}, {'T',"T"}, {'U',"U"}, {'V',"V"}, {'W',"W"}, {'X',"X"}, {'Y',"Y"}, {'Z',"Z"}, {'0',"0"},
	{'1',"1"}, {'2',"2"}, {'3',"3"}, {'4',"4"}, {'5',"5"}, {'6',"6"}, {'7',"7"}, {'8',"8"}, {'9',"9"},
	{VK_F1,"F1"}, {VK_F2,"F2"}, {VK_F3,"F3"}, {VK_F4,"F4"}, {VK_F5,"F5"}, {VK_F6,"F6"}, {VK_F7,"F7"},
	{VK_F8,"F8"}, {VK_F9,"F9"}, {VK_F10,"F10"}, {VK_F11,"F11"}, {VK_F12,"F12"},
	{VK_UP,"UP ARROW"}, {VK_DOWN,"DOWN ARROW"}, {VK_LEFT,"LEFT ARROW"}, {VK_RIGHT,"RIGHT ARROW"},
	{VK_RETURN, "ENTER/RETURN"}, {VK_ESCAPE, "ESCAPE"}
};