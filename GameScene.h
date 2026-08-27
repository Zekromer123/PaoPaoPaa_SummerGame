#pragma once
#include "3d/DebugCamera.h"
#include "BackgroundManager.h"
#include "CameraController.h"
#include "Enemy.h"
#include "EnemyProjectile.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Mathematics.h"
#include "Player.h"
#include "Skydome.h"

#include <memory>
#include <vector>

class GameScene {
public:
	GameScene() = default;
	~GameScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return isFinished_; }
	bool IsGameOver() const { return isGameOver_; }
	bool IsGameClear() const { return isGameClear_; }

private:
	void CheckAllCollisions();
	void GenerateFieldObjects();

private:
	// Player
	std::unique_ptr<Player> player_;
	KamataEngine::Model* modelBody_ = nullptr;
	KamataEngine::Model* modelHead_ = nullptr;
	KamataEngine::Model* modelLeftArm_ = nullptr;
	KamataEngine::Model* modelLeftForearm_ = nullptr;
	KamataEngine::Model* modelRightArm_ = nullptr;
	KamataEngine::Model* modelRightForearm_ = nullptr;
	KamataEngine::Model* modelLeftThigh_ = nullptr;
	KamataEngine::Model* modelLeftCalf_ = nullptr;
	KamataEngine::Model* modelRightThigh_ = nullptr;
	KamataEngine::Model* modelRightCalf_ = nullptr;
	uint32_t texHpBar_ = 0;
	uint32_t texHpFillGreen_ = 0;
	uint32_t texHpFillYellow_ = 0;
	uint32_t texHpFillRed_ = 0;
	uint32_t texHpIcon_ = 0;
	uint32_t attackUI_ = 0;
	KamataEngine::Sprite* spriteHpBar_ = nullptr;  
	KamataEngine::Sprite* spriteHpFillGreen_ = nullptr; 
	KamataEngine::Sprite* spriteHpFillYellow_ = nullptr; 
	KamataEngine::Sprite* spriteHpFillRed_ = nullptr; 
	KamataEngine::Sprite* spriteHpIcon_ = nullptr; 
	KamataEngine::Sprite* spriteAttackUI_ = nullptr; 
	KamataEngine::Vector2 hpFillBaseSize_{};       

	// Enemy
	std::vector<std::unique_ptr<Enemy>> enemies_;
	std::vector<std::unique_ptr<EnemyProjectile>> projectiles_;
	KamataEngine::Model* projectileModel_ = nullptr;
	KamataEngine::Model* modelMeleeBody_ = nullptr;
	KamataEngine::Model* modelMeleeHead_ = nullptr;
	KamataEngine::Model* modelMeleeLeftArm_ = nullptr;
	KamataEngine::Model* modelMeleeLeftForearm_ = nullptr;
	KamataEngine::Model* modelMeleeRightArm_ = nullptr;
	KamataEngine::Model* modelMeleeRightForearm_ = nullptr;
	KamataEngine::Model* modelMeleeLeftThigh_ = nullptr;
	KamataEngine::Model* modelMeleeLeftCalf_ = nullptr;
	KamataEngine::Model* modelMeleeRightThigh_ = nullptr;
	KamataEngine::Model* modelMeleeRightCalf_ = nullptr;
	//Range
	KamataEngine::Model* modelRangeBody_ = nullptr;
	KamataEngine::Model* modelRangeHead_ = nullptr;
	KamataEngine::Model* modelRangeLeftArm_ = nullptr;
	KamataEngine::Model* modelRangeLeftForearm_ = nullptr;
	KamataEngine::Model* modelRangeRightArm_ = nullptr;
	KamataEngine::Model* modelRangeRightForearm_ = nullptr;
	KamataEngine::Model* modelRangeLeftThigh_ = nullptr;
	KamataEngine::Model* modelRangeLeftCalf_ = nullptr;
	KamataEngine::Model* modelRangeRightThigh_ = nullptr;
	KamataEngine::Model* modelRangeRightCalf_ = nullptr;
	//Boss
	KamataEngine::Model* modelBossBody_ = nullptr;
	KamataEngine::Model* modelBossHead_ = nullptr;
	KamataEngine::Model* modelBossLeftArm_ = nullptr;
	KamataEngine::Model* modelBossLeftForearm_ = nullptr;
	KamataEngine::Model* modelBossRightArm_ = nullptr;
	KamataEngine::Model* modelBossRightForearm_ = nullptr;
	KamataEngine::Model* modelBossLeftThigh_ = nullptr;
	KamataEngine::Model* modelBossLeftCalf_ = nullptr;
	KamataEngine::Model* modelBossRightThigh_ = nullptr;
	KamataEngine::Model* modelBossRightCalf_ = nullptr;


	// MapChip
	std::unique_ptr<MapChipField> mapChipField_;
	KamataEngine::Model* blockModel_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// Skydome
	std::unique_ptr<Skydome> skydome_;

	// Background
	std::unique_ptr<BackgroundManager> backgroundManager_;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	KamataEngine::Camera camera_;
	std::unique_ptr<CameraController> cameraController_;
	KamataEngine::WorldTransform* worldTransform_ = nullptr;

	 bool isFinished_ = false;
	bool isGameOver_ = false;
	bool isGameClear_ = false;
	int sceneTransitionTimer_ = 0;
};
