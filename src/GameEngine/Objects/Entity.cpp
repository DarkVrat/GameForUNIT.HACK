#include "Entity.h"

#include <glm/glm.hpp>

Entity::Entity(const glm::vec2& position, const glm::vec2& size, const float& speed, const float& healt, const float& mana, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const int& IDProjectile){
	m_coords = position;
	m_size = size;
	m_collision = collision;
	m_right = false;

	m_stats.emplace("healt", healt);    
	m_stats.emplace("maxHealt", healt);
	m_stats.emplace("regenHealt", 2.f);
	m_stats.emplace("regenHealt_Multiply", 1.f);

	m_stats.emplace("mana", mana);
	m_stats.emplace("maxMana", mana);
	m_stats.emplace("regenMana", 2.f);
	m_stats.emplace("regenMana_Multiply", 1.f);

	m_stats.emplace("moveSpeed", speed);
	m_stats.emplace("moveSpeed_Multiply", 1.f);

	m_stats.emplace("Particle_speedMultiply", 1.f);
	m_stats.emplace("Particle_distanceMultiply", 1.f);
	m_stats.emplace("Particle_cooldownMultiply", 1.f);
	m_stats.emplace("Particle_damageMultiply", 1.f);  

	m_AIController = nullptr;
	m_state = std::make_shared<StateAnimationController>("Main_Idle_left");  
	m_manager = std::make_shared<ProjectileManager>();
	m_manager->addTypesProjectile(IDProjectile);
	m_manager->setType(IDProjectile);
	m_moveDirection = glm::vec2(0, 0);
}

void Entity::render(){
	m_state->render(glm::vec3(m_coords, 0), m_size, 0);
}

void Entity::update(const double& duration) { 
	m_manager->update(duration);
	m_state->update(duration);

	if (m_AIController != nullptr) {
		if (m_AIController->getTargetMain()) {
			if (m_AIController->getChicken() && m_stats.at("healt") / m_stats.at("maxHealt") < 0.1f) {
				m_AIController->setAttackMain(true);
				m_coords += glm::vec2(m_moveDirection.x * -1, m_moveDirection.y * -1) * m_stats.at("moveSpeed") * m_stats.at("moveSpeed_Multiply") * static_cast<float>(duration);
				m_collision->setPosition(m_coords);
				m_AIController->setPosition(m_coords);
			}
			else if (glm::distance(m_coords, m_AIController->getTarget()) < m_manager->getDistance() * m_stats.at("Particle_distanceMultiply")) {
				m_AIController->setAttackMain(true);
			}
			else {
				m_AIController->setAttackMain(false);
				m_coords += m_moveDirection * m_stats.at("moveSpeed") * m_stats.at("moveSpeed_Multiply") * static_cast<float>(duration);
				m_collision->setPosition(m_coords); 
				m_AIController->setPosition(m_coords);
			}
		}
		else {
			m_coords += m_moveDirection * m_stats.at("moveSpeed") * m_stats.at("moveSpeed_Multiply") * static_cast<float>(duration);
			m_collision->setPosition(m_coords);
			if (glm::distance(m_coords, m_AIController->getTarget()) < 2.f) {
				m_AIController->update(duration);
			}
			m_AIController->setPosition(m_coords);
		}
	}
	else {
		m_coords += m_moveDirection * m_stats.at("moveSpeed") * m_stats.at("moveSpeed_Multiply") * static_cast<float>(duration);
		m_collision->setPosition(m_coords);
		SOUND_DEVICE::setPosition(glm::vec3(m_coords / 10.f, 0));
		
	
	}

	m_stats.at("healt") += m_stats.at("regenHealt") * m_stats.at("regenHealt_Multiply") * duration;
	if (m_stats.at("healt") > m_stats.at("maxHealt")) m_stats.at("healt") = m_stats.at("maxHealt");

	m_stats.at("mana") += m_stats.at("regenMana") * m_stats.at("regenMana_Multiply") * duration;
	if (m_stats.at("mana") > m_stats.at("maxMana")) m_stats.at("mana") = m_stats.at("maxMana");
}

