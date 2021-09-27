#include "PlainsLevel.h"

PlainsLevel::PlainsLevel(CGame* game, eSpriteType ply1Hat, eSpriteType ply2Hat, bool coop)
	: BaseLevelScene(game, ply1Hat, ply2Hat, coop)
{
	tileMapFileName = "forgotten_plains";
	tileSetSprite = PLAINS_TILE;
}
PlainsLevel::~PlainsLevel() {}