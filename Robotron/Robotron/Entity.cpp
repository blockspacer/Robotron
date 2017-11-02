#include "Entity.h"

Entity::Entity()
	: m_componentMask{ 0 }
{
}

bool Entity::operator==(const Entity& rhs) const
{
	return this == &rhs;
}

bool Entity::hasComponents(size_t componentMask) const
{
	return (m_componentMask & componentMask) == componentMask;
}

bool Entity::hasComponentsAny(size_t componentMask) const
{
	return (m_componentMask & componentMask) > 0;
}

bool Entity::hasComponents() const
{
	return m_componentMask != 0;
}

bool Entity::hadComponentsPrev(size_t componentMask) const
{
	return (m_componentMaskPrev & componentMask) == componentMask;
}

bool Entity::matches(size_t lhsComponentMask, size_t rhsComponentMask)
{
	return (lhsComponentMask & rhsComponentMask) == rhsComponentMask;
}

bool Entity::matchesAny(size_t lhsComponentMask, size_t rhsComponentMask)
{
	return (lhsComponentMask & rhsComponentMask) > 0;
}

void Entity::addComponents(size_t componentMask)
{
	size_t tmp = m_componentMask;

	if (matches(componentMask, COMPONENT_TRANSFORM)) {
		m_componentMask |= COMPONENT_TRANSFORM;
		transform = {};
	}
	if (matches(componentMask, COMPONENT_PHYSICS)) {
		m_componentMask |= COMPONENT_PHYSICS;
		physics = {};
	}
	if (matches(componentMask, COMPONENT_MODEL)) {
		m_componentMask |= COMPONENT_MODEL;
		model = {};
	}
	if (matches(componentMask, COMPONENT_CAMERA)) {
		m_componentMask |= COMPONENT_CAMERA;
	}
	if (matches(componentMask, COMPONENT_PLAYER_CONTROL)) {
		m_componentMask |= COMPONENT_PLAYER_CONTROL;
		controlVars = {};
	}
	if (matches(componentMask, COMPONENT_PLAYER)) {
		m_componentMask |= COMPONENT_PLAYER;
		playerStats = {};
	}
	if (matches(componentMask, COMPONENT_INPUT)) {
		m_componentMask |= COMPONENT_INPUT;
		input = {};
	}
	if (matches(componentMask, COMPONENT_INPUT_MAP)) {
		m_componentMask |= COMPONENT_INPUT_MAP;
		inputMap = {};
	}
	if (matches(componentMask, COMPONENT_LOGIC)) {
		m_componentMask |= COMPONENT_LOGIC;
		logicVars = {};
	}
	if (matches(componentMask, COMPONENT_ZOMBIE)) {
		m_componentMask |= COMPONENT_ZOMBIE;
	}
	if (matches(componentMask, COMPONENT_SNAKE)) {
		m_componentMask |= COMPONENT_SNAKE;
	}
	if (matches(componentMask, COMPONENT_ENEMY_SHOOTER)) {
		m_componentMask |= COMPONENT_ENEMY_SHOOTER;
	}
	if (matches(componentMask, COMPONENT_NETWORK)) {
		m_componentMask |= COMPONENT_NETWORK;
		// network.id should be left alone as a non-negative value in combination
		// with an isNewEntity=true indicates to the network that an entity has
		// been destroyed and its memory reused.
		network.isNewEntity = true;
		network.priority = 999;
		network.lastInputReceived = {};
		network.receivedInputThisFrame = false;
	}
	if (matches(componentMask, COMPONENT_SCOREPICKUP)) {
		m_componentMask |= COMPONENT_SCOREPICKUP;
	}
	if (matches(componentMask, COMPONENT_PLAYERBULLET)) {
		m_componentMask |= COMPONENT_PLAYERBULLET;
	}
	if (matches(componentMask, COMPONENT_ENEMYBULLET)) {
		m_componentMask |= COMPONENT_ENEMYBULLET;
	}

	if (matchesAny(componentMask, COMPONENT_ZOMBIE, COMPONENT_SNAKE, 
	                              COMPONENT_ENEMY_SHOOTER, COMPONENT_SCOREPICKUP, 
	                              COMPONENT_PLAYERBULLET, COMPONENT_ENEMYBULLET)) {
		aiVariables = {};
		aiVariables.wanderPosition = { 0, 1, 0 };
		controlVars = {};
	}

	if (tmp != m_componentMask)
		m_componentMaskPrev = tmp;
}

void Entity::removeComponents(size_t componentMask)
{
	size_t tmp = m_componentMask;

	m_componentMask &= (~componentMask);

	if (tmp != m_componentMask)
		m_componentMaskPrev = tmp;
}

void Entity::destroy()
{
	size_t tmp = m_componentMask;

	m_componentMask = 0;

	if (tmp != m_componentMask)
		m_componentMaskPrev = tmp;
}
