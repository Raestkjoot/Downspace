#pragma once

#include "Engine/Component.h"
#include "Engine/Components/ComponentRendererSprite.h"

#include "ComponentAmmunition.h"

class ComponentShooter : public MyEngine::Component {
public:
	void Init();
	bool ShootBullet(std::weak_ptr<MyEngine::GameObject> gameObject);
	void ReloadMag();

private:
	std::weak_ptr<MyEngine::GameObject> _gameObject;
	int _count;
	std::weak_ptr<ComponentAmmunition> _ammunition;
	MyEngine::Engine* _engine;
};
