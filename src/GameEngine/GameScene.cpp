#include "GameScene.h"

#include "../Renderer/MapRender.h"
#include "../Renderer/PrintText.h"
#include "../Control/KeyboardControl.h"
#include "../Control/MouseControl.h"
#include "Objects/Camera.h"
#include "PhysicsAndLogic/CircleCollision.h"
#include "../Managers/ResourceManager.h"
#include "PhysicsAndLogic/PhysicsEngine.h"
#include "../GameClass/MainGameClass.h"
#include "../Managers/SoundManager.h"

void GameScene::init(const std::string& gameScene){
	std::string jsonPath = "res/Scenes/" + gameScene + ".json";

	clear(); 

	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(jsonPath);

	auto musicIT = JSONDoc.FindMember("soundEffect");
	if (musicIT != JSONDoc.MemberEnd()) {
		if (m_backgroundMusic != nullptr)m_backgroundMusic->stop();
		m_backgroundMusic = MAKE_SOUND_PLAYER(musicIT->value["nameSound"].GetString(), musicIT->value["nameSample"].GetString());
		m_backgroundMusic->play();
	}

	m_Resources.load(jsonPath);
	  
	m_MainCharter = GameSceneResources::getMainCharecter();
	CAMERA::setCoords(m_MainCharter->getPosition());
	CAMERA::setEntity(m_MainCharter);
	m_MainCharter->getManagerProjectile()->addTypesProjectile(0);
	m_MainCharter->getManagerProjectile()->addTypesProjectile(1);
	m_MainCharter->getManagerProjectile()->addTypesProjectile(2);
	m_MainCharter->getManagerProjectile()->addTypesProjectile(3);

	m_UI.init();
	m_UI.setMain(m_MainCharter);
	m_UI.updateType();
}   

void GameScene::render(){
	m_MainCharter->render();
	for (auto& current : GameSceneResources::getEntity()) {
		current->render(); 
	}
	for (auto& current : m_Projectiles) {
		current->render();
	}
	for (auto& curent : GameSceneResources::getItems()) {
		curent->render();
	}
	for (auto& current : GameSceneResources::getInterObjects()) {
		current->render();
	}   
	m_UI.render(); 
	MAP::render();
}

void GameScene::update(const double& duration){
	if(m_backgroundMusic!=nullptr)
		if (m_backgroundMusic->isStopped())
			m_backgroundMusic->play();

	auto& Objects = GameSceneResources::getEntity();
	for (auto& currentObject : Objects) {
		currentObject->setDirectionMove(currentObject->getAI()->getTarget() - currentObject->getPosition());
		currentObject->update(duration);
		if (currentObject->getAI()->getAttackMain()) {
			std::shared_ptr<Projectile> project = currentObject->createProjectile(glm::normalize(m_MainCharter->getPosition()-currentObject->getPosition()));
			if (project) {
				project->setIgnoreEntity(currentObject);
				m_Projectiles.push_back(project);
			}
		}
	}

	for (auto& current : GameSceneResources::getInterObjects()) {
		current->update(duration);
	}

	m_MainCharter->update(duration);
	for (auto& currentProjectile : m_Projectiles) {
		currentProjectile->update(duration);
	}
	PhysicsAndLogic::PhysicsEngine::extrusionAll(m_Projectiles);

	for (int i = m_Projectiles.size() - 1; i >= 0; i--) {
		if (m_Projectiles.at(i)->getEndWork()) {
			m_Projectiles.erase(m_Projectiles.begin() + i);
		}
	}
	for (int i = Objects.size() - 1; i >= 0; i--) {
		if (Objects.at(i)->getHealt() < 0) {
			Objects.erase(Objects.begin() + i);
		}
	}

	if (m_MainCharter->getHealt() < 0.f) {
		MainGameClass::setGame(false);
		MainGameClass::getMenu().init("Death");
	}

	m_UI.update(duration);
	CAMERA::update(duration); 
}

void GameScene::events(){
	if (KEYBOARD::ifPressed(GLFW_KEY_ESCAPE)) {
		MainGameClass::setGame(false);
		MainGameClass::getMenu().init("Pause");
	}

	if (KEYBOARD::ifPressed(GLFW_KEY_MINUS)) {
		CAMERA::sizeIncrement();
	}
	if (KEYBOARD::ifPressed(GLFW_KEY_EQUAL)) {
		CAMERA::sizeDecrement();
	}
	 
	if (MOUSE::ifClamped(GLFW_MOUSE_BUTTON_LEFT)) {
		std::shared_ptr<Projectile> project = m_MainCharter->createProjectile(CAMERA::getGazeDirection());
		if (project) {
			project->setIgnoreEntity(m_MainCharter);
			m_Projectiles.push_back(project);
		}
	} 

	glm::vec2 direct = glm::vec2(0, 0);
	if (KEYBOARD::ifClamped(GLFW_KEY_W)) {direct.y = 1;} 
	else if (KEYBOARD::ifClamped(GLFW_KEY_S)) {direct.y = -1;}
	if (KEYBOARD::ifClamped(GLFW_KEY_A)) {direct.x = -1;}
	else if (KEYBOARD::ifClamped(GLFW_KEY_D)) {direct.x = 1;}
	m_MainCharter->setDirectionMove(direct);

	if (KEYBOARD::ifPressed(GLFW_KEY_1)) { m_MainCharter->getManagerProjectile()->setType(0); m_UI.updateType(); }
	else if (KEYBOARD::ifPressed(GLFW_KEY_2)) { m_MainCharter->getManagerProjectile()->setType(1); m_UI.updateType();}
	else if (KEYBOARD::ifPressed(GLFW_KEY_3)) { m_MainCharter->getManagerProjectile()->setType(2); m_UI.updateType();}
	else if (KEYBOARD::ifPressed(GLFW_KEY_4)) { m_MainCharter->getManagerProjectile()->setType(3); m_UI.updateType();}

	if (KEYBOARD::ifPressed(GLFW_KEY_E)) {
		for (auto& current : GameSceneResources::getInterObjects()) {
			if (current->getReady()) {
				current->runScript(m_MainCharter);
				current->setCompleteJob(true);
			}
		}
	}
}

void GameScene::clear(){
	m_Projectiles.clear();
	m_MainCharter = nullptr;
}
