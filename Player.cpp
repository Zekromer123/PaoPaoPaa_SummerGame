#include "Player.h"
#include "Mathematics.h"
#include "PlayerStateAttack.h"
#include "PlayerStateDead.h"
#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include <algorithm>
#include <cassert>
#include <numbers>

using namespace KamataEngine;

void Player::Initialize(
    KamataEngine::Model* modelBody, KamataEngine::Model* modelHead, KamataEngine::Model* modelLeftArm, KamataEngine::Model* modelLeftForearm, KamataEngine::Model* modelRightArm,
    KamataEngine::Model* modelRightForearm, KamataEngine::Model* modelLeftThigh, KamataEngine::Model* modelLeftCalf, KamataEngine::Model* modelRightThigh, KamataEngine::Model* modelRightCalf,
    KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(modelBody);
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelLeftArm_ = modelLeftArm;
	modelLeftForearm_ = modelLeftForearm;
	modelRightArm_ = modelRightArm;
	modelRightForearm_ = modelRightForearm;
	modelLeftThigh_ = modelLeftThigh;
	modelLeftCalf_ = modelLeftCalf;
	modelRightThigh_ = modelRightThigh;
	modelRightCalf_ = modelRightCalf;
	camera_ = camera;

	// Initialize Body Transform
	transformBody_.Initialize(); // Parent

	transformTorso_.Initialize(); // Torso Transform (Mesh Position)
	transformHead_.Initialize();
	transformLeftArm_.Initialize();
	transformLeftForearm_.Initialize();
	transformRightArm_.Initialize();
	transformRightForearm_.Initialize();
	transformLeftThigh_.Initialize();
	transformLeftCalf_.Initialize();
	transformRightThigh_.Initialize();
	transformRightCalf_.Initialize();

	transformHead_.parent_ = &transformBody_;
	transformTorso_.parent_ = &transformBody_;
	transformLeftArm_.parent_ = &transformBody_;
	transformRightArm_.parent_ = &transformBody_;
	transformLeftForearm_.parent_ = &transformLeftArm_;
	transformRightForearm_.parent_ = &transformRightArm_;
	transformLeftThigh_.parent_ = &transformBody_;
	transformLeftCalf_.parent_ = &transformLeftThigh_;
	transformRightThigh_.parent_ = &transformBody_;
	transformRightCalf_.parent_ = &transformRightThigh_;

	// transformLeftArm_.translation_.y = 0.5f;
	// transformRightArm_.translation_.y = 0.5f;

	transformBody_.translation_ = position;

	// transformBody_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	// Body
	transformTorso_.translation_ = {0.0f, 0.37f, 0.0f};
	// Head
	transformHead_.translation_ = {0.0f, 0.84f, 0.0f};
	// Arms
	transformRightArm_.translation_ = {0.2f, 0.7f, -0.02f};
	transformLeftArm_.translation_ = {-0.2f, 0.7f, 0.0f};
	// Forearms
	transformRightForearm_.translation_ = {0.12f, -0.2f, 0.02f};
	transformLeftForearm_.translation_ = {-0.12f, -0.2f, 0.02f};
	// Thighs
	transformRightThigh_.translation_ = {0.1f, 0.17f, -0.02f};
	transformLeftThigh_.translation_ = {-0.1f, 0.17f, -0.02f};
	// Calfs
	transformRightCalf_.translation_ = {0.0f, -0.5f, -0.01f};
	transformLeftCalf_.translation_ = {0.0f, -0.5f, -0.01f};

	ChangeState(std::make_unique<PlayerStateIdle>());
}

void Player::Update() {
	if (state_) {
		state_->Update();
	}
	if (invincibleTimer_ > 0) {
		invincibleTimer_--;
	}
#ifdef _DEBUG
	DrawImGui();
#endif

	velocity_.y -= 0.03f; // Gravitation

	if (lrDirection_ == LRDirection::kRight) {
		transformBody_.rotation_.y = +std::numbers::pi_v<float> / 2.0f;
	} else {

		transformBody_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
	}

	// Collision
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;

	MapCollision(collisionMapInfo);
	NoObject(collisionMapInfo);

	velocity_ = collisionMapInfo.move;
	isGrounded_ = collisionMapInfo.floorHit;
	WhileCollision(collisionMapInfo);

	UpdateWorldTransform(transformBody_); // Parent

	UpdateWorldTransform(transformHead_); // Children
	UpdateWorldTransform(transformTorso_);
	UpdateWorldTransform(transformLeftArm_);
	UpdateWorldTransform(transformRightArm_);
	UpdateWorldTransform(transformLeftThigh_);
	UpdateWorldTransform(transformRightThigh_);

	UpdateWorldTransform(transformLeftForearm_); // 2nd Children
	UpdateWorldTransform(transformRightForearm_);
	UpdateWorldTransform(transformLeftCalf_);
	UpdateWorldTransform(transformRightCalf_);
}

