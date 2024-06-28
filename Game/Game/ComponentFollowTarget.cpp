#include "ComponentFollowTarget.h"

#include "Engine/MyEngine.h"

void ComponentFollowTarget::Init(std::weak_ptr<MyEngine::GameObject> target, bool lockX, bool lockY) {
	_target = target;
	_lockX = lockX;
	_lockY = lockY;
}

void ComponentFollowTarget::Init(rapidjson::Value& serializedData) {
	_targetId = serializedData["target"].GetString();
	_offsetX = serializedData["offsetX"].GetFloat();
	_lockX = serializedData["lockX"].GetBool();
	_lockY = serializedData["lockY"].GetBool();

	_target = MyEngine::Engine::GetInstance()->GetGameObject(_targetId);
}

void ComponentFollowTarget::Update(float deltaTime) {
	auto target = _target.lock();
	if (!target)
		return;

	auto gameObject = GetGameObject().lock();
	if (!gameObject)
		return;

	auto targetPos = target->GetPosition();
	auto pos = gameObject->GetPosition();

	if (!_lockX)
		pos.x = targetPos.x + _offsetX;
	if (!_lockY) {
		if (pos.y > targetPos.y || glm::abs(targetPos.y - pos.y) > _deadZoneDistance ) {
			pos.y = targetPos.y;
		}
	}
	gameObject->SetPosition(pos);
}