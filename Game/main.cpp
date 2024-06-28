#include "sre/SpriteAtlas.hpp"

#include "Engine/MyEngine.h"
#include "Engine/ComponentFactory.h"

#include "Game/ComponentController.h"
#include "Game/ComponentFollowTarget.h"
#include "Game/ComponentScore.h"
#include "Game/ComponentLevel.h"
#include "Game/ComponentEnemy.h"
#include "Game/ComponentObjectCountUI.h"

MyEngine::Engine engine;

int main() {
	MyEngine::ComponentFactory::RegisterComponentOfType("POINTSCOUNTER", []() { return std::make_shared<ComponentScore>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("CONTROLLER", []() { return std::make_shared<ComponentController>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("FOLLOW_TARGET", []() { return std::make_shared<ComponentFollowTarget>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("LEVEL", []() { return std::make_shared<ComponentLevel>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("ENEMY", []() { return std::make_shared<ComponentEnemy>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("OBJECT_COUNT_UI", []() { return std::make_shared<ComponentObjectCountUI>(); });
	
	engine.Init("data/scene.json");
}
