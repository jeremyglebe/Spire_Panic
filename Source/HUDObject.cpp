#include "HUDObject.h"
#include "Renderer.h"

HUDObject::HUDObject(Scene* scene, eSpriteType spriteKey, float xPercentage, float yPercentage, float relativeZ)
{
	this->scene = scene;
	this->m_nSpriteIndex = spriteKey;
	this->xPercentage = xPercentage;
	this->yPercentage = yPercentage;
	this->relativeZ = relativeZ;
	relativeXScale = 1.0f;
	relativeYScale = 1.0f;
	adjust();
}

void HUDObject::adjust() {
	Vector3 cameraPos = m_pRenderer->GetCameraPos();
	float cameraZ = abs(cameraPos.z);

	m_vPos = {
		cameraPos.x + xPercentage * xMaxScale * cameraZ,
		cameraPos.y + yPercentage * yMaxScale * cameraZ,
		-5 + relativeZ
	};

	m_fXScale = relativeXScale * zBaseScale * cameraZ;
	m_fYScale = relativeYScale * zBaseScale * cameraZ;
}