#pragma once

#include <vector>

#include "Engine/Component.h"
#include "Engine/GameObjectPool.h"

#define NUM_OF_SECTIONS 3

class ComponentLevel : public MyEngine::Component {
public:
	void ResetGame();

private:
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;

	void CreateFirstSection();
	void CreateSection(int i);
	void MoveSection(int i, const glm::vec3& pos);
	void MoveSection(int i, const glm::vec3& pos, const std::string& layout);
	const std::string& GetRandomLevelLayout();

	int _iTopSection = 0;
	float _firstSectionPos = 100.0f;
	float _distBetweenSections = 500.0f;
	float _halfBlockSize = 25.0f;
	float _distThreshold = -700.0f;

	MyEngine::Engine* _engine;
	std::weak_ptr<MyEngine::GameObject> _gameObject;
	std::weak_ptr<MyEngine::GameObject> _player;
	std::weak_ptr<MyEngine::GameObject> _sections[NUM_OF_SECTIONS];

	MyEngine::GameObjectPool _blockPool;
	MyEngine::GameObjectPool _destructibleBlockPool;

	std::string _firstSectionLayout =
		"XXXXXXXXX"
		"#XX0000#X"
		"00000000#";

	// TODO: Probably move this to a json
	std::vector<std::string> _levelLayouts = {
		"X00000000"
		"00E000###"
		"XXX00000#"
		"0000XX00#"
		"0E000000#",

		"00E00000#"
		"XXXXXXX0#"
		"0XX00E000"
		"00X000000"
		"000000000",

		"0000XX000"
		"00E00000#"
		"##000000#"
		"##0000E00"
		"##0000000",

		"0000E0000"
		"##XXX00X#"
		"####XX0##"
		"###XX0000"
		"##0000E00",

		"000000XXX"
		"XX000E000"
		"000000XXX"
		"0000####X"
		"E000000##",

		"000E00XXX"
		"0000000XX"
		"XX#X#0000"
		"0###00E0#"
		"00000000#",
	};
};