#include "ComponentLevel.h"

#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "Engine/Components/ComponentPhysicsBody.h"

#include "ComponentLevelSubsection.h"

void ComponentLevel::Init(rapidjson::Value&) {
	_engine = MyEngine::Engine::GetInstance();
	_gameObject = GetGameObject();
	_player = _engine->GetGameObject("Player");

	CreateFirstSection();
	for (int i = 1; i < NUM_OF_SECTIONS; ++i) {
		CreateSection(i);
	}
}

void ComponentLevel::Update(float deltaTime) {
	float dist = _player.lock()->GetPosition().y - _sections[_iTopSection].lock()->GetPosition().y;

	if (dist < _distThreshold) {
		glm::vec3 pos = _sections[_iTopSection].lock()->GetPosition();
		pos.y -= _distBetweenSections * NUM_OF_SECTIONS;
		MoveSection(_iTopSection, pos);
		_iTopSection = (_iTopSection + 1) % NUM_OF_SECTIONS;
	}
}

void ComponentLevel::CreateFirstSection() {
	auto section = _engine->CreateGameObject("0_LevelSubsection", GetGameObject());

	glm::vec2 winSize = _engine->GetScreenSize();
	section.lock()->SetPosition({ _halfBlockSize - (winSize.x / 2.f), _firstSectionPos, 0.f });

	auto sectionComponent = section.lock()->CreateComponent<ComponentLevelSubsection>();
	sectionComponent.lock()->InitializeSection(_firstSectionLayout, &_blockPool, &_destructibleBlockPool, GetGameObject());

	_sections[0] = section;
}

void ComponentLevel::CreateSection(int i) {
	auto section = _engine->CreateGameObject(std::to_string(i) + "_LevelSubsection", GetGameObject());

	glm::vec2 winSize = _engine->GetScreenSize();
	float yPos = _firstSectionPos - static_cast<float>(i) * _distBetweenSections;
	section.lock()->SetPosition({ _halfBlockSize - (winSize.x / 2.f), yPos, 0.f });

	auto subsec = section.lock()->CreateComponent<ComponentLevelSubsection>();
	subsec.lock()->InitializeSection(GetRandomLevelLayout(), &_blockPool, &_destructibleBlockPool, GetGameObject());

	_sections[i] = section;
}

void ComponentLevel::MoveSection(int i, const glm::vec3& pos) {
	_sections[i].lock()->SetPosition(pos);
	_sections[i].lock()->FindComponent<ComponentLevelSubsection>().lock()->
		ReinitializeSection(GetRandomLevelLayout());
}

void ComponentLevel::MoveSection(int i, const glm::vec3& pos, const std::string& layout) {
	_sections[i].lock()->SetPosition(pos);
	_sections[i].lock()->FindComponent<ComponentLevelSubsection>().lock()->
		ReinitializeSection(layout);
}

const std::string& ComponentLevel::GetRandomLevelLayout() {
	return _levelLayouts[static_cast<int>(_engine->RandomRange(0.0f, 5.99f))];
}

void ComponentLevel::ResetGame() {
	glm::vec3 pos = _sections[0].lock()->GetPosition();
	_iTopSection = 0;

	float yPos = _firstSectionPos;
	pos.y = yPos;
	MoveSection(0, pos, _firstSectionLayout);

	for (int i = 1; i < NUM_OF_SECTIONS; ++i) {
		yPos -= _distBetweenSections;
		pos.y = yPos;
		MoveSection(i, pos);
	}
}