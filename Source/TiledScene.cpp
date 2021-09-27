#include "TiledScene.h"
#include "Game.h"

TiledScene::TiledScene(CGame* game) : Scene(game) {}

TiledScene::~TiledScene() { }

void TiledScene::render() {
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

	// Add each layer of the tilemap to render list
	for (TileLayer& layer : layers) {
		for (vector<CSpriteDesc3D>& row : layer) {
			for (CSpriteDesc3D& col : row) {
				if (m_pRenderer->BoxInFrustum(col.m_sAabb))
					renders.push_back(&col);
			}
		}
	}

	// Depth sort the render list
	stable_sort(renders.begin(), renders.end(), [](CSpriteDesc3D* a, CSpriteDesc3D* b) {
		return  a->m_vPos.y > b->m_vPos.y;
	});
	stable_sort(renders.begin(), renders.end(), [](CSpriteDesc3D* a, CSpriteDesc3D* b) {
		return  a->m_vPos.z > b->m_vPos.z;
	});

	// Draw all the objects now that they've been depth sorted
	for (auto spr : renders) {
		m_pRenderer->Draw(*spr);
	}

	// If we are set to draw tilemap collision boxes, do that now
	if (drawMapClsn) {
		for (const BoundingBox& box : clsnBoxes) {
			m_pRenderer->DrawBoundingBox(box);
		}
	}
}

void TiledScene::loadTileMap(std::string map_name, eSpriteType tileSpriteKey) {
	// Add path to front of map_name
	map_name = "Media\\Maps\\" + map_name + "\\" + map_name;
	// Load data about the tilemap
	std::ifstream inData(map_name + "_data.txt");
	// Load the data in a predetermined order
	inData >> tileSize >> tilemapWidth >> tilemapHeight;
	// Close the data file
	inData.close();

	// Load each of the main 3 display layers
	_loadTileLayer(map_name + "_bg.txt", tileSpriteKey, 1.35f);
	_loadTileLayer(map_name + "_bg2.txt", tileSpriteKey, 1.30f);
	_loadTileLayer(map_name + "_fg.txt", tileSpriteKey, 1.2f);
	_loadTileLayer(map_name + "_oh.txt", tileSpriteKey, 1.1f);
	// Load the collision layer
	_loadClsnLayer(map_name + "_collision.txt");
	// Load the path layer
	_loadPathLayer(map_name + "_paths.txt");
}

void TiledScene::_loadTileLayer(std::string filename, eSpriteType tileSpriteKey, float depth) {
	// Get the index of the layers vector we will be pushing a layer to
	size_t i = layers.size();
	// Push a new layer
	layers.push_back(TileLayer());
	// Open the correct file
	std::ifstream in(filename);

	// For the number of rows
	for (int r = 0; r < tilemapHeight; r++) {
		// Add a new row to the map
		layers[i].push_back(std::vector<CSpriteDesc3D>());
		// For the number of columns
		for (int c = 0; c < tilemapWidth; c++) {
			int tileID;
			in >> tileID;
			if (tileID >= 0) {
				CSpriteDesc3D desc;
				desc.m_nSpriteIndex = tileSpriteKey;
				desc.m_nCurrentFrame = tileID;
				// Larc engine uses a mathematical axis, so +y is upwards
				// and lower Z values render on top
				desc.m_vPos = Vector3((c * tileSize) + .5f * tileSize, (r * tileSize * -1) + (tilemapHeight * tileSize) - .5f * tileSize, depth);
				BoundingBox b;
				Vector3 p = desc.m_vPos;
				b.Center = { p.x, p.y, p.z };
				b.Extents = { .5f * tileSize, .5f * tileSize, depth / 2 };
				desc.m_sAabb = b;
				layers[i][r].push_back(desc);
			}
		}
	}
	in.close();
}

void TiledScene::_loadClsnLayer(std::string filename) {
	// Points to help with positioning tiles
	float top = (float)tileSize * tilemapHeight;
	// Create the collisions for the map
	int numClsnBoxes = 0;
	std::ifstream inClsn(filename);
	inClsn >> numClsnBoxes; // Get the number of collision boxes to read
	for (int i = 0; i < numClsnBoxes; i++) {
		float x, y, w, h;
		// Read in a collision box in order of coordinates then size
		inClsn >> x >> y >> w >> h;
		// Collision box object
		BoundingBox clsnBox;
		// Determine coordinates for aabb
		float x0 = x;
		float x1 = x0 + w;
		// Y coordinates from the map are top->bottom, larc axis is bottom->top
		// So we have to convert this.
		float y0 = top - y;
		float y1 = y0 - h;
		// Create the box from coordinates
		BoundingBox::CreateFromPoints(clsnBox, { x0, y0 }, { x1, y1 });
		// Add the collision box to the list
		clsnBoxes.push_back(clsnBox);
	}
}

void TiledScene::_loadPathLayer(std::string filename) {
	// Points to help with positioning tiles
	float top = (float)tileSize * tilemapHeight;
	// Create the collisions for the map
	int numPathPoints = 0;
	std::ifstream inPath(filename);
	inPath >> numPathPoints; // Get the number of collision boxes to read
	for (int i = 0; i < numPathPoints; i++) {
		// Read in a collision box in order of coordinates then size
		PathPoint p;
		inPath >> p.name >> p.x >> p.y >> p.next;
		p.y = top - p.y;
		if (p.name == "spawn")
			spawnPoints.push_back(p);
		else
			paths[p.name] = p;
	}
	inPath.close();
}