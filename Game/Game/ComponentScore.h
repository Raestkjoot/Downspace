#pragma once

#include "imgui.h"
#include "Engine/Component.h"

class ComponentScore : public MyEngine::Component {
public:
	void AddPoints(int value);
	void Reset();
	void ResetScore();
	void ResetCombo();
	void IncreaseCombo();
	int GetCombo();

private:
	void Render(sre::SpriteBatch::SpriteBatchBuilder& builder) override;
	
	int _currentScore = 0;
	int _highScore = 0;
	int _combo = 0;
	ImVec2 _windowSize = ImVec2(120, 150);
};