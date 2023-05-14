#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <rapidjson/document.h>
#include "../../UI/IElementUI.h"
#include "../../Audio/SoundEffectsPlayer.h"
#include "../../Renderer/RenderEngine.h"
#include "../../Managers/ConfigManager.h"
#include "staticSprite.h"
#include "staticTextForUI.h"
#include "staticAnimatedSprite.h"

class Menu {
public:
	void update(const double& duration);
	void updateButton();
	void render();  
	void events();   

	void init(const std::string& gameState);
	void clear();
	void stopSound();
private:
	std::shared_ptr<Audio::SoundEffectsPlayer> m_backgroundMusic;
	std::vector<std::shared_ptr<staticTextForUI>> m_textVector;
	std::vector<std::shared_ptr<staticAnimatedSprite>> m_animationVector;
	std::vector<std::shared_ptr<staticSprite>> m_staticSpriteVector;
	std::vector<std::shared_ptr<UserInterface::IElementUI>> m_UIElementVector;
};