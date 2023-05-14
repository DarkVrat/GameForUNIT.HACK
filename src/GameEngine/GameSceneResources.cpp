#include "GameSceneResources.h"

#include "../Managers/ResourceManager.h"
#include "../Renderer/MapRender.h"
#include "Objects/ProjectileManager.h"
#include "Objects/Camera.h"
#include "Objects/Entity.h"
#include "Objects/AIController.h"
#include "PhysicsAndLogic/CircleCollision.h"
#include "PhysicsAndLogic/RectangleCollision.h"
#include <thread>

std::vector<std::shared_ptr<Entity>> GameSceneResources::m_Objects;
std::vector<std::shared_ptr<Collider>> GameSceneResources::m_Colliders; 
std::vector<std::shared_ptr<Script>> GameSceneResources::m_Scripts;
std::vector<std::shared_ptr<Item>> GameSceneResources::m_ItemsScene;
std::vector<std::shared_ptr<InteractiveObject>> GameSceneResources::m_InterObject;
std::map<uint32_t, std::shared_ptr<Item>> GameSceneResources::m_Items;
std::map<uint32_t, std::shared_ptr<Entity>> GameSceneResources::m_Entityes; 
std::shared_ptr<Entity> GameSceneResources::m_MainCharacter;

std::string GameSceneResources::m_pathMainCharacter;
std::string GameSceneResources::m_pathCamera;
std::string GameSceneResources::m_pathEntityes;
std::string GameSceneResources::m_pathProjectile;
std::string GameSceneResources::m_pathItem;

void GameSceneResources::load(const std::string& gameScene){
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(gameScene);
	
	loadProjectiles(JSONDoc);

	std::thread MainCharacterThread([&]() {loadMainMainCharacter(JSONDoc); });
	std::thread CollidersThread([&]() {loadColliders(JSONDoc); });
	std::thread ScriptsThread([&]() {
		loadEntityes(JSONDoc); 
		loadItems(JSONDoc); 
		loadScripts(JSONDoc); 
	});
	std::thread ObjectsThread([&]() {loadInteractiveObject(JSONDoc); });

	loadMap(JSONDoc);
	loadCameraSettings(JSONDoc);

	MainCharacterThread.join();
	CollidersThread.join(); 
	ScriptsThread.join();
	ObjectsThread.join();
}

void GameSceneResources::loadMap(const rapidjson::Document& JSONDoc){
	std::string MapPath = JSONDoc.FindMember("map")->value["mapPath"].GetString();
	float CellMapSize = JSONDoc.FindMember("map")->value["sizeCell"].GetFloat();
	MAP::setMap(MapPath, CellMapSize);
}

void GameSceneResources::loadCameraSettings(const rapidjson::Document& JSONDoc){
	std::string path = JSONDoc.FindMember("Camera")->value.GetString();
	if (m_pathCamera == path) {
		return;
	}

	m_pathCamera = path;
	rapidjson::Document JSONCamera = RESOURCE_MANAGER::loadJSONDoc(path);

	float CameraFollowingX = JSONCamera.FindMember("RectangleFollowing")->value["x"].GetFloat();
	float CameraFollowingY = JSONCamera.FindMember("RectangleFollowing")->value["x"].GetFloat();
	float baseSpeed = JSONCamera.FindMember("baseSpeed")->value.GetFloat();
	float distanceSpeed = JSONCamera.FindMember("distanceSpeed")->value.GetFloat();
	float CameraSize = JSONCamera.FindMember("sizeCamera")->value.GetFloat();
	float stepSize = JSONCamera.FindMember("stepSize")->value.GetFloat();
	float MinSize = JSONCamera.FindMember("minMaxSize")->value["min"].GetFloat();
	float MaxSize = JSONCamera.FindMember("minMaxSize")->value["max"].GetFloat();

	CAMERA::setSettings(glm::vec2(CameraFollowingX, CameraFollowingY), baseSpeed, distanceSpeed);
	CAMERA::setMinMax(glm::vec2(MinSize, MaxSize));
	CAMERA::setStepSize(stepSize);
	CAMERA::setMaxCoordsMap(MAP::getMaxCoords());
	CAMERA::setSize(CameraSize);
}

