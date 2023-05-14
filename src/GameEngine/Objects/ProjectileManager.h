#pragma once

#include <memory>
#include <vector>
#include <map>
#include "Projectile.h"

class Projectile;

class ProjectileManager {
public:
	static void addProjectileTipe(const int& ID, std::shared_ptr<Projectile> NewProjectile);
	static void clear();

	ProjectileManager() :m_IDNow(-1), m_cooldown(0) {}
	int getType() { return m_IDNow; }
	float getDistance();
	void setType(const int& ID);
	void update(const double& duration);
	void addTypesProjectile(const int& ID);
	std::shared_ptr<Projectile> createProjectile(const float& cooldownMultiply);
private:
	static std::map<int, std::shared_ptr<Projectile>> m_mapTypesProjectile;
	
	std::map<int, float> m_TypesAvailable;
	float m_cooldown;
	int m_IDNow;
};