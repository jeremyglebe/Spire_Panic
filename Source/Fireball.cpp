#include "Fireball.h"

Fireball::Fireball(Scene* scene, float x, float y, float z, float level) :
	GameObject(scene, FIREBALL, x, y, z)
{
	tags.insert("projectile");
	tags.insert("explosive");
	m_sAabb = BoundingBox({ x,y,z }, { 5,5,5 });
	explodeAnimation = AnimationDesc(EXPLOSION, 0, 4, 0.15f);
	lvl = level;
	if (lvl == 0) {
		damage = 4;
		splash = 3;
	}
	if (lvl == 1) {
		damage = 6;
		splash = 6;
		radius = 30;
	}
	if (lvl == 2) {
		damage = 10;
		splash = 10;
		radius = 40;
	}
}

void Fireball::update() {
	stepAnimation();
	if (!detonated) {
		applyForces();
		updateForces();
	}
}

void Fireball::kill() {
	startAnimation(explodeAnimation);
	if (!detonated)m_pAudio->play(FIREBALL_SOUND);
	// Size of the explosion
	if (lvl == 0) {
		m_fXScale = 2.0;
		m_fYScale = 2.0;
	}
	if (lvl == 1) {
		m_fXScale = 3.0;
		m_fYScale = 3.0;
	}
	if (lvl == 2) {
		m_fXScale = 4.0;
		m_fYScale = 4.0;
	}

	// Set damage to nothing after initial hit
	detonated = true;
	damage = 0;
	splash = 0;
}

void Fireball::onAnimationComplete(AnimationDesc& anim) {
	if (anim == explodeAnimation) willCull = true;
}

void Fireball::setScale(float x, float y) {
	xScale = x;
	yScale = y;
}