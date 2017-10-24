//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A system which handles movement ai
//                for Enemy01 enemy types. They
//				  around the arena and pursue with
//				  flocking the closest player in their
//				  aggro range.
// Author       : Jack Mair
// Mail         : jack.mai7246@mediadesign.school.nz
//

#define _USE_MATH_DEFINES

#include "PlayerBulletSystem.h"

#include "GLUtils.h"
#include "GLMUtils.h"
#include "Utils.h"
#include "AIUtils.h"
#include "Scene.h"
#include "Entity.h"
#include "RenderSystem.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>

#include <cmath>

PlayerBulletSystem::PlayerBulletSystem(Scene& scene)
	: m_scene{ scene }
{

}

// Updates the position of the bullets every turn to travel in its travel directory.
// Checks to see if it has hit an enemy or a wall.
void PlayerBulletSystem::update(Entity& entity, float deltaTick)
{
	// Check that the entity is an Enemy02 object before proceeding.
	if ((entity.componentMask & COMPONENT_PLAYERBULLET) != COMPONENT_PLAYERBULLET)
		return;
	
	if ((   entity.transform[3] + glm::vec4{ entity.physics.velocity, 0 }).x > 20.0f
		|| (entity.transform[3] + glm::vec4{ entity.physics.velocity, 0 }).x < -20.0f
		|| (entity.transform[3] + glm::vec4{ entity.physics.velocity, 0 }).z > 20.0f
		|| (entity.transform[3] + glm::vec4{ entity.physics.velocity, 0 }).z < -20.0f)
	{
		m_scene.destroyEntity(entity);
		return;
	}

	// Cycle through all the entities in the scene and check if the bullet hit an enemy.
	for (unsigned int i = 0; i < m_scene.entities.size(); ++i)
	{
		if (((m_scene.entities.at(i)->componentMask & COMPONENT_ENEMY01) == COMPONENT_ENEMY01
			|| (m_scene.entities.at(i)->componentMask & COMPONENT_ENEMY02) == COMPONENT_ENEMY02
			|| (m_scene.entities.at(i)->componentMask & COMPONENT_ENEMY03) == COMPONENT_ENEMY03)
			&& glm::length(m_scene.entities.at(i)->transform[3] - entity.transform[3]) < 1)
		{
			m_scene.destroyEntity(entity);
			m_scene.destroyEntity(*m_scene.entities.at(i));
			return;
		}
	}

	entity.transform[3] += glm::vec4{ entity.physics.velocity, 0 };

	return;
}