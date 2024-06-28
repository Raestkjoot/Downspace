#pragma once

#include "Engine/Component.h"
#include "ComponentLevel.h"

class ComponentDestructibleBlock : public MyEngine::Component {
private:
	void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) override;
	void Update(float deltaTime);

	void SpawnGem();

	bool _setInactive = false;
};