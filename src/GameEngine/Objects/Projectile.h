#pragma once

#include "IObject.h"
#include "../PhysicsAndLogic/ICollision.h"
#include "../../GameClass/Animation/StateAnimationController.h"
#include "../../Managers/SoundManager.h"
#include "Entity.h"
#include <glm/glm.hpp>

class Entity;

class Projectile :public IObject {
public:
	Projectile(const glm::vec2& position, const glm::vec2& size, const float& speed, const float& distance, const float& damage, const bool& DPS, const float& cooldown, const float& priceMana, const std::string& state, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::string nameSound);

	void render();
	void update(const double& duration);
	std::shared_ptr<Projectile> copy();

	std::shared_ptr<PhysicsAndLogic::ICollosion> getCollision();

	void setPosition(const glm::vec2& value) override {
		m_coords = value;
		m_collision->setPosition(value);
	}

	float getCooldown() { return m_cooldown; }

	float getDistance() { return m_distanceMove; }
	float getDamage() { return m_damage; }
	float getSpeed() { return m_moveSpeed; }
	float getPrice() { return m_priceMana; }
	void setDistance(const float& value) { m_distanceMove = value; }
	void setDamage(const float& value) { m_damage = value; }
	void setSpeed(const float& value) { m_moveSpeed = value; }

	bool getEndWork() { return m_endWork; }
	void setEndWork(const bool& flag) { m_endWork = flag; }

	void setAttackEntity(std::shared_ptr<Entity> entity) { m_AttackEntity = entity; }
	void setIgnoreEntity(std::shared_ptr<Entity> entity);

	float getSize() { return glm::distance(glm::vec2(0, 0), m_size); }

	void setDirectionMove(const glm::vec2& direction);
private:
	glm::vec2 m_size;

	glm::vec2 m_moveDirection;
	float m_moveSpeed;
	float m_distanceMove;
	float m_cooldown;
	float m_damage;
	float m_rotateDegrees;
	float m_priceMana;
	bool m_damagePerSeconds;

	bool m_endWork;

	std::shared_ptr<StateAnimationController> m_state;
	std::shared_ptr<PhysicsAndLogic::ICollosion> m_collision;
	std::string m_NameSound;
	std::shared_ptr<Audio::SoundEffectsPlayer> m_sound;
	std::shared_ptr<Entity> m_IgnoreEntity;
	std::shared_ptr<Entity> m_AttackEntity;
};