#pragma once

#include "ICollision.h"
#include "../Objects/Entity.h"
#include "../../GameClass/MainGameClass.h"
#include "CircleCollision.h"
#include <functional>
#include <sstream>
#include <random>

class callbackForScripts {
public:
	static std::function<void(std::shared_ptr<Entity>)> getCallback(const std::string& str);

private:
	static std::vector<std::string> split(std::string str, char delim);
	static int getInt(const std::string& str, const std::string& name);
	static bool getBool(const std::string& str, const std::string& name);
	static float getFloat(const std::string& str, const std::string& name);
	static glm::vec2 getVec2(const std::string& str, const std::string& name);
	static std::string getString(const std::string& str, const std::string& name);
	static std::vector<float> getFloatVector(const std::string& str, const std::string& name);

	static int getIntRandom(const std::string& str);
};

inline std::function<void(std::shared_ptr<Entity>)> callbackForScripts::getCallback(const std::string& str){
	std::vector<std::function<void(std::shared_ptr<Entity>)>> vecCommands;
	std::vector<std::string> commands = split(str, ';');

	for (const auto& cmd : commands) {
		if (cmd.find("load ") == 0) {
			std::string scene = cmd.substr(5); 
			vecCommands.push_back([scene](std::shared_ptr<Entity> entity) {MAIN_GAME_CLASS::getScene().init(scene); });
		}
		else if (cmd.find("teleport ") == 0) {
			glm::vec2 VEC2 = getVec2(cmd, "POS");
			vecCommands.push_back([VEC2](std::shared_ptr<Entity> entity) {entity->setPosition(VEC2); });
		}
		else if (cmd.find("add ") == 0) {
			float value = getFloat(cmd, "value");
			std::string stat = getString(cmd, "stat");
			vecCommands.push_back([stat, value](std::shared_ptr<Entity> entity) {entity->addStat(stat, value); });
		}
		else if (cmd.find("createItem ") == 0) {
			glm::vec2 VEC2 = getVec2(cmd, "POS");
			int id = getInt(cmd, "ID");
			if (id == -1) { id = getIntRandom(cmd); }
			vecCommands.push_back([VEC2, id](std::shared_ptr<Entity> entity) {
				std::shared_ptr<Item> item = GameSceneResources::createItemFromID(id); 
				item->setPosition(VEC2);
				GameSceneResources::getItems().push_back(item);
				});
		}
		else if (cmd.find("addGun ") == 0) { 
			int id = getInt(cmd, "ID");
			if (id == -1) { id = getIntRandom(cmd); }
			vecCommands.push_back([id](std::shared_ptr<Entity> entity) {entity->getManagerProjectile()->addTypesProjectile(id);});
		}
		else if (cmd.find("spawn ") == 0) { 
			int id = getInt(cmd, "ID");
			if (id == -1)id = getIntRandom(cmd);
			glm::vec2 VEC2 = getVec2(cmd, "POS");
			float radius = getFloat(cmd, "radius");
			bool chiken = getBool(cmd, "chiken");
			std::vector<float> X = getFloatVector(cmd, "patrol_X");
			std::vector<float> Y = getFloatVector(cmd, "patrol_Y");
			std::vector<float> time = getFloatVector(cmd, "patrol_time");
			size_t minSize = std::min(std::min(X.size(), Y.size()), time.size());
			std::vector<std::pair<glm::vec2, float>> positionsPatrol;
			for (int i = 0; i < minSize; i++) {
				positionsPatrol.push_back(std::pair<glm::vec2, float>(glm::vec2(X.at(i), Y.at(i)), time.at(i)));
			}

			std::shared_ptr<Entity> enemy = GameSceneResources::createEntityFromID(id);
			enemy->setPosition(VEC2);
			std::shared_ptr<PhysicsAndLogic::ICollosion> collision = std::make_shared<PhysicsAndLogic::CircleCollision>(VEC2, radius);
			std::shared_ptr<AIController> controller = std::make_shared<AIController>(positionsPatrol, collision, chiken);
			enemy->setAI(controller);

			vecCommands.push_back([enemy](std::shared_ptr<Entity> entity) {GameSceneResources::getEntity().push_back(enemy); });
		}
		else if (cmd.find("dialog ")==0) {
			std::string dialog = getString(cmd, "str"); 
			vecCommands.push_back([dialog](std::shared_ptr<Entity> entity) {MAIN_GAME_CLASS::getScene().getUI().addDialog(dialog); }); 
		}
	}
	  
	return [vecCommands](std::shared_ptr<Entity> entity) {
		for (auto& command : vecCommands) {
			command(entity);
		}
	};
}

