#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace PhysicsAndLogic {
	class ICollosion {
	public:
		virtual bool checkCollision(std::shared_ptr<ICollosion> obj) = 0;
		virtual glm::vec2 extrusion(std::shared_ptr<ICollosion> obj) = 0;

		virtual void setPosition(const glm::vec2& position) = 0;
		virtual glm::vec2 getPosition() = 0;

		virtual void updateBorder() = 0;
		virtual glm::vec4 getBorder() = 0;

		virtual void setMovable(const bool& flag) = 0;
		virtual bool getMovable() = 0;

		virtual std::shared_ptr<ICollosion> copy() = 0;
	protected:
		glm::vec2 m_position;
		bool m_movable;
		glm::vec4 m_border;
	};
}