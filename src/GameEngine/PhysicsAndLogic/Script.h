#pragma once

#include "ICollision.h"
#include "../Objects/Entity.h"
#include <functional>

class Script {
public:
	Script(std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::string& callback, const bool& onlyMainEntity, const bool& disposable);

	std::shared_ptr<PhysicsAndLogic::ICollosion> getCollision();
	bool getOnlyMainEntity();
	bool getDisposable();
	void run(std::shared_ptr<Entity> entity);
private:
	std::shared_ptr<PhysicsAndLogic::ICollosion> m_collision;
	std::function<void(std::shared_ptr<Entity>)> m_callback;
	bool m_onlyMainEntity;
	bool m_disposable;
};