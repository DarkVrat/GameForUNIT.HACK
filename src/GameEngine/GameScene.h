#pragma once

#include "Objects/Projectile.h"
#include "Objects/Entity.h"
#include <memory>
#include <string>
#include <vector>
#include "GameSceneResources.h"
#include "../Audio/SoundEffectsPlayer.h"
#include "GameUI.h"

class GameScene {
public:
	void init(const std::string& gameScene);
	void render();
	void update(const double& duration);
	void events();
	void clear();

	GameUI& getUI() { return m_UI; }
	void stopMusic() { m_backgroundMusic->stop(); }
private:
	std::shared_ptr<Audio::SoundEffectsPlayer> m_backgroundMusic;
	GameSceneResources m_Resources;
	GameUI m_UI;
	std::vector<std::shared_ptr<Projectile>> m_Projectiles;
	std::shared_ptr<Entity> m_MainCharter;
};