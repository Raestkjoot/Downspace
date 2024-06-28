#pragma once

#include "Engine/Component.h"
#include "Engine/Components/ComponentPhysicsBody.h"

class ComponentGem : public MyEngine::Component {
public:
	void Init(int value);
	int GetValue();
	int PickUpGem();

private:
	void Update(float deltaTime) override;
	void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) override;
	int _gemValue;
	float _timer = 0.0f;
	bool _markForDestruction = false;
};