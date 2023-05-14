#pragma once

#include <vector>
#include "../Objects/Entity.h"
#include "../Objects/Projectile.h"
#include "../GameSceneResources.h"

namespace PhysicsAndLogic {
	class PhysicsEngine {
	public:
		static void extrusionAll(const std::vector<std::shared_ptr<Projectile>>& projectiles);
	private:
		
	};
}