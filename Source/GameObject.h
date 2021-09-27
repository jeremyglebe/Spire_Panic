#pragma once
#include <unordered_set>
#include "Component.h"
#include "GameDefines.h"
#include "SpriteDesc.h"
#include "Common.h"
#include "StepTimer.h"
#include "AnimationDesc.h"
#include "PseudoForce.h"

// Forward declarations of classes that will exist
// (We can't just include it b/c of circular dependency
class Scene;

class GameObject : public CComponent, public CSpriteDesc3D, public CCommon
{
public:
	GameObject();
	// Constructor with default position
	GameObject::GameObject(Scene* scene, eSpriteType spriteKey, float x = 0.0f, float y = 0.0f, float z = 0.0f);
	virtual ~GameObject();
	virtual void update(); // Method runs every tick/update

	// Positioning related methods
	void setRoll(float angle);
	float getRoll();
	const Vector3& getPosition();
	const Vector2& getOldPosition();
	void setPosition(const Vector3& pos);
	bool inRadius(Vector3 pos, float dist);
	virtual void unmove(); // Undo the character's last movement

	// Description methods
	int getSpriteType();
	bool is(string tag); // Checks if the game object has a certain descriptor tag

	// Animation methods
	void startAnimation(AnimationDesc& anim, bool loop = false);
	void stepAnimation();
	void stopAnimation();
	bool isPlayingAnimation(AnimationDesc& anim);
	virtual void onAnimationComplete(AnimationDesc& anim);

	// Other methods
	Vector2 getViewVect();
	bool getWillCull(); //true if dead, false otherwise
	virtual void kill(); // set isdead flag to true;
	void explode(); // particle effect for fireball explosion
	bool objInRange(GameObject* other, float range); // Object is within a defined range of this
	float timeExisted(); // Returns how long the object has existed

	// Physics
	vector<PseudoForce> forces;
	virtual void applyForces();
	void updateForces();
	Vector2 getNetForce();
	void addForce(PseudoForce force);
	void addForce(Vector2 initial, float decay);
	void addForce(float magnitude, float angleInDegrees, float decay);
	void pushBack();
	void absoluteStop();
protected:
	// Scene which manages this object
	Scene* scene;
	// Stores when the object was first created
	float creationTime = 0.0f;
	// Stores the position from the previous frame
	Vector2 oldPosition;
	// Whether the object should be deleted next frame
	bool willCull = false;
	// Descriptor tags for the object
	unordered_set<string> tags;
	// Animation related variables
	AnimationDesc activeAnimation;
	float animationTimer = 0.0f;
	bool isAnimating = false;
	bool loopAnimation = false;
	CParticleEngine3D* m_pParticleEngine = nullptr; ///< Pointer to a particle engine.
};

