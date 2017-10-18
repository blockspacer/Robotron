//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A container for all the entities / components in 
//                scene.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include "Entity.h"

#include <vector>
#include <memory>

struct Scene {
public:
	Entity& createEntity();
	void destroyEntity(Entity&);

	// TODO: Change this to be a vector of 'observable' entities
	// that can be observed robustly, even when a vector resize causes the
	// entities to shift in memory.
	std::vector<std::unique_ptr<Entity>> entities;
};