/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"

#include "Abort.h"

CRenderer::CRenderer() :
	CSpriteRenderer(Unbatched3D) {
	// Initial camera position, standard "true pixel" frame
	m_pCamera->MoveTo(Vector3(m_vWinCenter.x, m_vWinCenter.y, -1600));
	// Keep track of the original font
	fonts.push_back(move(m_pFont));
	fontsRef["font"] = 0;
	m_pFont = move(fonts[fontsRef["font"]]);
} //constructor

/// Load the specific images needed for this game.
/// This is where eSpriteType values from GameDefines.h get
/// tied to the names of sprite tags in gamesettings.xml. Those
/// sprite tags contain the name of the corresponding image file.
/// If the image tag or the image file are missing, then
/// the game should abort from deeper in the Engine code,
/// leaving you with a dialog box that tells you what
/// went wrong.
void CRenderer::LoadImages() {
	BeginResourceUpload();

	// Loading of images goes here
	Load(SPIRE_BASE, "spire_base");
	Load(SPIRE_PEAK, "spire_peak");
	Load(MAGE1, "mage1");
	Load(MAGE2, "mage2");
	Load(MAGE3, "mage3");
	Load(RANGE1, "range1");
	Load(RANGE2, "range2");
	Load(RANGE3, "range3");
	Load(EXPLOSION, "explosion");
	Load(SMOKE, "smoke");
	Load(DESERTLEVEL, "desertLevel");
	Load(BASELEVEL, "baseLevel");
	Load(PLAINSLEVEL, "plainsLevel");
	// Hats
	Load(VIKING_HAT, "viking_hat");
	Load(WIZARD_HAT, "wizard_hat");
	Load(CYBORG_HAT, "cyborg_hat");
	// Projectiles
	Load(FIREBALL, "fireball");
	Load(BULLET1, "bullet1");
	Load(THROWING_AXE, "throwing_axe");
	Load(REDLINE, "redline");
	// Tilesets
	Load(SIMPLE_TILE, "simple_tile");
	Load(DESERT_TILE, "desert_tile");
	Load(PLAINS_TILE, "plains_tile");
	// Currency
	Load(COIN, "coin");
	Load(COIN_STACK, "coin_stack");
	Load(COIN_BAG, "coin_bag");
	// GUI
	Load(TITLE_SCREEN, "title_screen");
	Load(MENU_BOX, "menu_box");
	Load(BLACK_SQUARE, "black_square");
	Load(GREENLINE, "greenline");
	// Characters
	Load(HUMAN_IDLE, "human_idle");
	Load(HUMAN_WALK, "human_walk");
	Load(GOBLIN_WALK, "goblin_walk");
	Load(GOBLIN_ATTACK, "goblin_attack");
	Load(MINOTAUR_WALK, "minotaur_walk");
	Load(MINOTAUR_ATTACK, "minotaur_attack");
	Load(WOLF_WALK, "wolf_walk");
	Load(WOLF_ATTACK, "wolf_attack");
	// Load any additional fonts
	loadNewFont("title_font");
	loadNewFont("selection_font");
	loadNewFont("prompt_font");
	loadNewFont("big_prompt_font");

	EndResourceUpload();

} //LoadImages

/// Draw an axially aligned bounding box using the green line sprite.
/// \param aabb An axially aligned bounding box.
void CRenderer::DrawBoundingBox(const BoundingBox& aabb) {
	const Vector3 center = aabb.Center;
	const Vector3 extent = aabb.Extents;

	CSpriteDesc3D clsnSprite;
	clsnSprite.m_nSpriteIndex = GREENLINE;
	clsnSprite.m_fAlpha = 0.5;
	clsnSprite.m_vPos = { center.x, center.y, -5 };
	clsnSprite.m_fXScale = extent.x;
	clsnSprite.m_fYScale = extent.y;
	Draw(clsnSprite);
} //DrawBoundingBox

Vector2 CRenderer::getCenter() {
	return Vector2(m_vWinCenter.x, m_vWinCenter.y);
}


// For some reason this function is only run for the first fonts, and we wanted more fonts
// So I've borrowed it from the LARC engine.
// I DID NOT WRITE THIS FUNCTION, ONLY MODIFIED IT FOR MULTIPLE FONTS
/// Load the font for the screen text from a font file specified in gamesettings.xml.
void CRenderer::loadNewFont(string fontTag) {
	XMLElement* tag = m_pXmlSettings->FirstChildElement(fontTag.c_str());

	if (tag == nullptr)return; //no tag, so bail

	const char* filename = tag->Attribute("file");
	const size_t newsize = strlen(filename) + 1;
	wchar_t* wfilename = new wchar_t[newsize]; //wide file name
	size_t n;
	mbstowcs_s(&n, wfilename, newsize, filename, _TRUNCATE); //convert file name to wide characters

	unique_ptr<SpriteFont> theFont;

	theFont = make_unique<SpriteFont>(m_pD3DDevice, *m_pResourceUpload,
		wfilename,
		m_pDescriptorHeap->GetCpuHandle(m_nNumResourceDesc),
		m_pDescriptorHeap->GetGpuHandle(m_nNumResourceDesc));

	++m_nNumResourceDesc;

	delete[] wfilename;

	fontsRef[fontTag] = (int)fonts.size();
	fonts.push_back(move(theFont));
} //LoadScreenFont

void CRenderer::setFont(string fontTag) {
	int id = fontsRef[fontTag];
	// Return the current font
	fonts[font_id] = move(m_pFont);
	// Move the new font to be used
	m_pFont = move(fonts[id]);
	// Change the font id
	font_id = id;
}