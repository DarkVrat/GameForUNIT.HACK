#include "CircleCollision.h"

#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include "RectangleCollision.h"

namespace PhysicsAndLogic {
    bool CircleCollision::checkCollision(std::shared_ptr<ICollosion> obj) {

        if (std::shared_ptr<RectangleCollision> rectengle = std::dynamic_pointer_cast<RectangleCollision>(obj)) {
            return rectengle->checkCollision(shared_from_this());
        }

        if (std::shared_ptr<CircleCollision> circle = std::dynamic_pointer_cast<CircleCollision>(obj)) {
            float distance = glm::distance(m_position, circle->m_position);
            return m_radius + circle->m_radius > distance;
        }

        return false;
    }

    glm::vec2 CircleCollision::extrusion(std::shared_ptr<ICollosion> obj){

        if (std::shared_ptr<RectangleCollision> rectengle = std::dynamic_pointer_cast<RectangleCollision>(obj)) {
            return -rectengle->extrusion(shared_from_this());
        }

        if (std::shared_ptr<CircleCollision> circle = std::dynamic_pointer_cast<CircleCollision>(obj)) {
            float distance = glm::distance(m_position, circle->m_position);
            float sumRadius = m_radius + circle->m_radius;
            float differens = sumRadius - distance;
            glm::vec2 direction = glm::normalize(circle->m_position - m_position);
            return direction * differens;
        }

        return glm::vec2(0,0); 
    }

    void CircleCollision::setRadius(const float& radius){
        m_radius = radius;
        updateBorder();
    }

    void CircleCollision::setPosition(const glm::vec2& position){
        m_position = position;
        updateBorder();
    }

    void CircleCollision::updateBorder(){
        m_border = glm::vec4(m_position.x - m_radius,
                             m_position.y - m_radius,
                             m_position.x + m_radius,
                             m_position.y + m_radius);
    }

    std::shared_ptr<ICollosion> CircleCollision::copy(){
        return std::make_shared<CircleCollision>(m_position, m_radius, m_movable);
    }
}