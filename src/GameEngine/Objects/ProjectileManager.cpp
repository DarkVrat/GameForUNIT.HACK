#include "ProjectileManager.h"

std::map<int, std::shared_ptr<Projectile>> ProjectileManager::m_mapTypesProjectile;

void ProjectileManager::addProjectileTipe(const int& ID, std::shared_ptr<Projectile> NewProjectile){
	m_mapTypesProjectile.emplace(ID, NewProjectile);
}

void ProjectileManager::clear(){
	m_mapTypesProjectile.clear();
}

float ProjectileManager::getDistance(){
	return m_mapTypesProjectile.at(m_IDNow)->getDistance();
}

void ProjectileManager::setType(const int& ID){
	if (m_TypesAvailable.find(ID) != m_TypesAvailable.end()) {
		m_IDNow = ID;
	}
}

void ProjectileManager::update(const double& duration){
	m_cooldown -= duration;
}

void ProjectileManager::addTypesProjectile(const int& ID){
	m_TypesAvailable.emplace(ID, m_mapTypesProjectile.at(ID)->getCooldown());
}

std::shared_ptr<Projectile> ProjectileManager::createProjectile(const float& cooldownMultiply){
	if (m_cooldown > 0.f||m_TypesAvailable.empty()) return nullptr;

	std::shared_ptr<Projectile> ret = m_mapTypesProjectile.at(m_IDNow)->copy();
	m_cooldown = m_TypesAvailable.at(m_IDNow) * cooldownMultiply;
	return ret;
}
