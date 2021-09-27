#pragma once
#include <string>
#include <map>
#include "Scene.h"

// Forward declarations of classes that will exist
// (We can't just include it b/c of circular dependency
class CGame;
struct PathPoint;

// Type aliases
typedef std::vector<std::vector<CSpriteDesc3D>> TileLayer;

class TiledScene :
	public Scene
{
public:
	TiledScene(CGame* game);
	virtual ~TiledScene() override;
	virtual void render() override;
	void loadTileMap(std::string map_name, eSpriteType tileSpriteKey);
	map<string, PathPoint> getPaths() { return paths; }
	vector<PathPoint> getSpawnPoints() { return spawnPoints; }
protected:
	// Data about the tilemap
	int tilemapWidth = 0; // Width of tilemap in tiles
	int tilemapHeight = 0; // Height of tilemap in tiles
	// Data about tile graphic
	eSpriteType tileset = SIMPLE_TILE; // Sprite index to use for rendering tilemap
	int tileSize = 0; // Width and height of tile in pixels (must be square)
	// Tile layers that should be displayed
	std::vector<TileLayer> layers;
	// List of collision boxes for scene (tile aabbs, usually)
	std::vector<BoundingBox> clsnBoxes;
	// List of points on the level's path
	map<string, PathPoint> paths;
	vector<PathPoint> spawnPoints;
	// Boolean determining if we draw tilemap aabbs
	bool drawMapClsn = false;
	// Functions to add a single layer of a map
	void _loadTileLayer(std::string filename, eSpriteType tileSpriteKey, float depth);
	void _loadClsnLayer(std::string filename);
	void _loadPathLayer(std::string filename);
};
