#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "UIGame/Menu.h"
#include "../GameEngine/GameScene.h"

#define MAIN_GAME_CLASS MainGameClass

 //(RUS) ������� ������� �����, ������� �� ������ � �������� ���������, �� ��������� � ����������, � ��� �� �������� � ���������
//(ENG) The main game class, responsible for working with game objects, rendering and updating them, and also works with clicks

class MainGameClass {
public:
	static bool init(glm::vec2 window);

	static void update(const double& duration);
	static void render();

	static void events();
	static void setProjectionMat(const glm::ivec2& window);
	static void setGame(const bool& flag);

	static void terminate();

	static Menu& getMenu();
	static GameScene& getScene();

private:
	MainGameClass();
	~MainGameClass() {};

	static bool m_StateGame;
	static GameScene m_gameScene;
	static Menu m_Menu;
	static glm::ivec2 m_window;
	static double m_time;
	static int m_fps;
};