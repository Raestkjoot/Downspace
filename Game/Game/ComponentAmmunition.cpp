#include "ComponentAmmunition.h"


void ComponentAmmunition::Init() {
	_ammunition = _maxAmmunition;
}

void ComponentAmmunition::ConsumeAmmunition() {
	_ammunition--;
}

int ComponentAmmunition::GetAmmunition() {
	return _ammunition;
}

void ComponentAmmunition::Reload() {
	_ammunition = _maxAmmunition;
}