std::shared_ptr<Entity> Entity::copy(){ 
	return std::make_shared<Entity>(m_coords, m_size, m_stats.at("moveSpeed"), m_stats.at("maxHealt"), m_stats.at("maxMana"), m_collision->copy(), m_manager->getType());
}

std::shared_ptr<PhysicsAndLogic::ICollosion> Entity::getCollision(){ 
	return m_collision;
}

void Entity::setDirectionMove(const glm::vec2& direction){
	if(m_AIController!=nullptr)
		if (glm::distance(m_coords, m_AIController->getTarget()) < 2.f) {
			m_moveDirection = glm::vec2(0, 0);
			if (m_right) {
				m_state->setState("Enemy_Idle_right");
			}
			else {
				m_state->setState("Enemy_Idle_left");
			}
			return;
		}

	if (std::abs(direction.x) < 0.1f && std::abs(direction.y) < 0.1f) {
		if (m_right) {
			m_state->setState("Main_Idle_right");
		}
		else {
			m_state->setState("Main_Idle_left");
		}
		m_moveDirection = direction;
	}
	else {
		m_moveDirection = glm::normalize(direction);
		if (m_moveDirection.x < -0.01f) { m_right = false; }
		else { m_right = true; }
		if (m_right) {
			if (m_AIController == nullptr)m_state->setState("Main_Walk_right");
			else m_state->setState("Enemy_Walk_right");
		}
		else {
			if (m_AIController == nullptr)m_state->setState("Main_Walk_left"); 
			else m_state->setState("Enemy_Walk_left");
		}
	}
}

std::shared_ptr<ProjectileManager> Entity::getManagerProjectile(){
	return m_manager;
}

std::shared_ptr<Projectile> Entity::createProjectile(const glm::vec2& direction){
	std::shared_ptr<Projectile> ret = m_manager->createProjectile(m_stats.at("Particle_cooldownMultiply"));
	if (ret) {
		if (m_stats.at("mana") < ret->getPrice()) {return nullptr;}
		m_stats.at("mana") -= ret->getPrice();
		ret->setDistance(ret->getDistance() * m_stats.at("Particle_distanceMultiply"));
		ret->setDamage(ret->getDamage() * m_stats.at("Particle_damageMultiply"));
		ret->setSpeed(ret->getSpeed() * m_stats.at("Particle_speedMultiply"));
		ret->setPosition(glm::vec2(m_coords.x, m_coords.y-5)); 
		ret->setDirectionMove(direction);

		if (direction.x > 0) {
			m_right = true;
			if (ret->getDistance() < 64.f) {
				if(m_AIController==nullptr) m_state->setState("Main_Attack_right");
				else m_state->setState("Enemy_Attack_right");
			}
			else {
				if (m_AIController == nullptr)m_state->setState("Main_Magic_right");
				else m_state->setState("Enemy_Magic_right");
			}
		}
		else {
			m_right = false; 
			if (ret->getDistance() < 64.f) {
				if (m_AIController == nullptr)m_state->setState("Main_Attack_left");
				else m_state->setState("Enemy_Attack_left");
			}
			else {
				if (m_AIController == nullptr)m_state->setState("Main_Magic_left");
				else m_state->setState("Enemy_Magic_left");
			}
		}
	}
	return ret;
}

void Entity::addStat(const std::string& stat, const float& value){
	if (stat.find("Multiply") != stat.npos) {
		m_stats.at(stat) *= value;
	}
	else {
		m_stats.at(stat) += value; 
	}
}

void Entity::addItem(std::shared_ptr<Item> item){
	m_Items.push_back(item);
	for (auto& currentEffect : item->getStats()) {
		addStat(currentEffect.first, currentEffect.second);
	}
}
 