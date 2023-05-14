#pragma once

#include "../PhysicsAndLogic/ICollision.h"

class Collider {
public:
	Collider(std::shared_ptr<PhysicsAndLogic::ICollosion> collision);

	std::shared_ptr<PhysicsAndLogic::ICollosion> getCollision();
private:
	std::shared_ptr<PhysicsAndLogic::ICollosion> m_collision;
};