#include "GameObject.h"

#include <glad/glad.h>
#include "../Audio/SoundEffectsLibrary.h"
#include "../Managers/ResourceManager.h"
#include "../Renderer/PrintText.h"

 //(RUS) �������� �������� ������� �� ����� �����������
//(ENG) Creating a game object with all parameters
GameObject::GameObject(const std::string& startState, const glm::vec3& position, const glm::vec2& size, const float& rotation){
	m_stateControll = std::make_shared<StateAnimationController>(startState);
	m_position=position;
	m_size=size;
	m_rotation=rotation;

	std::shared_ptr<Audio::SoundEffectsPlayer> player = MAKE_SOUND_PLAYER("sword");
	player->setVec3Param(AL_POSITION, position);
	m_mapPlayer.emplace("Attack1", player);
}

 //(RUS) �������� �������� �������
//(ENG) game object removal
GameObject::~GameObject(){
	m_mapPlayer.clear();
}

 //(RUS) ���������� �������� �������
//(ENG) game object update
void GameObject::update(const double& duration){
	m_stateControll->update(duration);
}

 //(RUS) ��������� �������� �������
//(ENG) game object rendering
void GameObject::render(){
	m_stateControll->render(m_position, m_size, m_rotation);
}

 //(RUS) ������� �����, ������ ��������, � ��������������� �����
//(ENG) attack event, start animation, and play sound
void GameObject::attack() {
	m_stateControll->setState("Attack1");
	m_mapPlayer["Attack1"]->play();
	idle();
}

 //(RUS) ������� �����������
//(ENG) idle event
void GameObject::idle(){
	m_stateControll->setState("Idle");
}

 //(RUS) ������� � �������
//(ENG) getters and setters
void GameObject::setPosition(const glm::vec3& position) { m_position = position; }
void GameObject::setSize(const glm::vec2& size) { m_size = size; }
void GameObject::setRotation(const float& rotation) { m_rotation = rotation; }
glm::vec3 GameObject::getPosition() { return m_position; }
glm::vec2 GameObject::getSize() { return m_size; }
float GameObject::getRotation() { return m_rotation; }
