#pragma once

#include "Engine/Component.h"

#include <Box2D/Box2D.h>
#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/Shapes/b2Shape.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Dynamics/b2Body.h"

enum ShapeType {
	Box = 0,
	Circle = 1
};

class ComponentPhysicsBody : public MyEngine::Component {
	friend class MyEngine::Engine; // TODO get rid of this

public:
	~ComponentPhysicsBody();

	virtual void Init(rapidjson::Value& serializedData) override;

	void CreateBody(b2BodyType bodyType, bool isSensor, glm::vec2 size, ShapeType shapeType = ShapeType::Box);
	// Force gradually affects the velocity over time
	void addForce(glm::vec2 force);
	// Instantly affects velocity
	void addImpulse(glm::vec2 impulse);
	void setLinearVelocity(glm::vec2 velocity);
	glm::vec2 getLinearVelocity();
	void SetGravityScale(float scale);
	void SetActive(bool isActive);
	void SetStaticBodyPosition(glm::vec2);

private:
	b2World* _world;
	b2Body* _body = nullptr;
	b2Fixture* _fixture = nullptr;
	b2Shape* _shape = nullptr;

	// TODO move to deserialization library (as DeserializeVector2)
	static glm::vec2 DeserializeVector2(rapidjson::Value& vectorData);
};