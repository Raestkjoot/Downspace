#pragma once

#include "Engine/Component.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "ComponentShooter.h"
#include "ComponentHealthManagerWithUI.h"
#include "ComponentScore.h"
#include "ComponentFootContact.h"
#include "ComponentSideContact.h"

class ComponentController : public MyEngine::Component {
public:
	bool IsGameOver();

private:
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;
	void KeyEvent(SDL_Event&) override;
	void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) override;

	void TryJump(bool keydown);
	void GameOver();
	void Reset();
	void UpdateSprite(float deltaTime, glm::vec2 velocity);

private:
	float _movSpeed;
	float _jumpPower;

	bool _isGameOver = false;
	bool _movLeft = false;
	bool _movRight = false;
	bool _hasReleasedJumpKey = true;

	float _flyBorder = 2.0f;
	float _walkAnimationDeltaTime = 0.0f;
	float _walkAnimationSpeed = 0.15f;
	bool _walkAnimationFrameChange = false;
	float _maxVerticalSpeedUp = 4.0f;
	float _maxVerticalSpeedDown = 20.0f;

	MyEngine::Engine* _engine;

	std::weak_ptr<ComponentPhysicsBody> _physicsBody;
	std::weak_ptr<ComponentRendererSprite> _spriteRenderer;
	std::weak_ptr<ComponentShooter> _shooter;
	std::weak_ptr<ComponentScore> _score;

	std::weak_ptr<ComponentHealthManagerWithUI> _healthManager;

	std::weak_ptr<ComponentFootContact> _footContact;
	std::weak_ptr<ComponentSideContact> _sideContact;
};