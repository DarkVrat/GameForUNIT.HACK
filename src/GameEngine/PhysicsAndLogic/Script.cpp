#include "Script.h"

#include "callbackForScripts.h"

Script::Script(std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::string& callback, const bool& onlyMainEntity, const bool& disposable){
	m_collision = collision;
	m_callback = callbackForScripts::getCallback(callback);
	m_onlyMainEntity = onlyMainEntity;
	m_disposable = disposable;
}

std::shared_ptr<PhysicsAndLogic::ICollosion> Script::getCollision(){
	return m_collision;
}
 
bool Script::getOnlyMainEntity(){
	return m_onlyMainEntity;
}

bool Script::getDisposable(){
	return m_disposable;
}

void Script::run(std::shared_ptr<Entity> entity){
	m_callback(entity);
}
