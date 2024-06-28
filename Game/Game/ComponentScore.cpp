#include "ComponentScore.h"

#include "Engine/MyEngine.h"

void ComponentScore::Render(sre::SpriteBatch::SpriteBatchBuilder& builder) {
	ImGui::SetNextWindowSize(_windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - _windowSize.x, 0));
	ImGui::Begin("Scoring");
	ImGui::Text("Current Score: ");
	ImGui::Text("%i", _currentScore);
	ImGui::Text("High score: ");
	ImGui::Text("%i", _highScore);
	ImGui::Text("Combo: ");
	ImGui::Text("%i", _combo);
	ImGui::End();
}

void ComponentScore::AddPoints(int score) {
	_currentScore += score;

	if (_currentScore > _highScore) {
		_highScore = _currentScore;
	}
}

void ComponentScore::Reset() {
	ResetScore();
	ResetCombo();
}

void ComponentScore::ResetScore() {
	_currentScore = 0;
}

void ComponentScore::ResetCombo() {
	_combo = 0;
}

void ComponentScore::IncreaseCombo(){
	_combo++;
}

int ComponentScore::GetCombo() {
	return _combo;
}