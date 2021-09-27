#include "GameObject.h"

GameObject::GameObject() {}

GameObject::GameObject(Scene* scene, eSpriteType spriteKey, float x, float y, float z) {
	this->scene = scene;
	this->m_vPos = Vector3(x, y, z);
	creationTime = m_pStepTimer->GetTotalSeconds();
	m_nSpriteIndex = spriteKey;
	activeAnimation = AnimationDesc(spriteKey, 0, 0, 0.5);
	m_pParticleEngine = new CParticleEngine3D(); // each object has its own particle engine
}
GameObject::~GameObject() {
	delete m_pParticleEngine;

}

void GameObject::update() {
	stepAnimation();
	//move();
	applyForces();
	updateForces();
}

float GameObject::timeExisted() {
	return m_pStepTimer->GetTotalSeconds() - creationTime;
}

// Object is within a defined range of this
bool GameObject::objInRange(GameObject* other, float range) {
	return inRadius(other->m_vPos, range);
}

void GameObject::unmove() {
	// Store the current position
	Vector2 nowPos = { m_vPos.x, m_vPos.y };
	// Move back to the old position
	m_vPos.x = oldPosition.x;
	m_vPos.y = oldPosition.y;
	// Set the new "old" position (which is actually where we started this frame)
	oldPosition = nowPos;
	// Update the bounding box location
	m_sAabb.Center.x = m_vPos.x;
	m_sAabb.Center.y = m_vPos.y;
}

bool GameObject::inRadius(Vector3 pos, float radius) {		//returns true if object is within dist of pos
	float dx = m_vPos.x - pos.x;
	float dy = m_vPos.y - pos.y;
	float dx2 = dx * dx;
	float dy2 = dy * dy;
	float distance = sqrt(dx2 + dy2);
	distance = abs(distance);
	return distance <= radius;
}

void GameObject::explode() {
	//TODO: smoke effect is not appearing for some reason
	const Vector3 pos = this->getPosition();
	printf("Smoke effect should play?\n");
	CParticleDesc3D dExplosion;
	dExplosion.m_nSpriteIndex = SMOKE;
	dExplosion.m_vPos = pos;
	dExplosion.m_fFadeOutFrac = 0.4f;
	dExplosion.m_fScaleOutFrac = 0.5f;
	dExplosion.m_fLifeSpan = 2.0f;
	dExplosion.m_fMaxScale = 0.5f;
	m_pParticleEngine->create(dExplosion);
}

void GameObject::kill() {
	willCull = true;
}

bool GameObject::getWillCull() {
	return willCull;
}

void GameObject::setRoll(float angle) {
	m_fRoll = angle;
}

float GameObject::getRoll() {
	return m_fRoll;
}
Vector2 GameObject::getViewVect() {
	return Vector2(sinf(m_fRoll), -cosf(m_fRoll));
}

const Vector3& GameObject::getPosition() {
	return m_vPos;
}

const Vector2& GameObject::getOldPosition() {
	return oldPosition;
}

void GameObject::setPosition(const Vector3& pos) {
	m_vPos = pos;
}

int GameObject::getSpriteType() {
	return m_nSpriteIndex;
}

bool GameObject::is(string tag) {
	return tags.count(tag);
}

void GameObject::startAnimation(AnimationDesc& anim, bool loop) {
	if (!isPlayingAnimation(anim)) {
		activeAnimation = anim;
		isAnimating = true;
		loopAnimation = loop;
		m_nSpriteIndex = activeAnimation.sprite;
		m_nCurrentFrame = activeAnimation.startFrame;
		animationTimer = m_pStepTimer->GetTotalSeconds() + activeAnimation.frameTime;
	}
}

void GameObject::stepAnimation() {
	if (isAnimating) {
		const float t = m_pStepTimer->GetTotalSeconds();
		if (t > animationTimer) {
			m_nCurrentFrame++;
			// If we've reached the end of the animation
			if (m_nCurrentFrame > activeAnimation.endFrame) {
				// Run the completion callback
				onAnimationComplete(activeAnimation);
				// If we are looping, update set the frame back to the start
				if (loopAnimation) {
					m_nCurrentFrame = activeAnimation.startFrame;
				}
				// If we are not looping, set frame to the end and stop animating
				else {
					m_nCurrentFrame = activeAnimation.endFrame;
					stopAnimation();
				}
			}
			// Update when the next frame change should occur
			animationTimer = t + activeAnimation.frameTime;
		}
	}
}

void GameObject::stopAnimation() {
	isAnimating = false;
	loopAnimation = false;
}

void GameObject::onAnimationComplete(AnimationDesc& anim) {
	return;
}

bool GameObject::isPlayingAnimation(AnimationDesc& anim) {
	return isAnimating && activeAnimation == anim;
}

// Pseudo physics stuff
void GameObject::applyForces() {
	// Update the tracker of object's previous position
	oldPosition = { m_vPos.x, m_vPos.y };
	// Get the time elapsed since last frame
	float t = m_pStepTimer->GetElapsedSeconds();
	// Get the force over time
	Vector2 transform = getNetForce() * t;
	// Safeguard, hopefully this never runs. Prevents mega-forces that shouldn't exist anyway
	if (transform.Length() > 128.0f)
		return;
	// Move the object
	m_vPos.x += transform.x;
	m_vPos.y += transform.y;
	// Update the bounding box location
	m_sAabb.Center.x = m_vPos.x;
	m_sAabb.Center.y = m_vPos.y;
}
void GameObject::updateForces() {
	// Delete any forces which have expired
	forces.erase(std::remove_if(forces.begin(), forces.end(),
		[](auto& force) { return force.isDead(); }), forces.end());
}
Vector2 GameObject::getNetForce() {
	Vector2 net = { 0, 0 };
	for (auto force : forces) {
		net += force.get();
	}
	return net;
}
void GameObject::addForce(PseudoForce force) {
	forces.push_back(force);
}
void GameObject::addForce(Vector2 initial, float decay) {
	forces.push_back(PseudoForce(initial, decay));
}
void GameObject::addForce(float magnitude, float angleInDegrees, float decay) {
	// Convert the angle to radians for mathematics
	float angleInRadians = angleInDegrees * (MY_PI / 180);
	// X component and Y component calculations
	float xComponent = magnitude * cosf(angleInRadians);
	float yComponent = magnitude * sinf(angleInRadians);
	// Add the force
	forces.push_back(PseudoForce({ xComponent, yComponent }, decay));
}
void GameObject::pushBack() {
	Vector2 oldp = getOldPosition();
	Vector2 newp = Vector2(m_vPos.x, m_vPos.y);
	// Get the direction the player was traveling
	Vector2 motion = oldp - newp;
	motion *= 2.0f;
	//motion *= getNetForce();
	// Move the player back to their old position
	unmove();
	addForce(motion, 0.3f);
	applyForces();
}
void GameObject::absoluteStop() {
	forces.clear();
	pushBack();
}