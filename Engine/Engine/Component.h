#pragma once

#include <memory>

#include "sre/RenderPass.hpp"
#include "rapidjson/document.h"
#include "Box2D/Box2D.h"

#include "Engine/GameObject.h"

class ComponentPysicsComponent;

namespace MyEngine {
	class Engine;

	class Component {
		friend class GameObject;

	public:
		virtual void Init(rapidjson::Value& serializedData) {};
		virtual void Update(float) {};
		virtual void Render(sre::SpriteBatch::SpriteBatchBuilder& builder) {};
		virtual void KeyEvent(SDL_Event&) {};
		virtual void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {};
		virtual void OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) {};
		virtual void OnEnabled() {};
		virtual void OnDisabled() {};

		std::weak_ptr<GameObject> GetGameObject();

	private:
		std::weak_ptr<GameObject> _gameObject;

		void SetGameObject(std::weak_ptr<GameObject>);
	};
}