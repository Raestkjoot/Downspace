#pragma once

#include "Engine/Component.h"
#include "Engine/GameObjectPool.h"

class ComponentBullet : public MyEngine::Component {
private:
	void Update(float deltaTime) override;
	void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) override;

private:
	float _timer = 0.0f;
	bool _markForDestroy;
};