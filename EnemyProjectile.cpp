#include "EnemyProjectile.h"
using namespace KamataEngine;

void EnemyProjectile::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	model_ = model;
	camera_ = camera;
	velocity_ = velocity;
	isDead_ = false;
	lifeTimer_ = 180;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {radius_, radius_, radius_};
}

void EnemyProjectile::Update() {
	if (isDead_)return;

	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.rotation_.x += 10.0f;
	worldTransform_.matWorld_ = Mathematics::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	
	
	if (--lifeTimer_ <= 0) {
		isDead_ = true;
	}
}
void EnemyProjectile::Draw() {
	if (isDead_)
		return;
	if (model_ && camera_) {
		model_->Draw(worldTransform_, *camera_);
	}
}
AABB EnemyProjectile::GetAABB() const {
	AABB aabb;
	aabb.min = {worldTransform_.translation_.x - radius_, worldTransform_.translation_.y - radius_, worldTransform_.translation_.z - radius_};
	aabb.max = {worldTransform_.translation_.x + radius_, worldTransform_.translation_.y + radius_, worldTransform_.translation_.z + radius_};
	return aabb;
}