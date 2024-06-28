#pragma once

#include "Engine/Component.h"


class ComponentHealthManager : public MyEngine::Component {
public:
	virtual void Init(int health);
	virtual void TakeDamage(int damage);
	virtual int GetHealth();

protected:
	int _health;
};