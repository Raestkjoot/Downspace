#include "ComponentController.h"
#include "ComponentEnemy.h"

#include "glm/glm.hpp"
#include "SDL.h"

#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "Engine/MyEngine.h"

#include "ComponentGem.h"
#include "ComponentLevel.h"
#include "ComponentFootContact.h"
#include "ComponentHealthManagerWithUI.h"

void ComponentController::Init(rapidjson::Value& serializedData) {
	_movSpeed = serializedData["movSpeed"].GetFloat();
	_jumpPower = serializedData["jumpPower"].GetFloat();

	auto gameObject = GetGameObject().lock();
	_physicsBody = gameObject->FindComponent<ComponentPhysicsBody>();
	_physicsBody.lock()->SetGravityScale(1.2f);
	_spriteRenderer = gameObject->FindComponent<ComponentRendererSprite>();

	_shooter = gameObject->CreateComponent<ComponentShooter>();
	_shooter.lock()->Init();

	_engine = MyEngine::Engine::GetInstance();

	_score = _engine->GetGameObject("PointCounter").lock()->FindComponent<ComponentScore>();

	// Health component
	_healthManager = GetGameObject().lock()->CreateComponent<ComponentHealthManagerWithUI>();
	_healthManager.lock()->Init(4);

	// Contacts
	_footContact = GetGameObject().lock()->CreateComponent<ComponentFootContact>();
	_footContact.lock()->SetShooter(_shooter);
	_footContact.lock()->SetScore(_score);
  
	_sideContact = GetGameObject().lock()->CreateComponent<ComponentSideContact>();
}

void ComponentController::Update(float deltaTime) {
	auto physicsBody = _physicsBody.lock();
	if (!physicsBody) {
		return;
	}

	if (_isGameOver) {
		physicsBody->setLinearVelocity({ 0.f, 0.f });
		return;
	}

	auto linearVeclocity = physicsBody->getLinearVelocity();

	// Set horizontal movement according to input and wall collision
	float movement = _movRight - _movLeft;
	if (_sideContact.lock()->CanMoveHorizontal(GetGameObject().lock()->GetPosition().x, movement)) {
		linearVeclocity.x = movement * _movSpeed;
	}
	else {
		linearVeclocity.x = 0.0f;
	}
	// Clamp vertical movement
	linearVeclocity.y = glm::clamp(linearVeclocity.y, -_maxVerticalSpeedDown, _maxVerticalSpeedUp);

	physicsBody->setLinearVelocity(linearVeclocity);

	UpdateSprite(deltaTime, linearVeclocity);

}

void ComponentController::KeyEvent(SDL_Event& event) {
	bool keydown = event.type == SDL_KEYDOWN;

	switch (event.key.keysym.sym) {
		// Jumping and shooting
	case SDLK_SPACE:
	case SDLK_w:
	case SDLK_UP:
		TryJump(keydown);
		break;

		//Left and right movement
	case SDLK_a:
	case SDLK_LEFT:
		_movLeft = keydown && !_isGameOver; break;
	case SDLK_d:
	case SDLK_RIGHT:
		_movRight = keydown && !_isGameOver; break;
	}
}

void ComponentController::OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (other->GetGameObject().lock()->GetName().rfind("Gem", 0) == 0 && !_isGameOver) {
		auto gem = other->GetGameObject().lock()->FindComponent<ComponentGem>().lock();
		_score.lock()->AddPoints(gem->GetValue()*(_score.lock()->GetCombo()+1));
		gem->PickUpGem();
	}
	else if (other->GetGameObject().lock()->GetName().rfind("Enemy", 0) == 0 && !_isGameOver){
		if (manifold->localNormal.y > 0.2f) {
			other->GetGameObject().lock()->FindComponent<ComponentHealthManager>().lock()->TakeDamage(2);
			_score.lock()->IncreaseCombo();
			auto enemyPos = other->GetGameObject().lock()->GetPosition();
			auto pos = GetGameObject().lock()->GetPosition();
			auto directionVector = glm::vec2(pos.x - enemyPos.x, pos.y - enemyPos.y);
			_physicsBody.lock()->addImpulse(glm::normalize(directionVector) * 10.0f);
			_shooter.lock()->ReloadMag();
		}
		else  {
			_healthManager.lock()->TakeDamage(1);
			auto enemyPos = other->GetGameObject().lock()->GetPosition();
			auto pos = GetGameObject().lock()->GetPosition();
			auto directionVector = glm::vec2(pos.x - enemyPos.x, pos.y - enemyPos.y);
			_score.lock()->ResetCombo();
			if (_healthManager.lock()->GetHealth() <= 0) {
				GameOver();
			}

			if (manifold->localNormal.y < -0.2) {
				_physicsBody.lock()->addImpulse(glm::normalize(directionVector) * 0.5f);
			}
			else {
				_physicsBody.lock()->addImpulse(glm::normalize(directionVector) * 10.0f);
			}
		}
	}
}

