#pragma once
#include "BasePlayerState.h"
#include <KamataEngine.h>
#include "Mathematics.h"

enum class AttackType {
	kLight,
	kHeavy,
};

enum class AttackPhase {
	kStartup,
	kActive,
	kRecovery,
};

struct AttackParameter {
	int chargeFrames;
	int attackFrames;
	int clearFrames;
	KamataEngine::Vector3 hitboxSize;
	KamataEngine::Vector3 hitboxOffset;
};

class PlayerStateAttack : public BasePlayerState {
public:
	PlayerStateAttack(AttackType type, int comboStep = 1);

	void Initialize(Player* player) override;
	void Update() override;

	// Getter Hitbox
	AttackType GetAttackType() const { return attackType_; };
	AttackPhase GetAttackPhase() const { return attackPhase_; }
	int GetComboStep() const { return comboStep_; };

	KamataEngine::Vector3 GetHitboxOffset() const;
	KamataEngine::Vector3 GetHitboxSize() const;
	AABB GetHitboxAABB() const;

	//for imgui
	bool IsAttacking() const override { return true; }
	bool IsHitboxActive() const override { return attackPhase_ == AttackPhase::kActive; }
	

private:
	AttackType attackType_ = AttackType::kLight;
	AttackPhase attackPhase_ = AttackPhase::kStartup;
	int comboStep_ = 1;
	int counter_ = 0;
	bool isNextAttackRequested_ = false;
	AttackParameter attackParam_;

	void SetupAttackParameters();
	void ChangePhase(AttackPhase nextPhase);
	static inline uint32_t punchSound_;
};
