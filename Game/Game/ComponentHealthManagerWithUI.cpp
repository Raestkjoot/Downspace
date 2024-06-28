#include "ComponentHealthManagerWithUI.h"

#include "Engine/MyEngine.h"
#include "ComponentObjectCountUI.h"


void ComponentHealthManagerWithUI::Init(int health) {
	ComponentHealthManager::Init(health);

	if (_objectCountUI != nullptr) {
		_objectCountUI->Reset();
	}
}

void ComponentHealthManagerWithUI::TakeDamage(int damage) {
	ComponentHealthManager::TakeDamage(damage);

	if (_objectCountUI != nullptr) {
		for (int i = 0; i < damage; ++i) {
			_objectCountUI->Decrement();
		}
	}
}

int ComponentHealthManagerWithUI::SetUIComponent(ComponentObjectCountUI* objectCountUI) {
	_objectCountUI = objectCountUI;
	return _health;
}