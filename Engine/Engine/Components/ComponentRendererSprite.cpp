#include "ComponentRendererSprite.h"

#include "Engine/MyEngine.h"

#include "Engine/ResourceManager.h"

void ComponentRendererSprite::Init(rapidjson::Value& serializedData) {
	SetSprite(
		serializedData["atlas"].GetString(),
		serializedData["sprite"].GetString()
	);

	auto gameObject = GetGameObject().lock();
	if (gameObject) {
		glm::vec3 goScale = gameObject->GetScale();
		SetScale({ goScale.x, goScale.y });
	}
}

void ComponentRendererSprite::SetSprite(std::string idSprite) {
	auto tex = sre::Texture::create().withFile("data/" + idSprite + ".png")
		.withFilterSampling(false)
		.build();
	auto atlas = sre::SpriteAtlas::createSingleSprite(tex, idSprite, glm::vec2(0.5f, 0.5f));
	MyEngine::ResourceManager::RegisterAtlas("SINGLE", atlas);

	_atlas = MyEngine::ResourceManager::GetAtlas("SINGLE");
	_sprite = _atlas->get(idSprite);
	UpdateSprite();
}

void ComponentRendererSprite::SetSprite(std::string idAtlas, std::string idSprite) {
	_atlas = MyEngine::ResourceManager::GetAtlas(idAtlas);
	_sprite = _atlas->get(idSprite);
	UpdateSprite();
}

void ComponentRendererSprite::Render(sre::SpriteBatch::SpriteBatchBuilder& builder) {
	if (!_isActive) return;
	auto gameObject = GetGameObject().lock();
	if (!gameObject) return;

	glm::vec3 gameObjectPos = gameObject->GetPosition();
	_sprite.setPosition({ gameObjectPos.x + _offset.x, gameObjectPos.y + _offset.y});
	builder.addSprite(_sprite);
}

glm::vec2 ComponentRendererSprite::GetSize() {
	glm::vec2 size = _sprite.getSpriteSize();
	return size * _scale;
}

void ComponentRendererSprite::SetScale(const glm::vec2& scale) {
	_scale = scale;
	_sprite.setScale(scale);
}

void ComponentRendererSprite::SetFlipped(bool isFlipped) {
	if (_isFlipped == isFlipped) return;

	_isFlipped = isFlipped;
	_sprite.setFlip(glm::bvec2(isFlipped, false));
}

void ComponentRendererSprite::SetActive(bool isActive) {
	_isActive = isActive;
}

void ComponentRendererSprite::SetOffset(glm::vec2 offset) {
	_offset = offset;
}

void ComponentRendererSprite::SetSortingOrder(uint16_t sortOrder) {
	_sortOrder = sortOrder;
	_sprite.setOrderInBatch(sortOrder);
}

void ComponentRendererSprite::UpdateSprite() {
	_sprite.setFlip(glm::bvec2(_isFlipped, false));
	_sprite.setScale(_scale);
	_sprite.setOrderInBatch(_sortOrder);
}