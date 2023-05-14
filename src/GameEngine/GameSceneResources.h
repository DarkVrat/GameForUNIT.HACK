#pragma once

#include <string>
#include "Objects/IObject.h"
#include "Objects/Entity.h"
#include "Objects/Item.h"
#include "Objects/interactiveObject.h"
#include "PhysicsAndLogic/Collider.h"
#include "PhysicsAndLogic/Script.h"
#include <map>
#include <memory>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

class GameSceneResources{
public:
	static void load(const std::string& gameScene);

	static void loadMap(const rapidjson::Document& JSONDoc);
	static void loadCameraSettings(const rapidjson::Document& JSONDoc);
	static void loadMainMainCharacter(const rapidjson::Document& JSONDoc);
	static void loadEntityes(const rapidjson::Document& JSONDoc);
	static void loadColliders(const rapidjson::Document& JSONDoc);
	static void loadScripts(const rapidjson::Document& JSONDoc);
	static void loadProjectiles(const rapidjson::Document& JSONDoc);
	static void loadItems(const rapidjson::Document& JSONDoc);
	static void loadInteractiveObject(const rapidjson::Document& JSONDoc);

	static std::shared_ptr<Item> createItemFromID(const uint32_t& id);
	static std::shared_ptr<Entity> createEntityFromID(const uint32_t& id);
	static std::shared_ptr<Entity> getMainCharecter();
	static std::vector<std::shared_ptr<Collider>>& getColliders();
	static std::vector<std::shared_ptr<Script>>& getScripts();
	static std::vector<std::shared_ptr<Item>>& getItems();
	static std::vector<std::shared_ptr<InteractiveObject>>& getInterObjects();
	static std::vector<std::shared_ptr<Entity>>& getEntity();

	static void clear();
private:
	static std::vector<std::shared_ptr<Entity>> m_Objects;
	static std::vector<std::shared_ptr<Script>> m_Scripts;
	static std::vector<std::shared_ptr<Collider>> m_Colliders;
	static std::vector<std::shared_ptr<Item>> m_ItemsScene;
	static std::vector<std::shared_ptr<InteractiveObject>> m_InterObject;
	static std::map<uint32_t, std::shared_ptr<Item>> m_Items;
	static std::map<uint32_t, std::shared_ptr<Entity>> m_Entityes;
	static std::shared_ptr<Entity> m_MainCharacter;

	static std::string m_pathMainCharacter;
	static std::string m_pathCamera;
	static std::string m_pathEntityes;
	static std::string m_pathProjectile;
	static std::string m_pathItem;
};