#pragma once

#include "Engine/Component.h"


class ComponentAmmunition : public MyEngine::Component {
public:
	void Init();
	void ConsumeAmmunition();
	int GetAmmunition();
	void Reload();

private:
	int _ammunition;
	int _maxAmmunition = 8;
};