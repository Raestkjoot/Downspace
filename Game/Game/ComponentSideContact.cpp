#include "ComponentSideContact.h"

#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentPhysicsBody.h"

void ComponentSideContact::OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (!other->GetGameObject().lock()->GetName().rfind("Block", 0) == 0 ||
		!(manifold->localNormal.y < _threshold && manifold->localNormal.y > -_threshold)) {
		return;
	}

	auto contact = std::shared_ptr<MyEngine::GameObject>(other->GetGameObject());
	_contacts.push_back(contact);

	_isTouchingSide = true;
}

void ComponentSideContact::OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (!other->GetGameObject().lock()->GetName().rfind("Block", 0) == 0) {
		// Sometimes the start collision normal is different from the end collision normal
		// So we ignore the normal check here
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
		_isTouchingSide = false;
	}
}

bool ComponentSideContact::CanMoveHorizontal(float xPos, float movement) {
	// Check if blocked by edge of screen
	float screenEdgeOffset = MyEngine::Engine::GetInstance()->GetScreenSize().x / 2.0f;
	screenEdgeOffset -= 15.f;
	if (xPos >= screenEdgeOffset && movement > 0.f) {
		return false;
	}
	else if (xPos <= -screenEdgeOffset && movement < 0.f) {
		return false;
	}

	// Check if blocked by blocks
	if (!_isTouchingSide) {
		return true;
	}

	bool movementBlocked = false;

	auto c_iter = _contacts.begin();
	while (c_iter != _contacts.end()) {
		if (c_iter->expired()) {
			c_iter = _contacts.erase(c_iter);
			continue;
		}

		float contactPos = c_iter->lock()->GetPosition().x;
		float xDiff = xPos - contactPos;
		// true for right blocked, false for left blocked
		bool blockedSide = xDiff < 0.0f;

		if (blockedSide) {
			// right side blocked, can't move if moving right
			movementBlocked = movement > 0.f;
		}
		else {
			// left side blocked, can't move if moving left
			movementBlocked = movement < 0.f;
		}

		break;
	}

	return !movementBlocked;
}
