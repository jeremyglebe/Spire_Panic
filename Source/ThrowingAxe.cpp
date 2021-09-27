#include "ThrowingAxe.h"

ThrowingAxe::ThrowingAxe(Scene* scene, float x, float y, float z) :
	GameObject(scene, THROWING_AXE, x, y, z)
{
	tags.insert("projectile");
	m_sAabb = BoundingBox({ x,y,z }, { 5,5,5 });
	damage = 20;
	m_fXScale = 2.0f;
	m_fYScale = 2.0f;
	m_sAabb.Extents = { 16.0f, 16.0f, 16.0f };
	m_pAudio->play(AXE_SOUND);
}

void ThrowingAxe::update() {
	m_fRoll += 0.1f;
	GameObject::update();
}