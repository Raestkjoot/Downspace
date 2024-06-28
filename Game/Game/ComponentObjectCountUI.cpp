#include "ComponentObjectCountUI.h"

#include "ComponentHealthManagerWithUI.h"

void ComponentObjectCountUI::Init(rapidjson::Value& serializedData) {
	auto engine = MyEngine::Engine::GetInstance();

	auto healthManager = engine->GetGameObject("Player").lock()->
		FindComponent<ComponentHealthManagerWithUI>();
	_maxCount = healthManager.lock()->SetUIComponent(this);
	_curCount = _maxCount - 1;

	glm::vec2 screenSize = engine->GetScreenSize();
	_position.x -= screenSize.x / 2.f;
	_position.y += screenSize.y / 2.f;

	for (int i = 0; i < _maxCount; ++i) {
		CreateSprite();
	}
}

void ComponentObjectCountUI::Decrement() {
	if (_curCount < 0) return;

	_sprites[_curCount].lock()->SetActive(false);
	_curCount--;
}

void ComponentObjectCountUI::Reset() {
	for (auto uiElement : _sprites) {
		uiElement.lock()->SetActive(true);
	}
	_curCount = _maxCount - 1;
}

void ComponentObjectCountUI::CreateSprite() {
	auto uiElement = GetGameObject().lock()->CreateComponent<ComponentRendererSprite>().lock();
	uiElement->SetSprite("platformer-art-deluxe", _spriteName);

	_position += _padding;
	uiElement->SetOffset(_position);
	uiElement->SetSortingOrder(1);

	_sprites.push_back(uiElement);
}