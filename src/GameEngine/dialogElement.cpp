#include "dialogElement.h"

dialogElement::dialogElement(std::vector<std::shared_ptr<staticSprite>> sprites, std::vector<std::shared_ptr<staticAnimatedSprite>> anim, std::shared_ptr<staticTextForUI> text){
	m_sprites = sprites;
	m_text = text;
	m_anim = anim;
}

void dialogElement::render(){
	for (auto& currentSprite : m_sprites) {
		currentSprite->renderUI();
	}
	for (auto& currentAnimation : m_anim) {
		currentAnimation->renderUI();
	}
	m_text->render();
}

void dialogElement::update(const double& duration){
	for (auto& currentAnimation : m_anim) {
		currentAnimation->update(duration);
	}
}