void GameSceneResources::loadMainMainCharacter(const rapidjson::Document& JSONDoc){
	std::string path = JSONDoc.FindMember("MainCharacter")->value.GetString();
	float startPosMainX = JSONDoc.FindMember("MainCharacter_startPos")->value["x"].GetFloat();
	float startPosMainY = JSONDoc.FindMember("MainCharacter_startPos")->value["y"].GetFloat();

	if (m_pathMainCharacter == path) {
		m_MainCharacter->setPosition(glm::vec2(startPosMainX, startPosMainY));
		return;
	}

	m_pathMainCharacter = path;
	rapidjson::Document JSONMainCharacter = RESOURCE_MANAGER::loadJSONDoc(path);

	int typeMain = JSONMainCharacter.FindMember("type")->value.GetInt();
	float sizeMainX = JSONMainCharacter.FindMember("size")->value["x"].GetFloat();
	float sizeMainY = JSONMainCharacter.FindMember("size")->value["y"].GetFloat();
	float speedMain = JSONMainCharacter.FindMember("speed")->value.GetFloat();
	float healt = JSONMainCharacter.FindMember("healt")->value.GetFloat();
	float mana = JSONMainCharacter.FindMember("mana")->value.GetFloat();
	int startProjectile = JSONMainCharacter.FindMember("startProjectile")->value.GetInt();

	std::shared_ptr<PhysicsAndLogic::ICollosion> collision;

	if (typeMain == 0) {
		float sizeCollisionX = JSONMainCharacter.FindMember("sizeCollision")->value["x"].GetFloat();
		float sizeCollisionY = JSONMainCharacter.FindMember("sizeCollision")->value["y"].GetFloat();
		collision = std::make_shared<PhysicsAndLogic::RectangleCollision>(glm::vec2(startPosMainX, startPosMainY), glm::vec2(sizeCollisionX, sizeCollisionY));
	}
	else if (typeMain == 1) {
		float radiusCollision = JSONMainCharacter.FindMember("radiusCollision")->value.GetFloat();
		collision = std::make_shared<PhysicsAndLogic::CircleCollision>(glm::vec2(startPosMainX, startPosMainY), radiusCollision);
	}

	m_MainCharacter = std::make_shared<Entity>(glm::vec2(startPosMainX, startPosMainY), glm::vec2(sizeMainX, sizeMainY), speedMain,healt, mana, collision, startProjectile);
	m_MainCharacter->getCollision()->setMovable(true);
}

void GameSceneResources::loadEntityes(const rapidjson::Document& JSONDoc){
	std::string path = JSONDoc.FindMember("Entity")->value.GetString();
	if (m_pathEntityes != path) {
		m_Entityes.clear();
		m_pathEntityes = path;
		rapidjson::Document JSONEntity = RESOURCE_MANAGER::loadJSONDoc(path);

		auto EnemyIt = JSONEntity.FindMember("Enemy");
		if (EnemyIt != JSONEntity.MemberEnd()) {
			for (const auto& currentEntity : EnemyIt->value.GetArray()) {
				int ID = currentEntity["ID"].GetInt();
				int type = currentEntity["type"].GetInt();
				float speed = currentEntity["speed"].GetFloat();
				float healt = currentEntity["healt"].GetFloat();
				float mana = currentEntity["mana"].GetFloat();
				int startProjectile = currentEntity["startProjectile"].GetInt();
				float sizeX = currentEntity["size"].FindMember("x")->value.GetFloat();
				float sizeY = currentEntity["size"].FindMember("y")->value.GetFloat();

				std::shared_ptr<PhysicsAndLogic::ICollosion> collision;
				std::shared_ptr<Entity> Enemy;

				if (type == 0) {
					float CollisX = currentEntity["sizeCollision"].FindMember("x")->value.GetFloat();
					float CollisY = currentEntity["sizeCollision"].FindMember("y")->value.GetFloat();

					collision = std::make_shared<PhysicsAndLogic::RectangleCollision>(glm::vec2(0, 0), glm::vec2(CollisX, CollisY));
				}
				else if (type == 1) {
					float radius = currentEntity["radiusCollision"].GetFloat();

					collision = std::make_shared<PhysicsAndLogic::CircleCollision>(glm::vec2(0, 0), radius);
				}

				Enemy = std::make_shared<Entity>(glm::vec2(0, 0), glm::vec2(sizeX, sizeY), speed, healt, mana, collision, startProjectile);
				Enemy->getCollision()->setMovable(true);
				m_Entityes.emplace(ID, Enemy);
			}
		}
	}

	m_Objects.clear();
	auto EnemySceneIt = JSONDoc.FindMember("enemyOnScene");
	if (EnemySceneIt != JSONDoc.MemberEnd()) {
		for (const auto& currentItem : EnemySceneIt->value.GetArray()) {
			int ID = currentItem["ID"].GetInt();
			float X = currentItem["position"].FindMember("x")->value.GetFloat();
			float Y = currentItem["position"].FindMember("y")->value.GetFloat();
			std::shared_ptr<Entity> enemy = m_Entityes.at(ID)->copy();
			enemy->setPosition(glm::vec2(X, Y));

			float radiusVision= currentItem["radiusVision"].GetFloat();
			bool chiken = currentItem["chiken"].GetBool();
			std::vector<std::pair<glm::vec2, float>> CoordsPatrooleAndTime;
			for (const auto& currentPosition : currentItem["patrol"].GetArray()) {
				float X = currentPosition["position"].FindMember("x")->value.GetFloat();
				float Y = currentPosition["position"].FindMember("y")->value.GetFloat();
				float time = currentPosition["time"].GetFloat();
				CoordsPatrooleAndTime.push_back(std::pair<glm::vec2, float>(glm::vec2(X, Y), time));
			}
			std::shared_ptr<PhysicsAndLogic::ICollosion> collision = std::make_shared<PhysicsAndLogic::CircleCollision>(glm::vec2(X, Y), radiusVision);
			std::shared_ptr<AIController> controller=std::make_shared<AIController>(CoordsPatrooleAndTime, collision, chiken);
			enemy->setAI(controller);

			m_Objects.push_back(enemy);
		}
	}
} 

