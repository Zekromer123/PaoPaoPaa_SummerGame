#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateAttack.h"
#include "PlayerStateGuard.h"
#include "PlayerStateJump.h"
#include "PlayerStateWalk.h"
#include <KamataEngine.h>

using namespace KamataEngine;
void PlayerStateIdle::Update() {
	player_->SetVelocity({0.0f, player_->GetVelocity().y, 0.0f});
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		player_->SetDirection(Player::LRDirection::kRight);
		player_->ChangeState(std::make_unique<PlayerStateWalk>());
		return;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		player_->SetDirection(Player::LRDirection::kLeft);
		player_->ChangeState(std::make_unique<PlayerStateWalk>());
		return;
	}

	if (Input::GetInstance()->TriggerKey(DIK_UP) && player_->IsGrounded()) {
		player_->ChangeState(std::make_unique<PlayerStateJump>());
		return;
	}
	// LightAttack
	if (Input::GetInstance()->TriggerKey(DIK_Z)) {
		player_->ChangeState(std::make_unique<PlayerStateAttack>(AttackType::kLight, 1));
		return;
	}
	// Heavy Attack
	if (Input::GetInstance()->TriggerKey(DIK_X)) {
		player_->ChangeState(std::make_unique<PlayerStateAttack>(AttackType::kHeavy, 1));
		return;
	}
	// Guard
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		player_->ChangeState(std::make_unique<PlayerStateGuard>());
		return;
	}

#pragma region Animations
	
	//Reset Animations
	player_->GetTransformHead().rotation_ = {0.0f, 0.0f, 0.0f};
	//player_->GetTransformHead().translation_ = {0.0f, 0.84f, 0.0f};
	player_->GetTransformTorso().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformRightArm().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformLeftArm().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformRightForearm().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformLeftForearm().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformRightThigh().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformLeftThigh().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformRightCalf().rotation_ = {0.0f, 0.0f, 0.0f};
	player_->GetTransformLeftCalf().rotation_ = {0.0f, 0.0f, 0.0f};

	// Animation
	static float timer = 0.0f;
	timer += 0.05f;
	Vector3 scale = {1.0f, 1.0f + sinf(timer) * 0.03f, 1.0f};
	player_->GetTransformTorso().scale_ = scale;
	player_->GetTransformHead().scale_ = scale;
	player_->GetTransformRightArm().scale_ = scale;
	player_->GetTransformLeftArm().scale_ = scale;
	player_->GetTransformRightForearm().scale_ = scale;
	player_->GetTransformLeftForearm().scale_ = scale;

#pragma endregion
}
