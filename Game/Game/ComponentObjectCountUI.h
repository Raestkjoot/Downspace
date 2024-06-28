#pragma once

#include "imgui.h"
#include "Engine/MyEngine.h"
#include "Engine/Component.h"
#include "Engine/Components/ComponentRendererSprite.h"

// TODO: Generalize to a ComponentObjectCountUI that we can inherit from to create UI for health and ammo

class ComponentObjectCountUI : public MyEngine::Component {
public:
	void Decrement();
	void Reset();

private:
	void Init(rapidjson::Value&) override;

	void CreateSprite();

	std::vector<std::weak_ptr<ComponentRendererSprite>> _sprites;
	std::string _spriteName = "139.png";
	glm::vec2 _position = glm::vec2(5.f, -20.f);
	glm::vec2 _padding = glm::vec2(25.f, 0.f);
	int _maxCount = 0;
	int _curCount = 0;
	std::string _baseName = "Object";
};