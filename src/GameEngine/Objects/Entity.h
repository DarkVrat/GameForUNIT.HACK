#pragma once

#include "IObject.h"
#include "../PhysicsAndLogic/ICollision.h"
#include "../../Renderer/Sprite.h"
#include "../../GameClass/Animation/StateAnimationController.h"
#include "../../Audio/SoundDevice.h"
#include "ProjectileManager.h"
#include "Item.h"
#include "AIController.h"

class ProjectileManager;
class Projectile;

class Entity :public IObject {
public:
	Entity(const glm::vec2& position, const glm::vec2& size, const float& speed, const float& healt, const float& mana, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const int& IDProjectile);

	void render();
	void update(const double& duration);
	std::shared_ptr<Entity> copy();

	std::shared_ptr<PhysicsAndLogic::ICollosion> getCollision();
	bool checkCollision(std::shared_ptr<Entity> obj) { return m_collision->checkCollision(obj->getCollision()); }
	glm::vec2 extrusion(std::shared_ptr<Entity> obj) { return m_collision->extrusion(obj->getCollision()); }

	void setPosition(const glm::vec2& value) override { 
		m_coords = value; 
		m_collision->setPosition(value);
	}

	void move(const glm::vec2& value) {
		m_coords += value;
		m_collision->setPosition(m_coords);
	}

	void damage(const float& value) {m_stats.at("healt") -= value; }
	float getHealt() { return m_stats.at("healt"); }
	float getStat(const std::string& stat) { return m_stats.at(stat); }

	void addStat(const std::string& stat, const float& value);

	void setDirectionMove(const glm::vec2& direction);

	std::shared_ptr<ProjectileManager> getManagerProjectile();
	std::shared_ptr<Projectile> createProjectile(const glm::vec2& direction);

	void addItem(std::shared_ptr<Item> item);

	void setAI(std::shared_ptr<AIController> AI) { 
		m_AIController = AI; 
		m_state = std::make_shared<StateAnimationController>("Enemy_Idle_left");
	}
	std::shared_ptr<AIController> getAI() { return m_AIController; }
private:
	glm::vec2 m_size;
	bool m_right;
	glm::vec2 m_moveDirection;
	
	std::map<std::string, float> m_stats;

	std::vector<std::shared_ptr<Item>> m_Items;

	std::shared_ptr<ProjectileManager> m_manager;
	std::shared_ptr<StateAnimationController> m_state;
	std::shared_ptr<PhysicsAndLogic::ICollosion> m_collision;
	std::shared_ptr<AIController> m_AIController;
};