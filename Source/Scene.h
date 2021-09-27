#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Common.h"
#include "GameDefines.h"
#include "Renderer.h"
#include "GameObject.h"
#include "HUDObject.h"


// Forward declarations of classes that will exist
// (We can't just include it b/c of circular dependency
class CGame;

class Scene :
	public CCommon,
	public CComponent
{
public:
	Scene(CGame* game);
	virtual ~Scene();
	virtual void create();
	virtual void update();
	virtual void render();
	GameObject* addObject(GameObject* obj);// Adds an object to the list
	HUDObject* addObject(HUDObject* obj);// Adds a special hud object to the hud list
	void deleteObject(GameObject* object);// Remove an object from the scene, mainly to allow the user to destroy a misplaced tower
	void deleteObject(HUDObject* object);// Remove an object from the scene, mainly to allow the user to destroy a misplaced tower
	void adjustHUD(); // Re-position and re-scale all hud elements for the camera
	// Delete all objects flagged to be culled (often safer to do all at once at end of step, rather than delete 1 by 1)
	virtual void cullObjects();
protected:
	CGame* game; // Reference to game which created this scene
	std::vector<GameObject*> objectList; // Game objects managed by this scene
	vector<HUDObject*> hud; // HUD objects that scale to screen
	GameObject* backgroundSprite = nullptr;
};

