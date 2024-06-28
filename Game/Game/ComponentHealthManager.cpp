#include "ComponentHealthManager.h"

void ComponentHealthManager::Init(int health) {
	_health = health;
}

void ComponentHealthManager::TakeDamage(int damage) {
	_health -= damage;
}

int ComponentHealthManager::GetHealth() {
	return _health;
}