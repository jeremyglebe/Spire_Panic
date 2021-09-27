#include "Scene.h"
#include "Random.h"

// Constructor for the Scene object
// This is not necessarily where we need to create objects, but instead where
// necessary configuration should occur. (Such as including a reference to the
// game object)
Scene::Scene(CGame* game) {
	this->game = game;
}

// Destructor, prepares for deletion by deleting variables
Scene::~Scene() {
	// Delete every element of the object list
	for (GameObject*& obj : objectList) {
		delete obj;
	}
	// Clear out all items in the object list
	objectList.clear();
	// Delete every element of the hud list
	for (auto obj : hud) {
		delete obj;
	}
	// Clear out all items in the hud list
	hud.clear();
}

// Method where objects, tiles, etc should be created
void Scene::create() {
	backgroundSprite = addObject(new GameObject(this, BLACK_SQUARE, 640, 360, 10));
	backgroundSprite->m_fXScale = 1280;
	backgroundSprite->m_fYScale = 720;
}

// Method that runs every tick, most logic should go here. You may also call
// the update method of game objects.
void Scene::update() {
	size_t sz = objectList.size();
	for (int i = 0; i < sz; i++) {
		objectList[i]->update();
	}
}

// Method to render the scene. Typically you'll just want to render all
// objects in the list.
void Scene::render() {
	// Render list so that all the objects are depth sorted
	vector<CSpriteDesc3D*> renders;

	// Add every game object managed by this scene to render list
	for (GameObject*& obj : objectList) {
		// Object needs to be treated as a sprite descriptor
		CSpriteDesc3D* objAsSprite = obj;
		// Draw the object
		renders.push_back(objAsSprite);
	}

	// Add every hud object to render list
	for (auto obj : hud) {
		// Object needs to be treated as a sprite descriptor
		CSpriteDesc3D* objAsSprite = obj;
		// Draw the object
		renders.push_back(objAsSprite);
	}

	// Depth sort the render list
	stable_sort(renders.begin(), renders.end(), [](CSpriteDesc3D* a, CSpriteDesc3D* b) {
		return a->m_vPos.z > b->m_vPos.z;
	});

	// Draw all the objects now that they've been depth sorted
	for (auto spr : renders) {
		m_pRenderer->Draw(*spr);
	}
}

// Adds an object to the list of objects, and returns the object in case we
// still need a reference to it.
// Example: GameObject* duck = scene.addObject(new Duck(this));
// In this case, we would have a reference to the game object AND it would be
// in the scene's list, all in one line.
GameObject* Scene::addObject(GameObject* obj) {
	objectList.push_back(obj);
	return obj;
}

HUDObject* Scene::addObject(HUDObject* obj) {
	hud.push_back(obj);
	return obj;
}

// param object is the object to be deleted. Removes object from objectlist and frees memory
void Scene::deleteObject(GameObject* object) {
	for (std::vector<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); it++) {
		if (*it == object) {
			objectList.erase(it);
			break;
		}
	}
	delete object;
}

void Scene::deleteObject(HUDObject* object) {
	for (std::vector<HUDObject*>::iterator it = hud.begin(); it != hud.end(); it++) {
		if (*it == object) {
			hud.erase(it);
			break;
		}
	}
	delete object;
}

void Scene::adjustHUD() {
	for (auto obj : hud) {
		obj->adjust();
	}
}

void Scene::cullObjects() {
	size_t sz = objectList.size();
	for (int i = 0; i < sz; i++) {
		if (objectList[i]->getWillCull()) {
			// This deletes the object and removes it's reference from objectList
			deleteObject(objectList[i]);
			// Adjust size since vector resized during processing
			sz = objectList.size();
		}
	}
}
