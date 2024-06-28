#include "ComponentEnemy.h"
#include "ComponentController.h"
#include "Engine/MyEngine.h"
#include "ComponentGem.h"
#include "glm/ext.hpp"

void ComponentEnemy::Init(rapidjson::Value& serializedData) {
	CreateEnemy(serializedData["movSpeed"].GetFloat(), 
		serializedData["target"].GetString(), 
		true, 
		serializedData["agroRange"].GetFloat());
}

void ComponentEnemy::Update(float deltaTime) {
	if (_isDead) {
		MyEngine::Engine::GetInstance()->DestroyGameObject(GetGameObject().lock().get());
	}
	else if (_healthManager.lock()->GetHealth() <= 0) {
		Die();
	}

	_chasingTarget = (glm::distance(GetGameObject().lock()->GetPosition(), _target.lock()->GetPosition()) < _agroRange);

	if (_chasingTarget && !_target.lock()->FindComponent<ComponentController>().lock()->IsGameOver()) {
		auto targetPos = _target.lock()->GetPosition();
		auto pos = GetGameObject().lock()->GetPosition();
		auto directionVector = glm::vec2(targetPos.x - pos.x, targetPos.y - pos.y);
		_physicsBody.lock()->setLinearVelocity(glm::normalize(directionVector) * _movSpeed);
	} else {
		_physicsBody.lock()->setLinearVelocity({ 0.0f, 0.0f });
	}
}

void ComponentEnemy::Die() {
	_isDead = true;
	SpawnGem();
}

void ComponentEnemy::SpawnGem() {
	//Make gems from the blocks
	auto engine = MyEngine::Engine::GetInstance();
	std::string name = "GemObject";
	name += GetGameObject().lock()->GetName();

	auto gemObject = engine->CreateGameObject(name).lock();
	gemObject->ResetTransform();
	gemObject->SetPosition(GetGameObject().lock()->GetPosition());
	auto renderer = gemObject->CreateComponent<ComponentRendererSprite>().lock();
	if (engine->RandomRange(-1.f, 1.f) > 0) {
		renderer->SetSprite("platformer-art-deluxe", "288.png");
		renderer->SetScale({ 2.0f, 2.0f });
		auto body = gemObject->CreateComponent<ComponentPhysicsBody>().lock();
		glm::vec2 size = renderer->GetSize() / 2.0f;
		body->CreateBody(b2_dynamicBody, false, size);
		body->SetGravityScale(1.0f);
		body->addImpulse(glm::vec2(engine->RandomRange(-1.f, 1.f), engine->RandomRange(2.f, 4.f)));
		auto gem = gemObject->CreateComponent<ComponentGem>().lock();
		gem->Init(3);
	}
	else {
		renderer->SetSprite("platformer-art-deluxe", "145.png");
		renderer->SetScale({ 2.0f, 2.0f });
		auto body = gemObject->CreateComponent<ComponentPhysicsBody>().lock();
		glm::vec2 size = renderer->GetSize() / 2.0f;
		body->CreateBody(b2_dynamicBody, false, size);
		body->SetGravityScale(1.0f);
		body->addImpulse(glm::vec2(engine->RandomRange(-1.f, 1.f), engine->RandomRange(2.f, 4.f)));
		auto gem = gemObject->CreateComponent<ComponentGem>().lock();
		gem->Init(6);
	}
}

void ComponentEnemy::CreateEnemy(float speed, const std::string& target, bool hasTarget, float agroRange) {
	_movSpeed = speed;
	_agroRange = agroRange;
	if (hasTarget) {
		_target = MyEngine::Engine::GetInstance()->GetGameObject(target);
	}

	auto gameObject = GetGameObject().lock();
	_physicsBody = gameObject->FindComponent<ComponentPhysicsBody>();
	_physicsBody.lock()->SetGravityScale(0.0f);
	_spriteRenderer = gameObject->FindComponent<ComponentRendererSprite>();
	_spriteRenderer.lock()->SetScale({ 2.0f, 2.0f });
	_healthManager = gameObject->CreateComponent<ComponentHealthManager>();
	_healthManager.lock()->Init(2);
	_chasingTarget = false;
	_isDead = false;
}

void ComponentEnemy::Destroy() {
	_isDead = true;
}