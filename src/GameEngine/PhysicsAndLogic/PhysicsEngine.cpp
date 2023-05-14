#include "PhysicsEngine.h"
#include "../GameSceneResources.h"

namespace PhysicsAndLogic {
	void PhysicsEngine::extrusionAll(const std::vector<std::shared_ptr<Projectile>>& projectiles) {
		std::shared_ptr<Entity> main = GameSceneResources::getMainCharecter();
		auto& Colliders = GameSceneResources::getColliders();
		auto& Scripts = GameSceneResources::getScripts();
		auto& Items = GameSceneResources::getItems();
		auto& Entityes = GameSceneResources::getEntity();

		for (int i = 0; i < Entityes.size(); i++) {
			if (main->checkCollision(Entityes.at(i))) {
				glm::vec2 vec = main->extrusion(Entityes.at(i));
				if (Entityes.at(i)->getCollision()->getMovable()) {
					main->move(-glm::vec2(vec.x / 2, vec.y / 2));
					Entityes.at(i)->move(glm::vec2(vec.x / 2, vec.y / 2));
				}
				else {
					main->move(-vec);
				}
			}

			if (Entityes.at(i)->getAI()->getCollision()->checkCollision(main->getCollision())) {
				Entityes.at(i)->getAI()->setTarget(main->getPosition());
				Entityes.at(i)->getAI()->setTargetMain(true);
			}
			else {
				if (Entityes.at(i)->getAI()->getTargetMain()) {
					Entityes.at(i)->getAI()->setAttackMain(false);
					Entityes.at(i)->getAI()->setTargetMain(false);
					Entityes.at(i)->getAI()->returnToPatrol();
				}
			}

			for (int j = i + 1; j < Entityes.size(); j++) {
				if (Entityes.at(j)->checkCollision(Entityes.at(i))) {
					glm::vec2 vec = Entityes.at(j)->extrusion(Entityes.at(i));
					if (Entityes.at(i)->getCollision()->getMovable()) {
						Entityes.at(j)->move(-glm::vec2(vec.x / 2, vec.y / 2));
						Entityes.at(i)->move(glm::vec2(vec.x / 2, vec.y / 2));
					}
					else {
						Entityes.at(j)->move(-vec);
					}
				}
			}
		}

		for (auto& currentCollider : Colliders) {
			if (main->getCollision()->checkCollision(currentCollider->getCollision())) {
				main->move(-main->getCollision()->extrusion(currentCollider->getCollision()));
			}

			for (int i = 0; i < Entityes.size(); i++) {
				if (Entityes.at(i)->getCollision()->checkCollision(currentCollider->getCollision())) {
					Entityes.at(i)->move(-Entityes.at(i)->getCollision()->extrusion(currentCollider->getCollision()));
				}
			}
		}

		for (int i = Scripts.size()-1; i >= 0; i--) {
			if (main->getCollision()->checkCollision(Scripts.at(i)->getCollision())) {
				Scripts.at(i)->run(main);
				if (Scripts.at(i)->getDisposable()) {
					Scripts.erase(Scripts.begin() + i); 
					continue;
				}
			}

			if (Scripts.at(i)->getOnlyMainEntity()) continue;
			for (int j = 0; j < Entityes.size(); j++) {
				if (Entityes.at(j)->getCollision()->checkCollision(Scripts.at(i)->getCollision())) {
					Scripts.at(i)->run(Entityes.at(j));
					if (Scripts.at(i)->getDisposable()) {
						Scripts.erase(Scripts.begin() + i);
						break;
					}
				}
			}
		}

		for (auto& currentProjectile : projectiles) {
			for (auto& currentCollider : Colliders) {
				if (currentCollider->getCollision()->checkCollision(currentProjectile->getCollision())) {
					if (glm::distance(glm::vec2(0, 0), currentCollider->getCollision()->extrusion(currentProjectile->getCollision())) > currentProjectile->getSize() * 0.3f) {
						currentProjectile->setEndWork(true);
					}
				}
			}
			for (auto& currentEntity : Entityes) {
				if (currentProjectile->getCollision()->checkCollision(currentEntity->getCollision())) {
					currentProjectile->setAttackEntity(currentEntity);
				}
			}
			if (currentProjectile->getCollision()->checkCollision(main->getCollision())) {
				currentProjectile->setAttackEntity(main);
			}
		}

		for (int i = Items.size() - 1; i >= 0; i--) {
			if (main->getCollision()->checkCollision(Items.at(i)->getCollision())) {
				main->addItem(Items.at(i));
				Items.erase(Items.begin() + i);
				continue;
			}

			for (int j = 0; j < Entityes.size(); j++) {
				if (Entityes.at(j)->getAI()->getCollision()->checkCollision(Items.at(i)->getCollision())) {
					Entityes.at(j)->getAI()->setTarget(Items.at(i)->getPos());
					Entityes.at(j)->getAI()->setTargetMain(false);
				}
				if (Entityes.at(j)->getCollision()->checkCollision(Items.at(i)->getCollision())) {
					Entityes.at(j)->addItem(Items.at(i));
					Items.erase(Items.begin() + i);
					break;
				}
			}

		}

		for (auto& current : GameSceneResources::getInterObjects()) {
			if (current->getCompleteJob()) continue;
			if (current->getCollision()->checkCollision(main->getCollision())) {
				current->setReady(true);
				break;
			}
			if (current->getReady()) {
				current->setReady(false);
			}
		}
	}
}
 