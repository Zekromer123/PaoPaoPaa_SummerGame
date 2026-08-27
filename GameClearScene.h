#pragma once
#include "Fade.h"
#include "Mathematics.h"
#include <KamataEngine.h>

class GameClearScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};
	void Initialize();
	void Update();
	void Draw();
	bool isFinished() const { return finished_; }
	~GameClearScene() { 
		delete fade_;
		delete titleModel_;
		delete subTitleModel_;
		delete camera_;
	}

private:
	bool finished_ = false;
	
	Fade* fade_ = nullptr;
	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::Model* subTitleModel_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::WorldTransform worldTransformTitle_;
	KamataEngine::WorldTransform worldTransformSubTitle_;
	float animationTimer_ = 0.0f;
	Phase phase_ = Phase::kFadeIn;
};
