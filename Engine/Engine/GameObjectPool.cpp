#include "GameObjectPool.h"

#include <cstdlib>
#include <map>

#include "Logger.h"
#include "MyEngine.h"

namespace MyEngine {
	void GameObjectPool::CreatePool(InitObjectFunc initObjectFunc, 
		const std::string& name, int poolSize, std::weak_ptr<MyEngine::GameObject> parent) {

		_initObjectFunc = initObjectFunc;
		_baseName = name;
		_parent = parent;

		for (int i = 0; i < poolSize; ++i) {
			CreatePoolObject();
		}
	}

	std::weak_ptr<GameObject> GameObjectPool::Get() {
		if (_pool.empty()) {
			Logger::Log("Warning: Empty GameObjectPool! Created new object.\n");
			CreatePoolObject();
		}

		auto iter = _pool.begin();
		auto go = iter->second.lock();
		_pool.erase(iter);
		go->SetActive(true);

		return go;
	}

	void GameObjectPool::Release(std::weak_ptr<GameObject> gameObject) {
		_pool[gameObject.lock()->GetName()] = gameObject;
		gameObject.lock()->SetActive(false);
	}

	void GameObjectPool::CreatePoolObject() {
		MyEngine::Engine* engine = MyEngine::Engine::GetInstance();
		auto go = engine->CreateGameObject(_baseName + std::to_string(_objectsCount), _parent);

		_initObjectFunc(go);
		go.lock()->SetActive(false);
		_pool[go.lock()->GetName()] = go;

		_objectsCount++;
	}
}