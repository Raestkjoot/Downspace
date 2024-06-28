#include "ComponentLevelSubsection.h"

#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "ComponentDestructibleBlock.h"

void ComponentLevelSubsection::InitializeSection(const std::string& layout,
	MyEngine::GameObjectPool* blockPool,
	MyEngine::GameObjectPool* destructibleBlockPool,
	std::weak_ptr<MyEngine::GameObject> level) {

	_engine = MyEngine::Engine::GetInstance();

	// Initialize pools
	_blockPool = blockPool;
	_destructibleBlockPool = destructibleBlockPool;
	_blockPool->CreatePool(&InitBlockObject, "Block", 20, level);
	_destructibleBlockPool->CreatePool(&InitDestructibleBlockObject, "BlockDestructable", 20, level);

	PlaceBlocks(layout);
}

void ComponentLevelSubsection::ReinitializeSection(const std::string& layout) {
	ReleaseSection();
	PlaceBlocks(layout);
}

void ComponentLevelSubsection::PlaceBlocks(const std::string& layout) {
	for (std::string::size_type i = 0; i < layout.size(); ++i) {
		int x = i % _xBlocks;
		int y = i / _xBlocks;

		switch (layout[i])
		{
		case '#':
			PlaceBlock(x, y, _blockPool, _createdBlocks);
			break;
		case 'X':
			PlaceBlock(x, y, _destructibleBlockPool, _createdDestructibleBlocks);
			break;
		case 'E':
			CreateEnemy(x, y);
			break;
		}
	}
}

void ComponentLevelSubsection::ReleaseSection() {
	for (auto block : _createdBlocks) {
		_blockPool->Release(block.lock());
	}
	
	for (auto block : _createdDestructibleBlocks) {
		_destructibleBlockPool->Release(block.lock());
	}
	for (auto enemy : _createdEnemies)
	{
		if (!enemy.expired()) {
			enemy.lock()->FindComponent<ComponentEnemy>().lock()->Destroy();
		}
	}

	_createdBlocks.clear();
	_createdDestructibleBlocks.clear();
	_createdEnemies.clear();
}

void ComponentLevelSubsection::InitBlockObject(std::weak_ptr<MyEngine::GameObject> blockObject) {
	auto renderer = blockObject.lock()->CreateComponent<ComponentRendererSprite>().lock();
	renderer->SetSprite("platformer-art-deluxe", "303.png");
	renderer->SetScale({ 2.4f, 2.4f });

	auto body = blockObject.lock()->CreateComponent<ComponentPhysicsBody>().lock();
	glm::vec2 size = renderer->GetSize() / 2.0f;
	body->CreateBody(b2_staticBody, false, size);
}

void ComponentLevelSubsection::InitDestructibleBlockObject(std::weak_ptr<MyEngine::GameObject> blockObject) {
	auto renderer = blockObject.lock()->CreateComponent<ComponentRendererSprite>().lock();
	renderer->SetSprite("platformer-art-deluxe", "191.png");
	renderer->SetScale({ 2.4f, 2.4f });

	auto body = blockObject.lock()->CreateComponent<ComponentPhysicsBody>().lock();
	glm::vec2 size = renderer->GetSize() / 2.0f;
	body->CreateBody(b2_staticBody, false, size);

	blockObject.lock()->CreateComponent<ComponentDestructibleBlock>();
}

void ComponentLevelSubsection::PlaceBlock(int x, int y, MyEngine::GameObjectPool* pool, std::list<std::weak_ptr<MyEngine::GameObject>>& createdBlocks) {
	auto block = pool->Get();

	glm::vec3 pos = GetGameObject().lock()->GetPosition();
	pos.x += x * _blockSize;
	pos.y -= y * _blockSize;

	block.lock()->SetPosition(pos);
	glm::vec2 bodyPos = { pos.x, pos.y };
	block.lock()->FindComponent<ComponentPhysicsBody>().lock()->SetStaticBodyPosition(bodyPos);

	createdBlocks.push_back(block);
}

void ComponentLevelSubsection::CreateEnemy(int x, int y) {
	glm::vec3 pos = GetGameObject().lock()->GetPosition();
	pos.x += x * _blockSize;
	pos.y -= y * _blockSize;

	std::string name = "EnemyObject";
	auto num = std::to_string(_engine->RandomRange(0.f, 100000.f));
	name += num;

	auto enemyObject = _engine->CreateGameObject(name, GetGameObject()).lock();
	enemyObject->SetPosition(pos);
	auto renderer = enemyObject->CreateComponent<ComponentRendererSprite>().lock();
	renderer->SetSprite("platformer-art-deluxe", "74.png");

	auto body = enemyObject->CreateComponent<ComponentPhysicsBody>().lock();
	glm::vec2 size = renderer->GetSize();
	body->CreateBody(b2_dynamicBody, false, size, Box);
	auto enemy = enemyObject->CreateComponent<ComponentEnemy>().lock();
	enemy->CreateEnemy(0.5f, "Player", true, 400.f);
	_createdEnemies.push_back(enemyObject);
}
