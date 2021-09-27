/// \file Renderer.h 
/// \brief Interface for the renderer class CRenderer.

/*
	Object Depth Reference
	
	When you're setting depth for objects in other classes, it is important to keep
	in mind what our game standardly expects for depths, as well as the general scale
	and everything of depths between objects. Here is an ordered list.

	-5
		-5.0 -> HUD Objects
	1
		1.1 -> Overhead Tile Layer
			1.15 -> Player & most objects
		1.2 -> Foreground Tile Layer
	    1.3 -> Background Tiles Layer
			1.30 -> Background 2
			1.31 -> Background 1
	10
		10.0 -> Black Background
	(Lower depth means closer to the camera)
*/

#pragma once

#include <map>
#include "GameDefines.h"
#include "SpriteRenderer.h"

/// \brief The renderer.
///
/// CRenderer handles the game-specific rendering tasks, relying on
/// the base class to do all of the actual API-specific rendering.
class CRenderer : public CSpriteRenderer {
public:
	CRenderer(); ///< Constructor.

	void DrawBoundingBox(const BoundingBox& aabb);
	void LoadImages(); ///< Load images.
	Vector2 getCenter();

	void loadNewFont(string fontTag);
	void setFont(string fontTag);
private:
	vector<unique_ptr<SpriteFont>> fonts;
	map<string, int> fontsRef;
	int font_id = 0;
}; //CRenderer