void GameSceneResources::loadColliders(const rapidjson::Document& JSONDoc){
	m_Colliders.clear();

	auto ColliderIt = JSONDoc.FindMember("Colliders");
	if (ColliderIt != JSONDoc.MemberEnd()) {
		for (const auto& currentCollider : ColliderIt->value.GetArray()) {
			int type = currentCollider["type"].GetInt();
			std::shared_ptr<PhysicsAndLogic::ICollosion> collision;

			if (type == 0) {
				float left = currentCollider["leftBottom"].FindMember("x")->value.GetFloat();
				float bottom = currentCollider["leftBottom"].FindMember("y")->value.GetFloat();
				float right = currentCollider["rightTop"].FindMember("x")->value.GetFloat();
				float top = currentCollider["rightTop"].FindMember("y")->value.GetFloat();

				collision = std::make_shared<PhysicsAndLogic::RectangleCollision>(glm::vec2((right + left) / 2.f, (top + bottom) / 2.f), glm::vec2(right - left, top - bottom));
			}
			if (type == 1) {
				float X = currentCollider["position"].FindMember("x")->value.GetFloat();
				float Y = currentCollider["position"].FindMember("y")->value.GetFloat();
				float radius = currentCollider["radius"].GetFloat();

				collision = std::make_shared<PhysicsAndLogic::CircleCollision>(glm::vec2(X, Y), radius);
			}

			m_Colliders.push_back(std::make_shared<Collider>(collision));
		}
	}
}
 
void GameSceneResources::loadScripts(const rapidjson::Document& JSONDoc){
	m_Scripts.clear();

	auto ScriptIt = JSONDoc.FindMember("scripts");
	if (ScriptIt != JSONDoc.MemberEnd()) {
		for (const auto& currentScript : ScriptIt->value.GetArray()) {
			int type = currentScript["type"].GetInt();
			std::shared_ptr<PhysicsAndLogic::ICollosion> collision;

			if (type == 0) {
				float left = currentScript["leftBottom"].FindMember("x")->value.GetFloat();
				float bottom = currentScript["leftBottom"].FindMember("y")->value.GetFloat();
				float right = currentScript["rightTop"].FindMember("x")->value.GetFloat();
				float top = currentScript["rightTop"].FindMember("y")->value.GetFloat();

				collision = std::make_shared<PhysicsAndLogic::RectangleCollision>(glm::vec2((right + left) / 2.f, (top + bottom) / 2.f), glm::vec2(right - left, top - bottom));
			}
			if (type == 1) {
				float X = currentScript["position"].FindMember("x")->value.GetFloat();
				float Y = currentScript["position"].FindMember("y")->value.GetFloat();
				float radius = currentScript["radius"].GetFloat(); 

				collision = std::make_shared<PhysicsAndLogic::CircleCollision>(glm::vec2(X, Y), radius);
			}

			std::string callback = currentScript["callback"].GetString();
			bool onlyMainEntity = currentScript["onlyMainEntity"].GetBool(); 
			bool disposable = currentScript["disposable"].GetBool();

			m_Scripts.push_back(std::make_shared<Script>(collision, callback, onlyMainEntity, disposable));
		}
	}
}

