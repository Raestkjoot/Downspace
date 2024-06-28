#pragma once

#include "Engine/Component.h"

class ComponentSideContact : public MyEngine::Component {
public:
	void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) override;
	void OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) override;

	bool CanMoveHorizontal(float, float);

private:
	bool _isTouchingSide = false;
	const float _threshold = 0.2f;
	std::list<std::weak_ptr<MyEngine::GameObject>> _contacts;
};