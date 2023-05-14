#include "RectangleCollision.h"

#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include "CircleCollision.h"

namespace PhysicsAndLogic {
    bool RectangleCollision::checkCollision(std::shared_ptr<ICollosion> obj){

        if (std::shared_ptr<RectangleCollision> rectengle = std::dynamic_pointer_cast<RectangleCollision>(obj)) {
            glm::vec4 objBorder = rectengle->getBorder();

            if (objBorder.z < m_border.x || objBorder.x > m_border.z) return false;
            if (objBorder.w < m_border.y || objBorder.y > m_border.w) return false;

            return true;
        }

        if (std::shared_ptr<CircleCollision> circle = std::dynamic_pointer_cast<CircleCollision>(obj)) {
            glm::vec2 objPosition = circle->getPosition();
            float objRadius = circle->getRadius();
            glm::vec4 objBorder = circle->getBorder();
            

            if (objBorder.z < m_border.x || objBorder.x > m_border.z) return false;
            if (objBorder.w < m_border.y || objBorder.y > m_border.w) return false;

            if (objPosition.x > m_border.x - 0.0001f && objPosition.x < m_border.z + 0.0001f) return true;
            if (objPosition.y > m_border.y - 0.0001f && objPosition.y < m_border.w + 0.0001f) return true;

            glm::vec2 point;
            point.x = (std::abs(objPosition.x - m_border.x) < std::abs(objPosition.x - m_border.z)) ? m_border.x : m_border.z;
            point.y = (std::abs(objPosition.y - m_border.y) < std::abs(objPosition.y - m_border.w)) ? m_border.y : m_border.w;
            float Distance = glm::distance(point, objPosition);
            if (Distance < objRadius) return true;

            return false;
        }

        return false;
    }

    glm::vec2 RectangleCollision::extrusion(std::shared_ptr<ICollosion> obj) {
        if (std::shared_ptr<RectangleCollision> rectengle = std::dynamic_pointer_cast<RectangleCollision>(obj)) {
            glm::vec4 objBorder = rectengle->getBorder();
            glm::vec2 vecMove(0, 0);

            vecMove.x = m_border.z - objBorder.x;
            float temp = m_border.x - objBorder.z;
            if (std::abs(temp) < vecMove.x) { vecMove.x = temp; }

            vecMove.y = m_border.y - objBorder.w;
            temp = m_border.w - objBorder.y;
            if (temp < std::abs(vecMove.y)) { vecMove.y = temp; }

            if (std::abs(vecMove.y) > std::abs(vecMove.x)) { vecMove.y = 0; }
            else { vecMove.x = 0; }

            return vecMove;
        }

        if (std::shared_ptr<CircleCollision> circle = std::dynamic_pointer_cast<CircleCollision>(obj)) {
            glm::vec2 objPosition = circle->getPosition();
            float objRadius = circle->getRadius();
            

            if ((objPosition.x > m_border.x - 0.0001f && objPosition.x < m_border.z + 0.0001f) || (objPosition.y > m_border.y - 0.0001f && objPosition.y < m_border.w + 0.0001f)) {
                glm::vec4 objBorder = circle->getBorder();
                glm::vec2 vecMove(0, 0);

                vecMove.x = m_border.z - objBorder.x;
                float temp = m_border.x - objBorder.z;
                if (std::abs(temp) < vecMove.x) { vecMove.x = temp; }

                vecMove.y = m_border.y - objBorder.w;
                temp = m_border.w - objBorder.y;
                if (temp < std::abs(vecMove.y)) { vecMove.y = temp; }

                if (std::abs(vecMove.y) > std::abs(vecMove.x)) { vecMove.y = 0; }
                else { vecMove.x = 0; }

                return vecMove;
            }
            
            glm::vec2 point;
            point.x = (std::abs(objPosition.x - m_border.x) < std::abs(objPosition.x - m_border.z)) ? m_border.x : m_border.z;
            point.y = (std::abs(objPosition.y - m_border.y) < std::abs(objPosition.y - m_border.w)) ? m_border.y : m_border.w;
            float Distance = glm::distance(point, objPosition);
            float differens = objRadius - Distance;
            glm::vec2 direction = glm::normalize(objPosition - point);
            return direction * differens;
        }

        return glm::vec2();
    }

    void RectangleCollision::setSize(const glm::vec2& size){
        m_size = size;
        updateBorder();
    }

    void RectangleCollision::setPosition(const glm::vec2& position){
        m_position = position;
        updateBorder();
    }

    void RectangleCollision::updateBorder(){
        m_border = glm::vec4(m_position.x - m_size.x / 2.0f,
                             m_position.y - m_size.y / 2.0f,
                             m_position.x + m_size.x / 2.0f,
                             m_position.y + m_size.y / 2.0f);
    }

    std::shared_ptr<ICollosion> RectangleCollision::copy() {
        return std::make_shared<RectangleCollision>(m_position, m_size, m_movable);
    }
}
