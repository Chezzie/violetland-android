#ifndef HUD_H_
#define HUD_H_

#include <sstream>
#include <iomanip>

#include <boost/format.hpp>

#include "../system/graphic/VideoManager.h"
#include "GameState.h"
#include "Resources.h"
#include "lifeforms/Player.h"

namespace violetland {
class HUD {
public:
	enum AREA {
		AREA_WASD,
		AREA_SHOOT,
		AREA_CHAR,
		AREA_RELOAD,
		AREA_GRENADE
	};

private:
	// References to resources and managers
	VideoManager* m_videoManager;
	Resources* m_resources;
	StaticObject* m_wasd;
	StaticObject* m_shoot;
	StaticObject* m_char;
	StaticObject* m_reload;
	StaticObject* m_grenade;
	// Object to store images of temporary bonuses
	std::map<PlayerBonusType, StaticObject*> m_bonusImg;
	// Object to store images of permanent indicators
	std::vector<StaticObject*> m_inventoryImg;
	// Console messages
	std::vector<TextObject*> m_messages;
	// HUD effects timer
	int m_bounce;

	// Apply HUD effects
	void applyEffects(float health, int levelPoints);
	// Drawing of the messages console
	void drawMessages();
	// Draw the health bar
	void drawHealth(float health, int bottomBasePoint);
	// Draw remaining times of bonuses
	void drawInventory(Player* player);
	// Draw the experience bar
	void drawExperience(float experience, int levelPoints, int bottomBasePoint);
	// Draw the ammo counter (not implemented)
	void drawAmmo(int ammo);
	// Draw the grenades counter (not implemented)
	void drawGrenades(int grenades);
	// Draw the timer
	void drawTime(GameState* gameState);
	// Draw a bar (rectangle)
	void drawBar(int x, int y, int width, int height, float value,
			GLfloat* bcolor, GLfloat* fcolor1, GLfloat* fcolor2);
	// Game over message, scores and name of player
	void drawEndGameScreen(GameState* gameState, int xp);

public:
	HUD(VideoManager* videoManager, Resources* resources);

	// Draw the whole HUD
	void draw(GameState* gameState, Player* player);
	void getButtonArea(AREA area, SDL_Rect* screenRect);
	bool isPointInRect(AREA area, const SDL_Point& pt, SDL_Rect* screenRect);

	// Add a message to console
	void addMessage(std::string message);

	// Setting the effects for HUD
	void setHealthBounce(bool value);
	void setLevelUpBounce(bool value);

	// Custom info string
	std::string Info;

	// Clear the messages console
	void reset();
	~HUD();
};
}

#endif /* HUD_H_ */
