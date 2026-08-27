#pragma once
#include <KamataEngine.h>
#include <vector>
#include <memory>

class BackgroundManager {
public:
	~BackgroundManager();
	void Initialize();
	void Update();
	void Draw(KamataEngine::Camera& camera);

	private:
	// 3D Models
	KamataEngine::Model* modelTower_ = nullptr;
	KamataEngine::Model* modelWindow_ = nullptr;
	KamataEngine::Model* modelWallFloor_ = nullptr; 
	KamataEngine::Model* modelCeiling_ = nullptr;
	
	//Position
	std::vector<KamataEngine::WorldTransform*> transformTowers_;  
	std::vector<KamataEngine::WorldTransform*> transformWindows_; 
	std::vector<KamataEngine::WorldTransform*> transformWalls_;   
	std::vector<KamataEngine::WorldTransform*> transformFloors_;  
	std::vector<KamataEngine::WorldTransform*> transformCeilings_; 
};
