#include "ComponentDestructibleBlock.h"
#include "ComponentGem.h"

#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentDestructibleBlock::OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (other->GetGameObject().lock()->GetName().rfind("Bullet", 0) == 0) {
		_setInactive = true;
	}
}

void ComponentDestructibleBlock::Update(float deltaTime) {

	if (_setInactive) {
		_setInactive = false;
		auto engine = MyEngine::Engine::GetInstance();
		auto random = engine->RandomRange(0.0, 5.0);
		if (random >= 3.33f) {
			SpawnGem();
		}
		// Setting to inactive because it is still used by the level's pool
		GetGameObject().lock()->SetActive(false);
	}
}

void ComponentDestructibleBlock::SpawnGem() {
	//Make gems from the blocks
	auto engine = MyEngine::Engine::GetInstance();
	std::string name = "GemObject";
	name += std::to_string(engine->RandomRange(1.f, 100.f));

	auto gemObject = engine->CreateGameObject(name).lock();
	gemObject->ResetTransform();
	gemObject->SetPosition(GetGameObject().lock()->GetPosition());
	auto renderer = gemObject->CreateComponent<ComponentRendererSprite>().lock();
	if (engine->RandomRange(-1.f, 1.f) > 0) {
		renderer->SetSprite("platformer-art-deluxe", "288.png");
		renderer->SetScale({2.0f, 2.0f});
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