#pragma once

#include "Engine/Component.h"
#include "ComponentHealthManager.h"
#include "ComponentObjectCountUI.h"


class ComponentHealthManagerWithUI : public ComponentHealthManager {
public:
	void Init(int health) override;
	void TakeDamage(int damage) override;

	int SetUIComponent(ComponentObjectCountUI* objectCountUI);

private:
	ComponentObjectCountUI* _objectCountUI = nullptr;
	int _health = 4;
};
