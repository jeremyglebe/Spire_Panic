#include "DesolateDesertLevel.h"

DesolateDesertLevel::DesolateDesertLevel(CGame* game, eSpriteType ply1Hat, eSpriteType ply2Hat, bool coop)
	: BaseLevelScene(game, ply1Hat, ply2Hat, coop)
{
	tileMapFileName = "desolate_desert";
	tileSetSprite = DESERT_TILE;
}
DesolateDesertLevel::~DesolateDesertLevel() {}