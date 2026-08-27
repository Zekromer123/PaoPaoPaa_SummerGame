#pragma once
#include <KamataEngine.h>
class Skydome {
public:
	~Skydome();
	void Initialize();
	void Update();
	void Draw(KamataEngine::Camera& camera);
	

private:
	KamataEngine::WorldTransform mWorldTransform;
	KamataEngine::Model* mModel = nullptr;
};
