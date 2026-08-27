#include "PlayerStateAttack.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "Mathematics.h"
#include <algorithm>

using namespace KamataEngine;

PlayerStateAttack::PlayerStateAttack(AttackType type, int comboStep) : attackType_(type), comboStep_(comboStep) {}

void PlayerStateAttack::Initialize(Player* player) {
	player_ = player;
	counter_ = 0;
	attackPhase_ = AttackPhase::kStartup;
	isNextAttackRequested_ = false;

	if (punchSound_ == 0) {
		punchSound_ = Audio::GetInstance()->LoadWave("SFX/playerPunching.wav");
	}
	Audio::GetInstance()->PlayWave(punchSound_, false, 0.8f);
	if (player_->IsGrounded()) {
		float forwardDir = (player_->GetDirection() == Player::LRDirection::kRight) ? +1.0f : -1.0f;

		float stepSpeed = 0.08f;
		Vector3 vel = player_->GetVelocity();
		vel.x = forwardDir * stepSpeed;
		player_->SetVelocity(vel);
	}

	// Load Parameter duration & hitbox
	SetupAttackParameters();
}

void PlayerStateAttack::Update() {
	counter_++;

	Vector3 vel = player_->GetVelocity();
	vel.x *= 0.88f; 
	player_->SetVelocity(vel);

	// Combo Input
	if (attackType_ == AttackType::kLight && Input::GetInstance()->TriggerKey(DIK_Z)) {
		isNextAttackRequested_ = true;
		
	}
	if (attackType_ == AttackType::kHeavy && Input::GetInstance()->TriggerKey(DIK_X)) {
		isNextAttackRequested_ = true;
	}

	// Phase Change
	switch (attackPhase_) {
	case AttackPhase::kStartup:
		if (counter_ >= attackParam_.chargeFrames) {
			ChangePhase(AttackPhase::kActive);
		}
		break;
	case AttackPhase::kActive:
		if (counter_ >= attackParam_.attackFrames) {
			ChangePhase(AttackPhase::kRecovery);
		}
		break;
	case AttackPhase::kRecovery:
		if (counter_ >= attackParam_.clearFrames) {
			// If theres input combo & combo step is not yet 4 -> conitnue combo
			if (isNextAttackRequested_ && comboStep_ < 4) {
				player_->ChangeState(std::make_unique<PlayerStateAttack>(attackType_, comboStep_ + 1));
				
				return;
			} else {
				// End, back to idle
				player_->ChangeState(std::make_unique<PlayerStateIdle>());
				return;
			}
		}
		break;
	}

#pragma region Animation

	 float t = 0.0f;
	if (attackPhase_ == AttackPhase::kStartup) {
		t = std::clamp((float)counter_ / (float)attackParam_.chargeFrames, 0.0f, 1.0f);
	} else if (attackPhase_ == AttackPhase::kActive) {
		t = std::clamp((float)counter_ / (float)attackParam_.attackFrames, 0.0f, 1.0f);
	} else {
		t = std::clamp((float)counter_ / (float)attackParam_.clearFrames, 0.0f, 1.0f);
	}


 if (attackType_ == AttackType::kLight) {
	
		if (comboStep_ == 1) { 
			if (attackPhase_ == AttackPhase::kStartup) {
				
				player_->GetTransformRightArm().rotation_.x = Mathematics::EaseIn(0.0f, -15.0f, t);
				player_->GetTransformRightForearm().rotation_.x = Mathematics::EaseIn(0.0f, -75.0f, t);
			} else if (attackPhase_ == AttackPhase::kActive) {
				
				player_->GetTransformRightArm().rotation_.x = Mathematics::EaseOut(-15.0f, -90.0f, t);
				player_->GetTransformRightForearm().rotation_.x = Mathematics::EaseOut(-90.0f, 0.0f, t);
			} else { // kRecovery
				
				player_->GetTransformRightArm().rotation_.x = Mathematics::EaseOut(-90.0f, 0.0f, t);
				player_->GetTransformRightForearm().rotation_.x = Mathematics::EaseOut(0.0f, 0.0f, t);
			}
		} else if (comboStep_ == 2) { 
			if (attackPhase_ == AttackPhase::kStartup) {
				player_->GetTransformLeftArm().rotation_.x = Mathematics::EaseIn(0.0f, -15.0f, t);
				player_->GetTransformLeftForearm().rotation_.x = Mathematics::EaseIn(0.0f, -75.0f, t);
			} else if (attackPhase_ == AttackPhase::kActive) {
				player_->GetTransformLeftArm().rotation_.x = Mathematics::EaseOut(-15.0f, -90.0f, t);
				player_->GetTransformLeftForearm().rotation_.x = Mathematics::EaseOut(-75.0f, 0.0f, t);
			} else {
				player_->GetTransformLeftArm().rotation_.x = Mathematics::EaseOut(-90.0f, 0.0f, t);
			}
		} else if (comboStep_ == 3) { 
			if (attackPhase_ == AttackPhase::kStartup) {
				player_->GetTransformRightArm().rotation_.x = Mathematics::EaseIn(0.0f, -20.0f, t);
				player_->GetTransformRightForearm().rotation_.x = Mathematics::EaseIn(0.0f, -60.0f, t);
			} else if (attackPhase_ == AttackPhase::kActive) {
				player_->GetTransformRightArm().rotation_.x = Mathematics::EaseOut(-20.0f, -45.0f, t);
				player_->GetTransformRightForearm().rotation_.x = Mathematics::EaseOut(-60.0f, -40.0f, t);
			} else {
				player_->GetTransformRightArm().rotation_.x = Mathematics::EaseOut(-45.0f, 0.0f, t);
			}
		} else if (comboStep_ == 4) { 
			if (attackPhase_ == AttackPhase::kStartup) {
				player_->GetTransformLeftArm().rotation_.x = Mathematics::EaseIn(0.0f, -30.0f, t);
				player_->GetTransformLeftForearm().rotation_.x = Mathematics::EaseIn(0.0f, -80.0f, t);
			} else if (attackPhase_ == AttackPhase::kActive) {
				player_->GetTransformLeftArm().rotation_.x = Mathematics::EaseOut(-30.0f, -80.0f, t);
				player_->GetTransformLeftForearm().rotation_.x = Mathematics::EaseOut(-80.0f, -20.0f, t);
			} else {
				player_->GetTransformLeftArm().rotation_.x = Mathematics::EaseOut(-80.0f, 0.0f, t);
				player_->GetTransformLeftForearm().rotation_.x = Mathematics::EaseOut(-20.0f, 0.0f, t);
			}
		}
 } else {
	
	 if (comboStep_ == 1) { 
		 if (attackPhase_ == AttackPhase::kStartup) {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseIn(0.0f, -45.0f, t);
			 player_->GetTransformRightCalf().rotation_.x = Mathematics::EaseIn(0.0f, -60.0f, t);
		 } else if (attackPhase_ == AttackPhase::kActive) {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseOut(-45.0f, -55.0f, t);
			 player_->GetTransformRightCalf().rotation_.x = Mathematics::EaseOut(-60.0f, 0.0f, t); 
		 } else {                                                                                  // kRecovery
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseOut(-55.0f, 0.0f, t);
		 }
	 } else if (comboStep_ == 2) {
		 if (attackPhase_ == AttackPhase::kStartup) {
			 player_->GetTransformLeftThigh().rotation_.x = Mathematics::EaseIn(0.0f, -45.0f, t);
			 player_->GetTransformLeftCalf().rotation_.x = Mathematics::EaseIn(0.0f, -60.0f, t);
		 } else if (attackPhase_ == AttackPhase::kActive) {
			 player_->GetTransformLeftThigh().rotation_.x = Mathematics::EaseOut(-45.0f, -55.0f, t);
			 player_->GetTransformLeftCalf().rotation_.x = Mathematics::EaseOut(-60.0f, 0.0f, t); 
		 } else {
			 player_->GetTransformLeftThigh().rotation_.x = Mathematics::EaseOut(-55.0f, 0.0f, t);
		 }
	 } else if (comboStep_ == 3) { 
		 if (attackPhase_ == AttackPhase::kStartup) {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseIn(0.0f, -55.0f, t);
			 player_->GetTransformRightCalf().rotation_.x = Mathematics::EaseIn(0.0f, -65.0f, t);
		 } else if (attackPhase_ == AttackPhase::kActive) {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseOut(-55.0f, -65.0f, t); 
			 player_->GetTransformRightCalf().rotation_.x = Mathematics::EaseOut(-65.0f, 0.0f, t);
		 } else {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseOut(-65.0f, 0.0f, t);
		 }
	 } else if (comboStep_ == 4) { 
		 if (attackPhase_ == AttackPhase::kStartup) {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseIn(0.0f, -50.0f, t);
			 player_->GetTransformRightCalf().rotation_.x = Mathematics::EaseIn(0.0f, -70.0f, t);
		 } else if (attackPhase_ == AttackPhase::kActive) {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseOut(-50.0f, -70.0f, t); 
			 player_->GetTransformRightCalf().rotation_.x = Mathematics::EaseOut(-70.0f, 0.0f, t);
		 } else {
			 player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseOut(-70.0f, 0.0f, t);
		 }
	 }
	}
#pragma endregion
}

