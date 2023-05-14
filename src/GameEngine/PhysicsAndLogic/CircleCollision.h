#pragma once

#include "ICollision.h"
#include <glm/vec2.hpp>


namespace PhysicsAndLogic {
	class CircleCollision :public ICollosion, public std::enable_shared_from_this<CircleCollision> {
	public:
		CircleCollision(const glm::vec2& position, const float& radius, const bool& movable=false) :m_radius(radius) { 
			m_position = position; 
			m_movable = movable;
			updateBorder();
		};

		bool checkCollision(std::shared_ptr<ICollosion> obj) override;
		glm::vec2 extrusion(std::shared_ptr<ICollosion> obj) override;

		void setRadius(const float& radius);
		float getRadius() { return m_radius; }

		void setPosition(const glm::vec2& position) override;
		glm::vec2 getPosition() override { return m_position; };

		void updateBorder() override;
		glm::vec4 getBorder() override { return m_border; };

		void setMovable(const bool& flag) override { m_movable = flag; }
		bool getMovable() override { return m_movable; }

		std::shared_ptr<ICollosion> copy() override;
	private:
		float m_radius;
	};
}