void ComponentController::TryJump(bool keydown) {
	if (_isGameOver) {
		Reset();
		return;
	}
	if (!keydown) {
		_hasReleasedJumpKey = true;
		return;
	}
	if (!_hasReleasedJumpKey) {
		return;
	}

	if (_footContact.lock()->CanJump()) {
		_physicsBody.lock()->addImpulse(glm::vec2(0.0f, _jumpPower));
		_hasReleasedJumpKey = false;
	}
	else if (_shooter.lock()->ShootBullet(GetGameObject().lock())) {
		// If we shoot, then add upwards force relative to vertical velocity
		float impulsePower = 2.f;
		float verticalVelocity = _physicsBody.lock()->getLinearVelocity().y;
		if (verticalVelocity < -2.f) {
			impulsePower -= verticalVelocity;
		}
		else {
			impulsePower = glm::max(0.2f, impulsePower - verticalVelocity);
		}

		_physicsBody.lock()->addImpulse({ 0.f, impulsePower });
		_hasReleasedJumpKey = false;
	}
}

void ComponentController::GameOver() {
	_isGameOver = true;
	auto gameOverText = _engine->CreateGameObject("GameOverText", GetGameObject()).lock();
	auto renderer = gameOverText->CreateComponent<ComponentRendererSprite>().lock();
	renderer->SetSprite("bird", "game-over.png");
	_movRight = false;
	_movLeft = false;
	auto sprite = renderer->GetSprite();
	_physicsBody.lock()->SetGravityScale(0.f);

	glm::vec3 pos = glm::vec3(
		0.f,
		GetGameObject().lock()->GetPosition().y+200,
		1.0f);

	gameOverText->SetPosition(pos);
}

void ComponentController::UpdateSprite(float deltaTime, glm::vec2 velocity) {
	if (velocity.x < 0.0f) {
		_spriteRenderer.lock()->SetFlipped(true);
	}
	else if (velocity.x > 0.0f) {
		_spriteRenderer.lock()->SetFlipped(false);
	}

	if (_footContact.lock()->IsGrounded()) {
		if (velocity.x == 0.0f) {
			// Standing
			_spriteRenderer.lock()->SetSprite("platformer-art-deluxe", "19.png");
		}
		else {
			// Walking
			_walkAnimationDeltaTime += deltaTime;
			if (_walkAnimationDeltaTime > _walkAnimationSpeed) {
				_walkAnimationFrameChange = !_walkAnimationFrameChange;
				_walkAnimationDeltaTime = 0.0f;
			}

			if (_walkAnimationFrameChange) {
				_spriteRenderer.lock()->SetSprite("platformer-art-deluxe", "20.png");
			}
			else {
				_spriteRenderer.lock()->SetSprite("platformer-art-deluxe", "21.png");
			}
		}
	}
	else {
		if (velocity.y > _flyBorder) {
			// flyUp
			_spriteRenderer.lock()->SetSprite("platformer-art-deluxe", "26.png");
		}
		else if (velocity.y < -_flyBorder) {
			// flyDown
			_spriteRenderer.lock()->SetSprite("platformer-art-deluxe", "28.png");
		}
		else {
			// fly
			_spriteRenderer.lock()->SetSprite("platformer-art-deluxe", "27.png");
		}
	}
}

void ComponentController::Reset() {
	_isGameOver = false;
	auto level = _engine->GetGameObject("Level");
	level.lock()->FindComponent<ComponentLevel>().lock()->ResetGame();
	_healthManager.lock()->Init(4);

	_physicsBody.lock()->~ComponentPhysicsBody();
	GetGameObject().lock()->SetPosition({ 0.f,300.f, 0.f });
	_physicsBody = GetGameObject().lock()->CreateComponent<ComponentPhysicsBody>();
	glm::vec2 size = {19.f,19.f};
	_physicsBody.lock()->CreateBody(b2_dynamicBody, false, size, Circle);
	_physicsBody.lock()->SetGravityScale(1.2f);

	_shooter.lock()->ReloadMag();
	_score.lock()->Reset();
	_engine->DestroyGameObject(_engine->GetGameObject("GameOverText").lock().get());
}

bool ComponentController::IsGameOver() {
	return _isGameOver;
}