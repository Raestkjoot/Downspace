#include "ComponentFootContact.h"

#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentPhysicsBody.h"

void ComponentFootContact::OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (!other->GetGameObject().lock()->GetName().rfind("Block", 0) == 0
		|| manifold->localNormal.y < 0.5f) {
		return;
	}

	auto contact = std::shared_ptr<MyEngine::GameObject>(other->GetGameObject());
	_contacts.push_back(contact);

	if (_isGrounded == false) {
		_isGrounded = true;
		if (!_score.expired()) {
			_score.lock()->ResetCombo();
		}
		if (!_shooter.expired()) {
			_shooter.lock()->ReloadMag();
		}
	}
}

void ComponentFootContact::OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (!other->GetGameObject().lock()->GetName().rfind("Block", 0) == 0) {
		// Sometimes the end collision is more on the side than the start collision,
		// so we ignore the contact normal here
		return;
	}

	auto c_iter = _contacts.begin();
	while (c_iter != _contacts.end()) {
		if (c_iter->expired()) {
			c_iter = _contacts.erase(c_iter);
			continue;
		}
		if (c_iter->lock() == other->GetGameObject().lock()) {
			c_iter = _contacts.erase(c_iter);
			break;
		}
		c_iter++;
	}

	if (_contacts.empty()) {
		_lastGroundedTime = MyEngine::Engine::GetInstance()->GetTime();
		_isGrounded = false;
	}
}

bool ComponentFootContact::CanJump() {
	bool coyote = 
		(MyEngine::Engine::GetInstance()->GetTime() - _lastGroundedTime)
		< _coyoteTime;

	return _isGrounded || coyote;
}

bool ComponentFootContact::IsGrounded() {
	return _isGrounded;
}

void ComponentFootContact::SetShooter(std::weak_ptr<ComponentShooter> shooter) {
	_shooter = shooter;
}

void ComponentFootContact::SetScore(std::weak_ptr<ComponentScore> score) {
	_score = score;
}
