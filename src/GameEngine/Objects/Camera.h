#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include "Entity.h"
#define CAMERA Camera

class Camera {
public:
	static void setCoords(const glm::vec2& newCoords);
	static glm::vec2 getCoords();
	static void setSize(const float& width);
	static glm::vec2 getSize();

	static glm::vec2 getGazeDirection();

	static void update(const double& deltaTime);
	static void updateSize();
	static void setSettings(const glm::vec2& RectangleFollowing, const float& baseSpeed, const float& distanceSpeed);
	static void setMinMax(const glm::vec2& minMaxSize);
	static void setStepSize(const float& step);
	static void setMaxCoordsMap(const glm::vec2& maxCoordsMap);

	static void sizeIncrement();
	static void sizeDecrement();

	static void setEntity(std::shared_ptr<Entity> obj);
private:
	static std::shared_ptr<Entity> m_Object;
	static glm::vec4 m_MaxCoordsCamera;
	static glm::vec2 m_followingRectangleFromJSON;
	static glm::vec2 m_followingRectangle;
	static glm::vec2 m_minMaxSize;
	static glm::vec2 m_MaxCoordsMap;
	static float m_stepSize;
	static float m_BaseSpeed;
	static float m_DistanceSpeed;

	static glm::vec2 m_coordsCamera;
	static glm::vec2 m_size;
};