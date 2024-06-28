#include "ComponentGem.h"

#include "Engine/MyEngine.h"

void ComponentGem::Init(int value) {
	_gemValue = value;
}

void ComponentGem::Update(float deltaTime) {
	_timer += deltaTime;

	if (_markForDestruction) {
		MyEngine::Engine::GetInstance()->DestroyGameObject(GetGameObject().lock().get());
	}

	if (_timer > 10.f) {
		_markForDestruction = true;
	}
}

int ComponentGem::PickUpGem() {
	_markForDestruction = true;
	return _gemValue;
}

int ComponentGem::GetValue() {
	return _gemValue;
}

void ComponentGem::OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (other->GetGameObject().lock()->GetName().rfind("Player", 0) == 0) {
		auto body = GetGameObject().lock()->FindComponent<ComponentPhysicsBody>().lock();
		body->setLinearVelocity({ 0, 0 });
		body->SetGravityScale(0.0f);
	}
}