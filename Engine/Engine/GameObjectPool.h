#pragma once

#include <list>
#include <memory>

#include "GameObject.h"

typedef void(*InitObjectFunc)(std::weak_ptr<MyEngine::GameObject>);

namespace MyEngine
{
	class GameObjectPool {
	public:
		void CreatePool(InitObjectFunc initObjectFunc, 
						const std::string& name, 
						int poolSize, 
						std::weak_ptr<MyEngine::GameObject> parent);

		std::weak_ptr<GameObject> Get();
		void Release(std::weak_ptr<GameObject> gameObject);

	private:
		void CreatePoolObject();

		std::map<std::string, std::weak_ptr<GameObject>> _pool;

		InitObjectFunc _initObjectFunc;
		std::string _baseName;
		int _objectsCount;
		std::weak_ptr<MyEngine::GameObject> _parent;
	};
}