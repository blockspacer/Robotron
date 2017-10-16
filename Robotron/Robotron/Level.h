//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : Controls the gameplay scene and components
//				  
// Author       : Jack Mair
// Mail         : jack.mai7246@mediadesign.school.nz
//

#define _USE_MATH_DEFINES

#include "Utils.h"
#include "GLUtils.h"
#include "SceneUtils.h"
#include "InputSystem.h"
#include "PlayerControlSystem.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "NetworkSystem.h"
#include "NetworkServerSystem.h"
#include "GameplayLogicSystem.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <cmath>

#pragma once
class Level
{
public:
	Level(GLFWwindow* window, Scene& scene, int levelNum);
	~Level();

	void process();

private:
	GLFWwindow* m_window;
	Scene& m_scene;
	RenderSystem m_renderSystem;
	PlayerControlSystem m_movementSystem;
	InputSystem m_inputSystem;
	NetworkServerSystem m_networkServerSystem;

	//stores the level current on. effects enemy spawn rate and spawn positioning
	int m_levelNum;
};
