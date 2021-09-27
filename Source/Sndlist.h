/// \file SndList.h
/// \brief Enumerated type for sounds.

#pragma once

#include "Sound.h"

/// \brief Game sound enumerated type. 
///
/// These are the sounds used in actual gameplay. 
/// The sounds must be listed here in the same order that
/// they are in the sound settings XML file.
enum eSoundType {
	MENU_SONG,
	MAP_SONG,
	GUN_SOUND,
	CLANG_SOUND,
	COIN_GRAB,
	EXPLOSION_SOUND,
	FIREBALL_SOUND,
	LASER_SOUND,
	AXE_SOUND
}; //eSoundType