void GameSceneResources::loadProjectiles(const rapidjson::Document& JSONDoc){
	std::string path = JSONDoc.FindMember("Projectiles")->value.GetString();
	if (m_pathProjectile == path) {
		return;
	}

	ProjectileManager::clear();
	m_pathProjectile = path;
	rapidjson::Document JSONProjectile = RESOURCE_MANAGER::loadJSONDoc(m_pathProjectile);

	auto ProjectileIt = JSONProjectile.FindMember("projectile");
	if (ProjectileIt != JSONProjectile.MemberEnd()) {
		for (const auto& currentProjectile : ProjectileIt->value.GetArray()) {
			int ID = currentProjectile["ID"].GetInt();
			int type = currentProjectile["type"].GetInt();
			float priceMana = currentProjectile["priceMana"].GetFloat();
			float speed = currentProjectile["speed"].GetFloat();
			float distance = currentProjectile["distance"].GetFloat();
			float damage = currentProjectile["damage"].GetFloat();
			bool DPS = currentProjectile["DPS"].GetBool();
			float cooldown = currentProjectile["cooldown"].GetFloat();
			std::string state = currentProjectile["stateAnim"].GetString();
			std::string nameSound = currentProjectile["nameSound"].GetString();

			std::shared_ptr<PhysicsAndLogic::ICollosion> collision;
			std::shared_ptr<Projectile> projectile;

			if (type == 0) {
				float X = currentProjectile["size"].FindMember("x")->value.GetFloat();
				float Y = currentProjectile["size"].FindMember("y")->value.GetFloat(); 

				collision = std::make_shared<PhysicsAndLogic::RectangleCollision>(glm::vec2(0, 0), glm::vec2(X, Y));
				projectile = std::make_shared<Projectile>(glm::vec2(0, 0), glm::vec2(X, Y), speed, distance, damage, DPS, cooldown, priceMana, state, collision, nameSound);
			}
			else if (type == 1) {
				float radius = currentProjectile["radius"].GetFloat();

				collision = std::make_shared<PhysicsAndLogic::CircleCollision>(glm::vec2(0, 0), radius);
				projectile = std::make_shared<Projectile>(glm::vec2(0, 0), glm::vec2(radius*2, radius * 2), speed, distance, damage, DPS, cooldown, priceMana, state, collision, nameSound);
			}

			ProjectileManager::addProjectileTipe(ID, projectile);
		}
	}
}

