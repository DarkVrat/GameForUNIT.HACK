#pragma once

#include "ICollision.h"
#include <glm/vec2.hpp>


namespace PhysicsAndLogic {
	class RectangleCollision:public ICollosion {
	public:
		RectangleCollision(const glm::vec2& position, const glm::vec2& size, const bool& movable=false) : m_size(size) {
			m_position = position; 
			m_movable = movable;
			updateBorder();
		};

		bool checkCollision(std::shared_ptr<ICollosion> obj) override;
		glm::vec2 extrusion(std::shared_ptr<ICollosion> obj) override;

		void setSize(const glm::vec2& size);
		glm::vec2 getSize() { return m_size; }

		void setPosition(const glm::vec2& position) override;
		glm::vec2 getPosition() override { return m_position; };

		void updateBorder() override;
		glm::vec4 getBorder() override { return m_border; };

		void setMovable(const bool& flag) override { m_movable = flag; }
		bool getMovable() override { return m_movable; }

		std::shared_ptr<ICollosion> copy() override;
	private:
		glm::vec2 m_size;
	};
}