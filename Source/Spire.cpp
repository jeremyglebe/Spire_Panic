#include "Spire.h"
#include "Scene.h"
#include "BaseLevelScene.h"

Spire::Spire(Scene* scene, float x, float y, float z) :
	GameObject(scene, SPIRE_BASE, x, y, z) {
	hp = maxHP;
	XMFLOAT3 clsnCenter = { x - 8, y + 6, z };
	m_sAabb = BoundingBox(clsnCenter, { 48, 26, 32 });
	attackZone = BoundingBox(clsnCenter, m_sAabb.Extents * 2.0f);
	peak = scene->addObject(new GameObject(scene, SPIRE_PEAK, x, y + 116, z - .1f));
	explodeAnimation = AnimationDesc(EXPLOSION, 0, 4, 0.15f);
	healthBar = scene->addObject(new HUDObject(scene, GREENLINE, 0, 0.9f, 0));
	healthBar->relativeYScale = 10.0f;

	// Descriptor tags
	tags.insert("unpassable");
	tags.insert("spire");
}

void Spire::update()
{
	stepAnimation();
	if (hp <= 0) {
		startAnimation(explodeAnimation);
		if (!detonated) m_pAudio->play(EXPLOSION_SOUND);
		detonated = true;
		m_vPos = { m_vPos.x, m_vPos.y , -5 };
		m_fXScale = 12.0;
		m_fYScale = 12.0;
		peak->m_fAlpha = 0;
		((BaseLevelScene*)scene)->endGame();
	}
	healthBar->relativeXScale = (hp / (float)maxHP) * 350.0f;
}

void Spire::onAnimationComplete(AnimationDesc& anim) {
	if (anim == explodeAnimation) {
		m_fAlpha = 0;
	}
}