inline std::vector<std::string> callbackForScripts::split(std::string str, char delim){ 
	while (str.find("  ") != -1) {
		str.replace(str.find("  "), 2, " ");
	}

	while (str.find(" ;") != -1) {
		str.replace(str.find(" ;"), 2, ";");
	}

	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;

	while (std::getline(ss, item, delim)) {
		if (item[0] == ' ') {
			item = item.substr(1);
		}
		result.push_back(item);
	}
	return result;
}

inline int callbackForScripts::getInt(const std::string& str, const std::string& name){
	size_t Pos = str.find(name);
	if (Pos == std::string::npos) return -1;

	size_t start_pos = str.find("(", Pos);
	size_t end_pos = str.find(")", start_pos);

	int value = std::stoi(str.substr(start_pos + 1, end_pos - start_pos - 1));

	return value;
}

inline bool callbackForScripts::getBool(const std::string& str, const std::string& name){
	size_t Pos = str.find(name);
	if (Pos == std::string::npos) return false;

	size_t start_pos = str.find("(", Pos);
	size_t end_pos = str.find(")", start_pos);

	bool value = str.substr(start_pos + 1, end_pos - start_pos - 1).find("true")!=std::string::npos;

	return value;
}

inline float callbackForScripts::getFloat(const std::string& str, const std::string& name){
	size_t Pos = str.find(name);
	if ( Pos == std::string::npos) return 0.f;

	size_t start_pos = str.find("(", Pos);
	size_t end_pos = str.find(")", start_pos);

	float value = std::stof(str.substr(start_pos + 1, end_pos - start_pos - 1));

	return value;
}

inline glm::vec2 callbackForScripts::getVec2(const std::string& str, const std::string& name){
	size_t Pos = str.find(name);
	if (Pos == std::string::npos) return glm::vec2(0, 0);

	size_t start_pos = str.find("(", Pos);
	size_t comma_pos = str.find(",", start_pos);
	size_t end_pos = str.find(")", comma_pos);

	float x = std::stof(str.substr(start_pos + 1, comma_pos - start_pos - 1));
	float y = std::stof(str.substr(comma_pos + 1, end_pos - comma_pos - 1));

	return glm::vec2(x, y);
}

inline std::string callbackForScripts::getString(const std::string& str, const std::string& name){
	size_t Pos = str.find(name);
	if (Pos == std::string::npos) return "";

	size_t start_pos = str.find("(", Pos);
	size_t end_pos = str.find(")", start_pos);

	std::string value = str.substr(start_pos + 1, end_pos - start_pos - 1);

	return value;
}

inline std::vector<float> callbackForScripts::getFloatVector(const std::string& str, const std::string& name){
	std::vector<float> vec;
	size_t Pos = str.find(name);
	if (Pos == std::string::npos) return vec;

	size_t left = str.find("[", Pos);
	size_t right = str.find("]", left);

	if(right<left) return vec;

	while (left<right){
		size_t comma_pos = str.find(",", left+1);
		if (comma_pos == std::string::npos)comma_pos = right;
		vec.push_back(std::stof(str.substr(left + 1, comma_pos - left - 1)));
		left = comma_pos;
	}
	
	return vec;
}

inline int callbackForScripts::getIntRandom(const std::string& str){
	std::srand(time(0));
	size_t Pos = str.find("random");
	if (Pos == std::string::npos) return 0;

	size_t start_pos = str.find("(", Pos);
	size_t comma_pos = str.find(",", start_pos);
	size_t end_pos = str.find(")", comma_pos);

	int min = std::stoi(str.substr(start_pos + 1, comma_pos - start_pos - 1));
	int max = std::stoi(str.substr(comma_pos + 1, end_pos - comma_pos - 1));

	std::random_device rd;  
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);;
}
