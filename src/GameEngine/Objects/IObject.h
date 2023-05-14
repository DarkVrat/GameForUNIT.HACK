#pragma once

#include <glm/vec2.hpp>
#include <memory>

class IObject {
public:
	virtual void render() = 0;
	virtual void update(const double& duration) = 0;
	
	virtual void setPosition(const glm::vec2& newCoords) {
		m_coords = newCoords;
	}
	virtual glm::vec2 getPosition(){
		return m_coords;
	}

protected:
	glm::vec2 m_coords=glm::vec2(0,0);
};