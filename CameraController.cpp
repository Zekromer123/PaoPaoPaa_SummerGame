#include "CameraController.h"
#include "Player.h"
#include <algorithm>
using namespace KamataEngine;
void CameraController::Initialize(Player* target) {
	target_ = target;
	cameraController_.Initialize();
	cameraController_.farZ = 1000.0f;
}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const KamataEngine::Vector3& targetVelocity = target_->GetVelocity();
	Vector3 targetPos = {
	    targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias, targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias,
	    targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias};
	// Lerp / Interpolation
	cameraController_.translation_ = Lerp(cameraController_.translation_, targetPos, kInterpolationRate);

	// Moveable Area
	cameraController_.translation_.x = std::clamp(cameraController_.translation_.x, movableArea_.left, movableArea_.right);
	cameraController_.translation_.y = std::clamp(cameraController_.translation_.y, movableArea_.bottom, movableArea_.top);

	// DeadZone
	cameraController_.translation_.x =
	    std::clamp(cameraController_.translation_.x, targetWorldTransform.translation_.x + targetOffset_.x + margin_.left, targetWorldTransform.translation_.x + targetOffset_.x + margin_.right);
	cameraController_.translation_.y =
	    std::clamp(cameraController_.translation_.y, targetWorldTransform.translation_.y + targetOffset_.y + margin_.bottom, targetWorldTransform.translation_.y + targetOffset_.y + margin_.top);

	cameraController_.UpdateMatrix();
}

void CameraController::Reset() {
	if (!target_) {
		return;
	}
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	cameraController_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	cameraController_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	cameraController_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}

KamataEngine::Vector3 CameraController::Lerp(const Vector3& v1, const Vector3& v2, float t) { return {v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t}; }
