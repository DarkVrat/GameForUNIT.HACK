#include "Projectile.h"

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

Projectile::Projectile(const glm::vec2& position, const glm::vec2& size, const float& speed, const float& distance, const float& damage, const bool& DPS, const float& cooldown, const float& priceMana, const std::string& state, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::string nameSound) {
	m_coords = position;
	m_size = size;
	m_moveSpeed = speed;
	m_state = std::make_shared<StateAnimationController>(state);
	m_collision = collision;
	m_damage = damage;
	m_damagePerSeconds = DPS;
	m_distanceMove = distance;
	m_cooldown = cooldown;
	m_rotateDegrees = 0.f;
	m_priceMana = priceMana;
	m_endWork = false;
	m_AttackEntity = nullptr;
	m_IgnoreEntity = nullptr;
	m_sound = nullptr;
	m_moveDirection = glm::vec2(0, 0);
	m_NameSound = nameSound;
}

void Projectile::render() {
	m_state->render(glm::vec3(m_coords, 0.1), m_size, m_rotateDegrees);
}

void Projectile::update(const double& duration) {
	m_state->update(duration);
	if (m_AttackEntity && !m_endWork && m_AttackEntity != m_IgnoreEntity) {
		if (m_damagePerSeconds) {
			m_AttackEntity->damage(m_damage * duration);
			if (!m_collision->checkCollision(m_AttackEntity->getCollision())) {
				m_AttackEntity = nullptr;
			} 
		}
		else {  
			m_AttackEntity->damage(m_damage); 
			m_endWork = true;
		}
	}
	float distance= m_moveSpeed * static_cast<float>(duration);
	m_coords += m_moveDirection * distance;
	m_collision->setPosition(m_coords);

	if (m_sound != nullptr) {
		if (m_sound->isStopped()) {
			m_sound->play();
		}
		m_sound->setVec3Param(AL_POSITION, glm::vec3(m_coords / 10.f, 0));
	}

	m_distanceMove -= distance;
	if (m_distanceMove < 0.f) {
		m_endWork = true;
	}
}

std::shared_ptr<Projectile> Projectile::copy() {
	return std::make_shared<Projectile>(m_coords, m_size, m_moveSpeed, m_distanceMove, m_damage,m_damagePerSeconds, m_cooldown,m_priceMana, m_state->getState(), m_collision->copy(), m_NameSound);
}

std::shared_ptr<PhysicsAndLogic::ICollosion> Projectile::getCollision() {
	return m_collision;
}

void Projectile::setIgnoreEntity(std::shared_ptr<Entity> entity){
	m_IgnoreEntity = entity;
	if (m_sound == nullptr) {
		m_sound = MAKE_SOUND_PLAYER(m_NameSound);
		m_sound->setVec3Param(AL_POSITION, glm::vec3(m_coords / 10.f, 0));
		
		m_sound->play();
	}
}

void Projectile::setDirectionMove(const glm::vec2& direction) {
	if (std::abs(direction.x) < 0.1f && std::abs(direction.y) < 0.1f) {
		m_moveDirection = direction;
	}
	else {
		m_moveDirection = glm::normalize(direction);
		m_rotateDegrees = glm::degrees(glm::orientedAngle(glm::vec2(1.f, 0.f), m_moveDirection));
	}
}