void Player::Draw() {
	if (modelBody_)
		modelBody_->Draw(transformTorso_, *camera_);
	if (modelHead_)
		modelHead_->Draw(transformHead_, *camera_);
	if (modelLeftArm_)
		modelLeftArm_->Draw(transformLeftArm_, *camera_);
	if (modelRightArm_)
		modelRightArm_->Draw(transformRightArm_, *camera_);
	if (modelLeftForearm_)
		modelLeftForearm_->Draw(transformLeftForearm_, *camera_);
	if (modelRightForearm_)
		modelRightForearm_->Draw(transformRightForearm_, *camera_);
	if (modelLeftThigh_)
		modelLeftThigh_->Draw(transformLeftThigh_, *camera_);
	if (modelLeftCalf_)
		modelLeftCalf_->Draw(transformLeftCalf_, *camera_);
	if (modelRightThigh_)
		modelRightThigh_->Draw(transformRightThigh_, *camera_);
	if (modelRightCalf_)
		modelRightCalf_->Draw(transformRightCalf_, *camera_);
}
#ifdef _DEBUG
void Player::DrawImGui() {
	ImGui::Begin("3D Body Parts Transform Debugger");
	// 1. Root Parent (Body)
	if (ImGui::TreeNode("0. Body (Root Parent)")) {
		ImGui::DragFloat3("Translation", &transformTorso_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformTorso_.rotation_.x, 0.5f);
		ImGui::DragFloat3("Scale", &transformTorso_.scale_.x, 0.01f);
		ImGui::TreePop();
	}
	// 2. Head
	if (ImGui::TreeNode("1. Head")) {
		ImGui::DragFloat3("Translation", &transformHead_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformHead_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	// 3. Lengan Kiri (Arm & Forearm)
	if (ImGui::TreeNode("2. Left Arm (Bahu Kiri)")) {
		ImGui::DragFloat3("Translation", &transformLeftArm_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformLeftArm_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("3. Left Forearm (Siku Kiri)")) {
		ImGui::DragFloat3("Translation", &transformLeftForearm_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformLeftForearm_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	// 4. Lengan Kanan (Arm & Forearm)
	if (ImGui::TreeNode("4. Right Arm (Bahu Kanan)")) {
		ImGui::DragFloat3("Translation", &transformRightArm_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformRightArm_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("5. Right Forearm (Siku Kanan)")) {
		ImGui::DragFloat3("Translation", &transformRightForearm_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformRightForearm_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	// 5. Kaki Kiri (Thigh & Calf)
	if (ImGui::TreeNode("6. Left Thigh (Paha Kiri)")) {
		ImGui::DragFloat3("Translation", &transformLeftThigh_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformLeftThigh_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("7. Left Calf (Betis Kiri)")) {
		ImGui::DragFloat3("Translation", &transformLeftCalf_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformLeftCalf_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	// 6. Kaki Kanan (Thigh & Calf)
	if (ImGui::TreeNode("8. Right Thigh (Paha Kanan)")) {
		ImGui::DragFloat3("Translation", &transformRightThigh_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformRightThigh_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("9. Right Calf (Betis Kanan)")) {
		ImGui::DragFloat3("Translation", &transformRightCalf_.translation_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transformRightCalf_.rotation_.x, 0.5f);
		ImGui::TreePop();
	}
	ImGui::End();
}
#endif
void Player::ChangeState(std::unique_ptr<BasePlayerState> newState) {
	state_ = std::move(newState);
	if (state_) {
		state_->Initialize(this);
	}
}

void Player::WhileCollision(const CollisionMapInfo& info) {
	if (info.ceilingHit) {
		velocity_.y = 0.0f;
	}
	if (info.wallHit) {
		velocity_.x = 0.0f;
	}
	if (info.floorHit) {
		isGrounded_ = true;
		velocity_.x *= (1.0f - kAttenuationLanding);
		velocity_.y = 0.0f;
	}
}

int Player::GetAttackDamage() const {
	auto attackState = dynamic_cast<PlayerStateAttack*>(state_.get());
	if (!attackState)
		return 0;
	if (attackState->GetAttackType() == AttackType::kLight) {

		return 25;
	} else {

		return 50;
	}
}

void Player::TakeDamage(int damage, const KamataEngine::Vector3& knockback) {
	if (hp_ <= 0 || invincibleTimer_ > 0) {
		return;
	}

	if (state_ && dynamic_cast<PlayerStateGuard*>(state_.get())) { // Check if the player is in guard state or not
		invincibleTimer_ = 30;
		velocity_.x = knockback.x * 0.25f;
		return;
	}

	hp_ -= damage;
	invincibleTimer_ = 60;
	velocity_ = knockback;
	if (hp_ <= 0) {
		hp_ = 0;
		ChangeState(std::make_unique<PlayerStateDead>());
	}
}

void Player::SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

Vector3 Player::GetWorldPosition() const {
	Vector3 worldPos;
	worldPos.x = transformBody_.matWorld_.m[3][0];
	worldPos.y = transformBody_.matWorld_.m[3][1];
	worldPos.z = transformBody_.matWorld_.m[3][2];
	return worldPos;
}

AABB Player::GetAABB() const {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Player::NoObject(const CollisionMapInfo& info) {
	transformBody_.translation_.x += info.move.x;
	transformBody_.translation_.y += info.move.y;
	transformBody_.translation_.z += info.move.z;
}

void Player::MapCollision(CollisionMapInfo& info) {
	MapCollisionUp(info);
	MapCollisionDown(info);
	MapCollisionRight(info);
	MapCollisionLeft(info);
}

void Player::MapCollisionUp(CollisionMapInfo& info) {
	if (info.move.y <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transformBody_.translation_ + info.move, static_cast<Corner>(i));
	}
	bool hit = false;
	IndexSet indexSet = {0, 0};
	IndexSet indexTopRight = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	IndexSet indexTopLeft = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	if (mapChipField_->GetMapChipTypeByIndex(indexTopRight.xIndex, indexTopRight.yIndex) == MapChipType::kBlock) {
		indexSet = indexTopRight;
		hit = true;

	} else if (mapChipField_->GetMapChipTypeByIndex(indexTopLeft.xIndex, indexTopLeft.yIndex) == MapChipType::kBlock) {
		indexSet = indexTopLeft;
		hit = true;
	}
	if (hit) {
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float playerTop = transformBody_.translation_.y + kHeight / 2.0f;
		info.move.y = rect.bottom - playerTop - 0.01f;
		info.ceilingHit = true;
	}
}

void Player::MapCollisionDown(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transformBody_.translation_ + info.move, static_cast<Corner>(i));
	}

	bool hit = false;
	IndexSet indexSet = {0, 0};
	IndexSet indexBottomLeft = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	MapChipType typeLeft = mapChipField_->GetMapChipTypeByIndex(indexBottomLeft.xIndex, indexBottomLeft.yIndex);
	MapChipType typeLeftNext = mapChipField_->GetMapChipTypeByIndex(indexBottomLeft.xIndex, indexBottomLeft.yIndex - 1);

	if (typeLeft == MapChipType::kBlock && typeLeftNext != MapChipType::kBlock) {
		indexSet = indexBottomLeft;
		hit = true;
	} else {

		IndexSet indexBottomRight = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		MapChipType typeRight = mapChipField_->GetMapChipTypeByIndex(indexBottomRight.xIndex, indexBottomRight.yIndex);
		MapChipType typeRightNext = mapChipField_->GetMapChipTypeByIndex(indexBottomRight.xIndex, indexBottomRight.yIndex - 1);

		if (typeRight == MapChipType::kBlock && typeRightNext != MapChipType::kBlock) {
			indexSet = indexBottomRight;
			hit = true;
		}
	}

	if (hit) {
		Vector3 bottomBefore = {transformBody_.translation_.x, transformBody_.translation_.y - (kHeight / 2.0f), 0.0f};
		IndexSet indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(bottomBefore);

		if (indexSetNow.yIndex != indexSet.yIndex) {
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			float playerBottom = transformBody_.translation_.y - (kHeight / 2.0f);
			info.move.y = rect.top - playerBottom;
			info.floorHit = true;
		}
	}
}

void Player::MapCollisionRight(CollisionMapInfo& info) {
	if (info.move.x <= 0.0f) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transformBody_.translation_ + info.move, static_cast<Corner>(i));
	}

	// Margin
	Vector3 posTop = positionsNew[kRightTop];
	Vector3 posBottom = positionsNew[kRightBottom];

	Vector3 posCenter = {posTop.x, transformBody_.translation_.y + info.move.y, posTop.z}; // For middle collision check

	posTop.y -= 0.1f;
	posBottom.y += 0.1f;

	bool hit = false;
	IndexSet indexSet = {0, 0};
	IndexSet indexTop = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	IndexSet indexCenter = mapChipField_->GetMapChipIndexSetByPosition(posCenter);
	IndexSet indexBottom = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	if (mapChipField_->GetMapChipTypeByIndex(indexTop.xIndex, indexTop.yIndex) == MapChipType::kBlock) {
		indexSet = indexTop;
		hit = true;

	} else if (mapChipField_->GetMapChipTypeByIndex(indexCenter.xIndex, indexCenter.yIndex) == MapChipType::kBlock) { // middle body check
		indexSet = indexCenter;
		hit = true;
	} else if (mapChipField_->GetMapChipTypeByIndex(indexBottom.xIndex, indexBottom.yIndex) == MapChipType::kBlock) {
		indexSet = indexBottom;
		hit = true;
	}
	if (hit) {
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float playerRight = transformBody_.translation_.x + kWidth / 2.0f;
		info.move.x = rect.left - playerRight - 0.01f;
		info.wallHit = true;
	}
}

void Player::MapCollisionLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0.0f) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transformBody_.translation_ + info.move, static_cast<Corner>(i));
	}

	// Margin
	Vector3 posTop = positionsNew[kLeftTop];
	Vector3 posBottom = positionsNew[kLeftBottom];
	Vector3 posCenter = {posTop.x, transformBody_.translation_.y + info.move.y, posTop.z}; // For middle collision check
	posTop.y -= 0.1f;
	posBottom.y += 0.1f;

	bool hit = false;
	IndexSet indexSet = {0, 0};

	// collision check
	IndexSet indexTop = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	IndexSet indexCenter = mapChipField_->GetMapChipIndexSetByPosition(posCenter);
	IndexSet indexBottom = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	if (mapChipField_->GetMapChipTypeByIndex(indexTop.xIndex, indexTop.yIndex) == MapChipType::kBlock) {

		indexSet = indexTop;
		hit = true;

	} else if (mapChipField_->GetMapChipTypeByIndex(indexCenter.xIndex, indexCenter.yIndex) == MapChipType::kBlock) {
		indexSet = indexCenter;
		hit = true;
	} else if (mapChipField_->GetMapChipTypeByIndex(indexBottom.xIndex, indexBottom.yIndex) == MapChipType::kBlock) {
		indexSet = indexBottom;
		hit = true;
	}
	if (hit) {
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float playerLeft = transformBody_.translation_.x - kWidth / 2.0f;
		info.move.x = rect.right - playerLeft + 0.01f;
		info.wallHit = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, // kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  // kLeftTop
	};
	Vector3 offset = offsetTable[static_cast<uint32_t>(corner)];

	return {center.x + offset.x, center.y + offset.y, center.z + offset.z};
}

void Player::UpdateWorldTransform(KamataEngine::WorldTransform& transform) {
	Matrix4x4 localMatrix = Mathematics::MakeAffineMatrix(transform.scale_, transform.rotation_, transform.translation_);
	if (transform.parent_) {
		transform.matWorld_ = Mathematics::Multiply(localMatrix, transform.parent_->matWorld_);
	} else {
		transform.matWorld_ = localMatrix;
	}
	transform.TransferMatrix();
}
AABB Player::GetAttackHitboxAABB() const {
	auto attackState = dynamic_cast<PlayerStateAttack*>(state_.get());
	if (attackState && attackState->IsHitboxActive()) {
		return attackState->GetHitboxAABB();
	}
	return {
	    {0, 0, 0},
        {0, 0, 0}
    };
}
