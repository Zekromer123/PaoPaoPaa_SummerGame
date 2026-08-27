#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "Mathematics.h"
#include <algorithm>
#include <KamataEngine.h>
using namespace KamataEngine;
void PlayerStateGuard::Initialize(Player* player) 
{
	player_ = player;
	if (player_->IsGrounded()) {
		player_->SetVelocity({0.0f, player_->GetVelocity().y, 0.0f});
	}
}

void PlayerStateGuard::Update() {
	if (!Input::GetInstance()->PushKey(DIK_C)) {
		player_->ChangeState(std::make_unique<PlayerStateIdle>());
		return;
	}
	guardTimer_ += 0.2f;
	float t = std::clamp(guardTimer_, 0.0f, 1.0f);
	player_->GetTransformRightArm().rotation_.x = Mathematics::EaseOut(0.0f, -60.0f, t);
	player_->GetTransformRightArm().rotation_.z = Mathematics::EaseOut(0.0f, -20.0f, t);
	player_->GetTransformLeftArm().rotation_.x = Mathematics::EaseOut(0.0f, -60.0f, t);
	player_->GetTransformLeftArm().rotation_.z = Mathematics::EaseOut(0.0f, +20.0f, t);
	player_->GetTransformRightForearm().rotation_.x = Mathematics::EaseOut(0.0f, -115.0f, t);
	player_->GetTransformLeftForearm().rotation_.x = Mathematics::EaseOut(0.0f, -115.0f, t);
	player_->GetTransformRightThigh().rotation_.x = Mathematics::EaseOut(0.0f, -15.0f, t);
	player_->GetTransformLeftThigh().rotation_.x = Mathematics::EaseOut(0.0f, +15.0f, t);
	player_->GetTransformRightCalf().rotation_.x = Mathematics::EaseOut(0.0f, +20.0f, t);
	player_->GetTransformLeftCalf().rotation_.x = Mathematics::EaseOut(0.0f, +20.0f, t);
	float baseDirY = (player_->GetDirection() == Player::LRDirection::kRight) ? +90.0f : -90.0f;
	player_->GetTransformBody().rotation_.y = baseDirY;
}