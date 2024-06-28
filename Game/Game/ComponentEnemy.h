#pragma once

#include "Engine/Component.h"
#include "ComponentHealthManager.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "Engine/Components/ComponentPhysicsBody.h"

class ComponentEnemy : public MyEngine::Component {
public:
	void CreateEnemy(float speed, const std::string& target, bool hasTarget, float agroRange);
	void Destroy();

private:
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;

	void SpawnGem();
	void Die();

private:
	float _movSpeed;
	float _agroRange;

	bool _isDead;
	bool _chasingTarget;

	std::weak_ptr<ComponentPhysicsBody> _physicsBody;
	std::weak_ptr<ComponentRendererSprite> _spriteRenderer;
	std::weak_ptr<ComponentHealthManager> _healthManager;
	std::weak_ptr<MyEngine::GameObject> _target;
};