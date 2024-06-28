#pragma once

#include "Engine/Component.h"
#include "Engine/GameObjectPool.h"
#include "ComponentEnemy.h"

class ComponentLevelSubsection : public MyEngine::Component {
public:
	void InitializeSection(const std::string& layout,
		MyEngine::GameObjectPool* blockPool,
		MyEngine::GameObjectPool* destructibleBlockPool,
		std::weak_ptr<MyEngine::GameObject> level);
	void ReinitializeSection(const std::string& layout);

private:
	void PlaceBlocks(const std::string& layout);
	void ReleaseSection();
	static void InitBlockObject(std::weak_ptr<MyEngine::GameObject> blockObject);
	static void InitDestructibleBlockObject(std::weak_ptr<MyEngine::GameObject> blockObject);
	void PlaceBlock(int x, int y, MyEngine::GameObjectPool* pool, std::list<std::weak_ptr<MyEngine::GameObject>>&);
	void CreateEnemy(int x, int y);

	int _xBlocks = 9;
	float _blockSize = 50.0f;
	glm::vec2 _blockRenderScale = { 2.4f, 2.4f };
	std::list<std::weak_ptr<MyEngine::GameObject>> _createdBlocks;
	std::list<std::weak_ptr<MyEngine::GameObject>> _createdDestructibleBlocks;
	std::list<std::weak_ptr<MyEngine::GameObject>> _createdEnemies;

	MyEngine::Engine* _engine;
	MyEngine::GameObjectPool* _blockPool;
	MyEngine::GameObjectPool* _destructibleBlockPool;
};