#include "PlayerStateWalk.h"
#include "Player.h"
#include "PlayerStateAttack.h"
#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include <KamataEngine.h>
#include <numbers>

using namespace KamataEngine;
void PlayerStateWalk::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_Z)) {
		player_->ChangeState(std::make_unique<PlayerStateAttack>(AttackType::kLight, 1));
		return;
	}
	if (Input::GetInstance()->TriggerKey(DIK_X)) {
		player_->ChangeState(std::make_unique<PlayerStateAttack>(AttackType::kHeavy, 1));
		return;
	}
	if (Input::GetInstance()->PushKey(DIK_C)) {
		player_->ChangeState(std::make_unique<PlayerStateGuard>());
		return;
	}

	Vector3 velocity = player_->GetVelocity();
	velocity.x = 0.0f;

	if (Input::GetInstance()->TriggerKey(DIK_UP) && player_->IsGrounded()) {
		player_->ChangeState(std::make_unique<PlayerStateJump>());
		return;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		velocity.x = +kWalkSpeed;
		player_->SetDirection(Player::LRDirection::kRight);
	} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		velocity.x = -kWalkSpeed;
		player_->SetDirection(Player::LRDirection::kLeft);
	} else {
		player_->ChangeState(std::make_unique<PlayerStateIdle>());
		return;
	}
	player_->SetVelocity(velocity);

#pragma region Animation
	// Running Animation
	static float runTimer = 0.0f;
	runTimer += 0.25f;
	float legAngle = sinf(runTimer) * 45.0f;
	float armAngle = sinf(runTimer) * 45.0f;

	//Head angle
	//player_->GetTransformHead().rotation_.x = 20.0f;
	//player_->GetTransformHead().translation_.y = 0.8f;
	//player_->GetTransformHead().translation_.z = 0.14f;
	//Body Angle
	//player_->GetTransformTorso().rotation_.x = 20.0f;

	// Legs Animation
	player_->GetTransformRightThigh().rotation_.x = +legAngle;
	player_->GetTransformLeftThigh().rotation_.x = -legAngle;

	// Arms Animation
	player_->GetTransformRightArm().rotation_.x = +armAngle;
	player_->GetTransformLeftArm().rotation_.x = -armAngle;

	// Forearms & Knees Animation
	if (legAngle < 0.0f) {
		
		player_->GetTransformRightCalf().rotation_.x = +75.0f * (fabsf(legAngle) / 45.0f);
		player_->GetTransformLeftCalf().rotation_.x = 0.0f;
	} else {
		
		player_->GetTransformLeftCalf().rotation_.x = +75.0f * (fabsf(legAngle) / 45.0f);
		player_->GetTransformRightCalf().rotation_.x = 0.0f;
	}

	player_->GetTransformRightForearm().rotation_.x = -85.0f;
	player_->GetTransformLeftForearm().rotation_.x = -85.0f;

	// Body
	float shoulderTwist = sinf(runTimer) * 6.0f;
	float baseDirY = (player_->GetDirection() == Player::LRDirection::kRight) ? +90.0f : -90.0f;
	player_->GetTransformBody().rotation_.y = baseDirY + shoulderTwist;
	player_->GetTransformTorso().scale_.y = 1.0f + fabsf(sinf(runTimer)) * 0.04f;
}
