#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include <KamataEngine.h>

using namespace KamataEngine;

void PlayerStateJump::Initialize(Player* player) { 
	player_ = player;
	Vector3 velocity = player_->GetVelocity();
	velocity.y = kJumpSpeed;
	player_->SetVelocity(velocity);
	player_->SetGrounded(false);
}

void PlayerStateJump::Update() {
	Vector3 velocity = player_->GetVelocity();
	velocity.x = 0.0f;
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		velocity.x = +0.2f;
		player_->SetDirection(Player::LRDirection::kRight);
		
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		velocity.x = -0.2f;
		player_->SetDirection(Player::LRDirection::kLeft);
	}
	player_->SetVelocity(velocity);
	if (player_->IsGrounded()) {
		player_->ChangeState(std::make_unique<PlayerStateIdle>());
		return;
	}
}