void PlayerStateAttack::ChangePhase(AttackPhase nextPhase) {
	attackPhase_ = nextPhase;
	counter_ = 0; // reset counter every change phase


}
void PlayerStateAttack::SetupAttackParameters() {
	if (attackType_ == AttackType::kLight) {
		// --- LIGHT ATTACK (Z1, Z2, Z3, Z4) ---
			attackParam_.chargeFrames = 8;
			attackParam_.attackFrames = 16;
			attackParam_.clearFrames = 10;
			attackParam_.hitboxSize = {1.8f, 1.8f, 1.0f};
		
	} else {
		// --- HEAVY ATTACK (X1, X2, X3 ---
		
			// X1, X2, X3
			attackParam_.chargeFrames = 9;
			attackParam_.attackFrames = 20;
			attackParam_.clearFrames = 12;
			attackParam_.hitboxSize = {1.8f, 1.6f, 1.0f};
		
	}
}

Vector3 PlayerStateAttack::GetHitboxSize() const { return attackParam_.hitboxSize; }
Vector3 PlayerStateAttack::GetHitboxOffset() const {
	float dirFactor = (player_->GetDirection() == Player::LRDirection::kRight) ? +0.4f : -0.4f;
	return {dirFactor, 0.0f, 0.0f};
}

AABB PlayerStateAttack::GetHitboxAABB() const {
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 offset = GetHitboxOffset();
	Vector3 size = GetHitboxSize();
	Vector3 center = {playerPos.x + offset.x, playerPos.y + offset.y, playerPos.z + offset.z};
	AABB aabb;
	aabb.min = {center.x - size.x / 2.0f, center.y - size.y / 2.0f, center.z - size.z / 2.0f};
	aabb.max = {center.x + size.x / 2.0f, center.y + size.y / 2.0f, center.z + size.z / 2.0f};
	return aabb;
}