#pragma once

#include "Engine/Component.h"
#include "ComponentShooter.h"
#include "ComponentScore.h"

class ComponentFootContact : public MyEngine::Component {
public:
	void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) override;
	void OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) override;

	bool CanJump();
	bool IsGrounded();
	void SetShooter(std::weak_ptr<ComponentShooter> shooter);
	void SetScore(std::weak_ptr<ComponentScore> score);

private:
	bool _isGrounded = false;
	bool _canJump = false;
	float _coyoteTime = 0.1f;
	float _lastGroundedTime = 0.0f;

	std::weak_ptr<ComponentShooter> _shooter;
	std::weak_ptr<ComponentScore> _score;
	std::list<std::weak_ptr<MyEngine::GameObject>> _contacts;
};