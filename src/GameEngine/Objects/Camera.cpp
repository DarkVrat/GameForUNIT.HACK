#include "Camera.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Managers/ConfigManager.h"
#include "../../Managers/ResourceManager.h"
#include "../../Control/MouseControl.h"

std::shared_ptr<Entity> Camera::m_Object;
glm::vec4 Camera::m_MaxCoordsCamera;
glm::vec2 Camera::m_followingRectangle;
glm::vec2 Camera::m_followingRectangleFromJSON;
glm::vec2 Camera::m_minMaxSize;
glm::vec2 Camera::m_MaxCoordsMap;
float Camera::m_stepSize; 
float Camera::m_BaseSpeed;
float Camera::m_DistanceSpeed;
glm::vec2 Camera::m_coordsCamera=glm::vec2(0,0);
glm::vec2 Camera::m_size;

void Camera::setCoords(const glm::vec2& newCoords){
	m_coordsCamera = glm::clamp(newCoords, glm::vec2(m_MaxCoordsCamera.x, m_MaxCoordsCamera.y), glm::vec2(m_MaxCoordsCamera.z, m_MaxCoordsCamera.w));
	RESOURCE_MANAGER::setView(glm::vec4(m_coordsCamera.x - m_size.x / 2, m_coordsCamera.y - m_size.y / 2, 0, 0));
}

glm::vec2 Camera::getCoords(){
	return m_coordsCamera;
}

void Camera::setSize(const float& width) {
	if (width < m_minMaxSize.x - 1.f) {
		setSize(m_minMaxSize.x);
		return;
	}
	if (width > m_minMaxSize.y + 1.f) {
		setSize(m_minMaxSize.y);
		return;
	}

	glm::vec2 sizeWindow = CONFIG_MANAGER::getWindowSize();

	m_size.x = width;
	m_size.y = m_size.x * sizeWindow.y / sizeWindow.x;
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_size.x), 0.f, static_cast<float>(m_size.y), -100.f, 100.f);

	m_followingRectangle.x = m_followingRectangleFromJSON.x * m_size.x / 2 ;
	m_followingRectangle.y = m_followingRectangleFromJSON.y * m_size.y / 2;

	RESOURCE_MANAGER::setProjection(projectionMatrix, true); 
	RESOURCE_MANAGER::setView(glm::vec4(m_coordsCamera.x - m_size.x / 2, m_coordsCamera.y - m_size.y / 2, 0, 0));

	m_MaxCoordsCamera.x = 0.f + m_size.x / 2.f;
	m_MaxCoordsCamera.y = 0.f + m_size.y / 2.f;
	m_MaxCoordsCamera.z = m_MaxCoordsMap.x - m_size.x / 2.f;
	m_MaxCoordsCamera.w = m_MaxCoordsMap.y - m_size.y / 2.f;

}

glm::vec2 Camera::getSize(){
	return m_size;
}

glm::vec2 Camera::getGazeDirection(){
	return glm::normalize(MOUSE::getPosition()*m_size+glm::vec2(m_coordsCamera.x - m_size.x / 2, m_coordsCamera.y - m_size.y / 2) - m_Object->getPosition());
}



void Camera::update(const double& deltaTime){
	glm::vec2 posObject = m_Object->getPosition();
	float distanceToTargetX = std::abs(posObject.x - m_coordsCamera.x);
	float distanceToTargetY = std::abs(posObject.y - m_coordsCamera.y);
	bool flag = false;

	if (distanceToTargetX > m_followingRectangle.x) {
		float interpolationFactorX = m_BaseSpeed * deltaTime + m_DistanceSpeed * distanceToTargetX / m_followingRectangle.x * deltaTime;
		m_coordsCamera.x += (posObject.x - m_coordsCamera.x) * interpolationFactorX;
		m_coordsCamera.x = glm::clamp(m_coordsCamera.x, m_MaxCoordsCamera.x, m_MaxCoordsCamera.z);
		flag = true;
	}
	if (distanceToTargetY > m_followingRectangle.y) {
		float interpolationFactorY = m_BaseSpeed * deltaTime + m_DistanceSpeed * distanceToTargetY / m_followingRectangle.y * deltaTime;
		m_coordsCamera.y += (posObject.y - m_coordsCamera.y) * interpolationFactorY;
		m_coordsCamera.y = glm::clamp(m_coordsCamera.y, m_MaxCoordsCamera.y, m_MaxCoordsCamera.w);
		flag = true;
	}

	if (flag) {
		RESOURCE_MANAGER::setView(glm::vec4(m_coordsCamera.x - m_size.x / 2, m_coordsCamera.y - m_size.y / 2, 0, 0));
	}
}   

void Camera::updateSize(){
	setSize(m_size.x);
}

void Camera::setSettings(const glm::vec2& RectangleFollowing, const float& baseSpeed, const float& distanceSpeed){
	m_followingRectangleFromJSON = RectangleFollowing;
	m_BaseSpeed = baseSpeed;
	m_DistanceSpeed = distanceSpeed;
}

void Camera::setMinMax(const glm::vec2& minMaxSize){
	m_minMaxSize = minMaxSize;
}

void Camera::setStepSize(const float& step){
	m_stepSize = step;
}

void Camera::setMaxCoordsMap(const glm::vec2& maxCoordsMap){
	m_MaxCoordsMap = maxCoordsMap;
}

void Camera::sizeIncrement(){
	setSize(m_size.x + m_stepSize);
}

void Camera::sizeDecrement(){
	setSize(m_size.x - m_stepSize);
}

void Camera::setEntity(std::shared_ptr<Entity> obj){
	m_Object = obj;
}