void GameSceneResources::loadItems(const rapidjson::Document& JSONDoc){
	std::string path = JSONDoc.FindMember("Items")->value.GetString();
	if (m_pathItem != path) {
		m_Items.clear();
		m_pathItem = path;
		rapidjson::Document JSONItem = RESOURCE_MANAGER::loadJSONDoc(m_pathItem);

		auto ItemIt = JSONItem.FindMember("items");
		if (ItemIt != JSONItem.MemberEnd()) {
			for (const auto& currentItem : ItemIt->value.GetArray()) {
				int ID = currentItem["ID"].GetInt();
				int type = currentItem["type"].GetInt();
				std::shared_ptr<Renderer::Sprite> sprite = RESOURCE_MANAGER::getSprite(currentItem["sprite"].GetString());
				
				glm::vec2 size;
				std::shared_ptr<PhysicsAndLogic::ICollosion> collision;
				if (type == 0) {
					float X = currentItem["size"].FindMember("x")->value.GetFloat();
					float Y = currentItem["size"].FindMember("y")->value.GetFloat();
					size = glm::vec2(X, Y);
					collision = std::make_shared<PhysicsAndLogic::RectangleCollision>(glm::vec2(0, 0), size);
				}
				else if (type == 1) {
					float radius = currentItem["radius"].GetFloat();
					size = glm::vec2(radius*2, radius*2);
					collision = std::make_shared<PhysicsAndLogic::CircleCollision>(glm::vec2(0, 0), radius);;
				}

				std::map<std::string, float> mapStats;
				auto statIt = currentItem.FindMember("effects");
				for (const auto& currentEffect : statIt->value.GetArray()) {
					mapStats.emplace(currentEffect["stat"].GetString(), currentEffect["value"].GetFloat());
				}

				m_Items.emplace(ID, std::make_shared<Item>(glm::vec2(0, 0), size, sprite, collision, mapStats));
			}
		}
	}

	m_ItemsScene.clear();
	auto ItemSceneIt = JSONDoc.FindMember("itemsOnScene");
	if (ItemSceneIt != JSONDoc.MemberEnd()) {
		for (const auto& currentItem : ItemSceneIt->value.GetArray()) {
			int ID = currentItem["ID"].GetInt();
			float X = currentItem["position"].FindMember("x")->value.GetFloat();
			float Y = currentItem["position"].FindMember("y")->value.GetFloat();
			std::shared_ptr<Item> item = m_Items.at(ID)->copy();
			item->setPosition(glm::vec2(X, Y));
			m_ItemsScene.push_back(item);
		}
	}
}

void GameSceneResources::loadInteractiveObject(const rapidjson::Document& JSONDoc){
	m_InterObject.clear();
	auto InterObjectIt = JSONDoc.FindMember("InteractiveObject"); 
	if (InterObjectIt != JSONDoc.MemberEnd()) {
		for (const auto& currentObject : InterObjectIt->value.GetArray()) {
			float PosX = currentObject["position"].FindMember("x")->value.GetFloat();
			float PosY = currentObject["position"].FindMember("y")->value.GetFloat();
			float SizeX = currentObject["size"].FindMember("x")->value.GetFloat();
			float SizeY = currentObject["size"].FindMember("y")->value.GetFloat();
			std::string firstState = currentObject["firstState"].GetString();
			std::string secondState = currentObject["secondState"].GetString();
			std::string callback = currentObject["script"].GetString();
			std::string nameSound = currentObject["nameSound"].GetString();

			std::shared_ptr<PhysicsAndLogic::ICollosion> collision = std::make_shared<PhysicsAndLogic::RectangleCollision>(glm::vec2(PosX, PosY), glm::vec2(SizeX, SizeY));
			std::shared_ptr<Script> script = std::make_shared<Script>(collision, callback, true, true);
			m_InterObject.push_back(std::make_shared<InteractiveObject>(glm::vec2(PosX, PosY), glm::vec2(SizeX, SizeY), script, firstState, secondState, collision, nameSound));
		}
	}
}

std::shared_ptr<Item> GameSceneResources::createItemFromID(const uint32_t& id){
	if (m_Items.find(id) == m_Items.end()) return nullptr;
	return m_Items.at(id)->copy();
}

std::shared_ptr<Entity> GameSceneResources::createEntityFromID(const uint32_t& id){
	if (m_Entityes.find(id) == m_Entityes.end()) return nullptr;
	return m_Entityes.at(id)->copy();
}

std::shared_ptr<Entity> GameSceneResources::getMainCharecter(){
	return m_MainCharacter;
}

std::vector<std::shared_ptr<Collider>>& GameSceneResources::getColliders(){
	return m_Colliders;
}

std::vector<std::shared_ptr<Script>>& GameSceneResources::getScripts(){
	return m_Scripts;
}

std::vector<std::shared_ptr<Item>>& GameSceneResources::getItems(){
	return m_ItemsScene;
}

std::vector<std::shared_ptr<InteractiveObject>>& GameSceneResources::getInterObjects(){
	return m_InterObject;
}

std::vector<std::shared_ptr<Entity>>& GameSceneResources::getEntity(){
	return m_Objects;
}

void GameSceneResources::clear(){
	m_pathMainCharacter="";
	m_pathCamera = "";
	m_pathEntityes = "";
	m_pathProjectile = "";
	m_pathItem = "";
}
