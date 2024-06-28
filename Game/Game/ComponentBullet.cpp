#include "ComponentBullet.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "ComponentHealthManager.h"
#include "Engine\MyEngine.h"

void ComponentBullet::Update(float deltaTime) {
	_timer += deltaTime;

	if (_timer > 1.5f) {
		_markForDestroy = true;
	}
	if (_markForDestroy) {
		MyEngine::Engine::GetInstance()->DestroyGameObject(GetGameObject().lock().get());
	}
}

void ComponentBullet::OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (other->GetGameObject().lock()->GetName().rfind("Block", 0) == 0) {
		_markForDestroy = true;
	}
	if (other->GetGameObject().lock()->GetName().rfind("Enemy", 0) == 0) {
		other->GetGameObject().lock()->FindComponent<ComponentHealthManager>().lock()->TakeDamage(1);
		_markForDestroy = true;
	}
}