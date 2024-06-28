#pragma once

#include "sre/SpriteAtlas.hpp"
#include "sre/Sprite.hpp"

#include "Engine/Component.h"

class ComponentRendererSprite : public MyEngine::Component {
public:
	virtual void Init(rapidjson::Value& serializedData) override;
	virtual void Render(sre::SpriteBatch::SpriteBatchBuilder& builder) override;

	// For SINGLE image sprites. For atlas image sprites use SetSprite(idAtlas, idSprite)
	void SetSprite(std::string idSprite);
	// For ATLAS image sprites. For single image sprites use SetSprite(idSprite)
	void SetSprite(std::string idAtlas, std::string idSprite);
	sre::Sprite* GetSprite() { return &_sprite; }
	void SetSpritePosition(glm::vec2& position);

	glm::vec2 GetSize();
	void SetScale(const glm::vec2& scale);
	void SetFlipped(bool);
	void SetActive(bool);
	// Set the sprite's offset from the gameobject's position
	void SetOffset(glm::vec2);
	// Default = 0. Set higher to render in front of other sprites.
	void SetSortingOrder(uint16_t sortOrder);

protected:
	sre::Sprite _sprite;

private:
	void UpdateSprite();
	std::shared_ptr<sre::SpriteAtlas> _atlas;
	unsigned int _id = 0;

	bool _isFlipped = false;
	bool _isActive = true;
	glm::vec2 _scale = glm::vec2(1.0f, 1.0f);
	glm::vec2 _offset = glm::vec2(0.0f, 0.0f);
	uint16_t _sortOrder = 0;

};