#pragma once

#include <vector>
#include <memory>
#include "../GameClass/UIGame/staticSprite.h"
#include "../GameClass/UIGame/staticAnimatedSprite.h"
#include "../GameClass/UIGame/staticTextForUI.h"

class dialogElement {
public:
	dialogElement(std::vector<std::shared_ptr<staticSprite>> sprites, std::vector<std::shared_ptr<staticAnimatedSprite>> anim, std::shared_ptr<staticTextForUI> text);

	void render();
	void update(const double& duration);
private:
	std::vector<std::shared_ptr<staticSprite>> m_sprites;
	std::vector<std::shared_ptr<staticAnimatedSprite>> m_anim;
	std::shared_ptr<staticTextForUI> m_text;
};