#pragma once
#include "EnemyProjectile.h"
#include "MapChipField.h"
#include "Mathematics.h"
#include <KamataEngine.h>
#include <memory>
#include <vector>
class Player;

enum class EnemyType {
	kMeleeMob,  // E0 
	kRangedMob, // E1 
	kBoss       // E2 
};

enum class EnemyState {
	kIdle, 
	kApproach,
	kAttack, 
	kHurt, 
	kDead 
};
class Enemy {
public:
	void Initialize(
	    KamataEngine::Model* modelBody, KamataEngine::Model* modelHead, KamataEngine::Model* modelLeftArm, KamataEngine::Model* modelLeftForearm, KamataEngine::Model* modelRightArm,
	    KamataEngine::Model* modelRightForearm, KamataEngine::Model* modelLeftThigh, KamataEngine::Model* modelLeftCalf, KamataEngine::Model* modelRightThigh, KamataEngine::Model* modelRightCalf,
	    KamataEngine::Model* projectileModel, KamataEngine::Camera* camera, const KamataEngine::Vector3& position, EnemyType type);

	  void Update(Player* player, std::vector<std::unique_ptr<EnemyProjectile>>& projectiles);
	void Draw();
	void TakeDamage(int damage, const KamataEngine::Vector3& knockback);
	bool IsDying() const { return state_ == EnemyState::kDead; } // cant move, cant attack, cant take damage
	bool IsDead() const { return state_ == EnemyState::kDead && stateTimer_ >= 60; } // dead and finished dying animation
	bool IsAttacking() const { return (state_ == EnemyState::kAttack && stateTimer_ >= 10 && stateTimer_ <= 25); };

	EnemyType GetType() const { return type_; }
	int GetHP() const { return hp_; }
	int GetMaxHP() const { return maxHP_; }
	AABB GetAABB() const;
	KamataEngine::Vector3 GetWorldPosition() const;
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	EnemyType type_ = EnemyType::kMeleeMob;
	EnemyState state_ = EnemyState::kIdle;
	int hp_ = 100;
	int maxHP_ = 100;
	int stateTimer_ = 0;
	int attackCooldown_ = 0;
	KamataEngine::Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	MapChipField* mapChipField_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::Model* projectileModel_ = nullptr;

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

	KamataEngine::WorldTransform transformBody_;
	KamataEngine::WorldTransform transformTorso_;
	KamataEngine::WorldTransform transformHead_;
	KamataEngine::WorldTransform transformLeftArm_;
	KamataEngine::WorldTransform transformLeftForearm_;
	KamataEngine::WorldTransform transformRightArm_;
	KamataEngine::WorldTransform transformRightForearm_;
	KamataEngine::WorldTransform transformLeftThigh_;
	KamataEngine::WorldTransform transformLeftCalf_;
	KamataEngine::WorldTransform transformRightThigh_;
	KamataEngine::WorldTransform transformRightCalf_;

	void UpdateWorldTransform(KamataEngine::WorldTransform& transform);
	void UpdateAI(Player* player, std::vector<std::unique_ptr<EnemyProjectile>>& projectiles);
	void UpdateAnimation();

	uint32_t punchSound_ = 0;
};
