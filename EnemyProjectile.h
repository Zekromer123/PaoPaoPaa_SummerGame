#pragma once
#include "Mathematics.h"
#include <KamataEngine.h>
class EnemyProjectile {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);
	void Update();
	void Draw();

	bool IsDead() const { return isDead_; }
	void SetDead() { isDead_ = true; }

	AABB GetAABB() const;

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_ = {0.0f,0.0f,0.0f};
	int lifeTimer_ = 180;
	bool isDead_ = false;
	float radius_ = 0.4f;
};
