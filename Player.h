#pragma once
#include "BasePlayerState.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Mathematics.h"
#include <memory>

class MapChipField;
enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };
class Player {
public:
	enum class LRDirection {
		kRight,
		kLeft,
	};
	struct CollisionMapInfo {
		bool ceilingHit = false;
		bool floorHit = false;
		bool wallHit = false;
		Vector3 move;
	};
	Player() = default;
	~Player() = default;

	void Initialize(
	    KamataEngine::Model* modelBody, KamataEngine::Model* modelHead, KamataEngine::Model* modelLeftArm, KamataEngine::Model* modelLeftForearm, KamataEngine::Model* modelRightArm,
	    KamataEngine::Model* modelRightForearm, KamataEngine::Model* modelLeftThigh, KamataEngine::Model* modelLeftCalf, KamataEngine::Model* modelRightThigh, KamataEngine::Model* modelRightCalf,
	    KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();
#ifdef _DEBUG
	void DrawImGui();
#endif
	// Change State
	void ChangeState(std::unique_ptr<BasePlayerState> newState);

	// Getter & Setter
	const KamataEngine::Vector3& GetPosition() const { return transformBody_.translation_; }
	BasePlayerState* GetState() const { return state_.get(); };
	// void SetPosition(const KamataEngine::Vector3& pos) { worldTransform_.translation_ = pos; }
	void SetMapChipField(MapChipField* mapChipField);

	// WorldTransform
	KamataEngine::WorldTransform& GetTransformBody() { return transformBody_; }
	KamataEngine::WorldTransform& GetTransformTorso() { return transformTorso_; }
	KamataEngine::WorldTransform& GetTransformHead() { return transformHead_; }
	KamataEngine::WorldTransform& GetTransformLeftArm() { return transformLeftArm_; }
	KamataEngine::WorldTransform& GetTransformLeftForearm() { return transformLeftForearm_; }
	KamataEngine::WorldTransform& GetTransformRightArm() { return transformRightArm_; }
	KamataEngine::WorldTransform& GetTransformRightForearm() { return transformRightForearm_; }
	KamataEngine::WorldTransform& GetTransformLeftThigh() { return transformLeftThigh_; }
	KamataEngine::WorldTransform& GetTransformLeftCalf() { return transformLeftCalf_; }
	KamataEngine::WorldTransform& GetTransformRightThigh() { return transformRightThigh_; }
	KamataEngine::WorldTransform& GetTransformRightCalf() { return transformRightCalf_; }

	////Pivot
	// KamataEngine::WorldTransform& GetPivotShoulderL() { return pivotShoulderL_; }
	// KamataEngine::WorldTransform& GetPivotShoulderR() { return pivotShoulderR_; }
	// KamataEngine::WorldTransform& GetPivotElbowL() { return pivotElbowL_; }
	// KamataEngine::WorldTransform& GetPivotElbowR() { return pivotElbowR_; }
	// KamataEngine::WorldTransform& GetPivotHipL() { return pivotHipL_; }
	// KamataEngine::WorldTransform& GetPivotHipR() { return pivotHipR_; }
	// KamataEngine::WorldTransform& GetPivotKneeL() { return pivotKneeL_; }
	// KamataEngine::WorldTransform& GetPivotKneeR() { return pivotKneeR_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	void SetVelocity(const KamataEngine::Vector3& vel) { velocity_ = vel; }
	bool IsGrounded() const { return isGrounded_; }
	void SetGrounded(bool grounded) { isGrounded_ = grounded; }
	LRDirection GetDirection() const { return lrDirection_; }
	void SetDirection(LRDirection dir) { lrDirection_ = dir; }
	KamataEngine::Vector3 GetWorldPosition() const;
	KamataEngine::WorldTransform& GetWorldTransform() { return transformBody_; }
	AABB GetAABB() const;
	void NoObject(const CollisionMapInfo& info);
	void WhileCollision(const CollisionMapInfo& info);
	int GetAttackDamage() const;

	// HP & Damage Player
	void TakeDamage(int damage, const KamataEngine::Vector3& knockback);
	int GetHp() const { return hp_; }
	int GetMaxHp() const { return maxHp_; }
	bool IsDead() const { return hp_ <= 0; }
	bool IsInvincible() const { return invincibleTimer_ > 0; }

	// Helper
	bool IsAttacking() const { return state_ ? state_->IsAttacking() : false; }
	bool IsHitboxActive() const { return state_ ? state_->IsHitboxActive() : false; }
	AABB GetAttackHitboxAABB() const;
	const char* GetStateName() const { return state_ ? state_->GetStateName() : "None"; }

private:
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionUp(CollisionMapInfo& info);
	void MapCollisionDown(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void UpdateWorldTransform(KamataEngine::WorldTransform& transform);

private:
	// KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_ = nullptr;
	// KamataEngine::Model* model_ = nullptr;

	KamataEngine::Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	bool isGrounded_ = false;
	LRDirection lrDirection_ = LRDirection::kRight;

	MapChipField* mapChipField_ = nullptr;

	float kWidth = 1.0f;
	float kHeight = 1.5f;
	static inline const float kAttenuationLanding = 0.1f;

	std::unique_ptr<BasePlayerState> state_ = nullptr;

	int maxHp_ = 100;         // Player Max HP
	int hp_ = maxHp_;         // Player HP
	int invincibleTimer_ = 0; // Invincible Time after taking damage

	// Model
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
	// 6 WorldTransform (Parent-Child)
	KamataEngine::WorldTransform transformBody_;         // ROOT PARENT || and collision box
	KamataEngine::WorldTransform transformTorso_;        // MESH Position
	KamataEngine::WorldTransform transformHead_;         // Child 1
	KamataEngine::WorldTransform transformLeftArm_;      // Child 2
	KamataEngine::WorldTransform transformLeftForearm_;  // Child 2
	KamataEngine::WorldTransform transformRightArm_;     // Child 3
	KamataEngine::WorldTransform transformRightForearm_; // Child 3
	KamataEngine::WorldTransform transformLeftThigh_;    // Child 4
	KamataEngine::WorldTransform transformLeftCalf_;     // Child 4
	KamataEngine::WorldTransform transformRightThigh_;   // Child 5
	KamataEngine::WorldTransform transformRightCalf_;    // Child 5

	////Pivot
	// KamataEngine::WorldTransform pivotShoulderL_;
	// KamataEngine::WorldTransform pivotShoulderR_;
	// KamataEngine::WorldTransform pivotElbowL_;
	// KamataEngine::WorldTransform pivotElbowR_;
	// KamataEngine::WorldTransform pivotHipL_;
	// KamataEngine::WorldTransform pivotHipR_;
	// KamataEngine::WorldTransform pivotKneeL_;
	// KamataEngine::WorldTransform pivotKneeR_;
};
