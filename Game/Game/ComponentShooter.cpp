#include "ComponentShooter.h"

#include "glm/glm.hpp"
#include "SDL.h"

#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "Engine/MyEngine.h"

#include "ComponentBullet.h"
#include "ComponentScore.h"

void ComponentShooter::Init() {
	_engine = MyEngine::Engine::GetInstance();
	_gameObject = GetGameObject();
	_count = 0;
	_ammunition = _gameObject.lock()->CreateComponent<ComponentAmmunition>();
	_ammunition.lock()->Init();
}


bool ComponentShooter::ShootBullet(std::weak_ptr<MyEngine::GameObject> gameObject) {	
	if (_ammunition.lock()->GetAmmunition() <= 0) {
		return false;
	}

	std::string name = "BulletObject";
	name += std::to_string(_count);
	_count++;

	auto bulletObject = _engine->CreateGameObject(name, _gameObject).lock();
	bulletObject->SetPosition(glm::vec3(gameObject.lock()->GetPosition().x, gameObject.lock()->GetPosition().y - 10.f, gameObject.lock()->GetPosition().z));
	auto renderer = bulletObject->CreateComponent<ComponentRendererSprite>().lock();
	renderer->SetSprite("platformer-art-deluxe", "407.png");

	auto body = bulletObject->CreateComponent<ComponentPhysicsBody>().lock();
	glm::vec2 size = renderer->GetSize() / 2.0f;
	body->CreateBody(b2_dynamicBody, true, size);
	body->SetGravityScale(0.0f);
	body->setLinearVelocity(glm::vec2(0.f, -6.f));
	auto bullet = bulletObject->CreateComponent<ComponentBullet>().lock();
	_ammunition.lock()->ConsumeAmmunition();
	return true;
}

void ComponentShooter::ReloadMag() {
	_ammunition.lock()->Reload();
}