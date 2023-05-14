#include "Collider.h"

Collider::Collider(std::shared_ptr<PhysicsAndLogic::ICollosion> collision){
	m_collision = collision;
}

std::shared_ptr<PhysicsAndLogic::ICollosion> Collider::getCollision(){
	return m_collision;
}
