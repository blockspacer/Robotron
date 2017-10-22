//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A system which handles movement based on an
//                entities input state.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#define _USE_MATH_DEFINES

#include "PlayerControlSystem.h"

#include "PlayerControlComponent.h"
#include "GLUtils.h"
#include "GLMUtils.h"
#include "Scene.h"
#include "Entity.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>

#include <cmath>

PlayerControlSystem::PlayerControlSystem(Scene& scene)
	: m_scene{ scene }
{
}

void PlayerControlSystem::update(Entity& entity)
{
	if ((entity.componentMask & COMPONENT_ENEMY01) == COMPONENT_ENEMY01)
		return;
	
	// Filter movable
	const size_t kMovableMask = COMPONENT_PLAYER_CONTROL | COMPONENT_INPUT | COMPONENT_TRANSFORM;
	const size_t kMovableCamMask = COMPONENT_CAMERA | COMPONENT_INPUT | COMPONENT_TRANSFORM;
	if ((entity.componentMask & kMovableMask) != kMovableMask && (entity.componentMask & kMovableCamMask) != kMovableCamMask)
		return;

	float moveSpeed = entity.controlVars.moveSpeed;
	float orientationSensitivity = entity.controlVars.orientationSensitivity;

	float deltaAzimuth = -orientationSensitivity * entity.input.orientationDelta.x;
	float deltaElevation = -orientationSensitivity * entity.input.orientationDelta.y;
	float roll = -orientationSensitivity * entity.input.orientationDelta.z;
	glm::vec3 front = glm::vec3{ 0, 0, -1 };
	if (!entity.controlVars.worldSpaceMove)
		front = entity.transform * glm::vec4{ front, 0 }; // Convert movement to local coordinates
	glm::vec3 pos = entity.transform[3];
	glm::vec3 up = glm::vec3{ 0, 1, 0 };
	glm::vec3 right = glm::cross(front, up);
	
	// Displacement
	glm::vec3 axis = GLMUtils::limitVec(entity.input.axis, 1);
	if (!entity.controlVars.worldSpaceMove)
		axis = entity.transform * glm::vec4{ axis, 0 }; // Convert movement to local coordinates
	entity.physics.velocity = moveSpeed * axis;
	if (axis != glm::vec3{ 0,0,0 })
		entity.aiVariables.previousVelocity = entity.physics.velocity;
	pos += entity.physics.velocity;

	// Rotation
	// Prevent elevation going past 90 degrees
	glm::mat3 elevationMat{ 1 };
	float elevation = static_cast<float>(M_PI / 2 - std::acos(glm::dot(front, up)));
	if (std::abs(elevation + deltaElevation) < M_PI / 2)
		elevationMat = glm::rotate(deltaElevation, right);
	glm::mat3 azimuthMat = glm::rotate(deltaAzimuth, up);
	glm::mat3 rollMat = glm::rotate(roll, front);

	entity.transform[3] = {0, 0, 0, 1}; // Remove displacement temporarily
	entity.transform = glm::mat4{ rollMat * azimuthMat * elevationMat } * entity.transform; // Rotation without displacement
	entity.transform[3] = glm::vec4{ pos, 1 }; // Add displacement back in
}
