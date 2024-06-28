#pragma once

#include "Engine/Component.h"

class ComponentFollowTarget : public MyEngine::Component {
public:
	void Init(std::weak_ptr<MyEngine::GameObject> target, bool lockX = true, bool lockY = true);

private:
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;

private:
	std::string _targetId;
	std::weak_ptr<MyEngine::GameObject> _target;
	float _deadZoneDistance = 200.0f;
	float _offsetX = 0.0f;
	bool _lockX;
	bool _lockY;
};