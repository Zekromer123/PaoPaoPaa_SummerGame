#include "PlayerStateDead.h"
#include "Mathematics.h"
#include "Player.h"
#include <algorithm>
using namespace KamataEngine;


void PlayerStateDead::Initialize(Player* player) {
	player_ = player;
	timer_ = 0;
	float knockDir = (player_->GetDirection() == Player::LRDirection::kRight) ? -0.2f : +0.2f;
	player_->SetVelocity({knockDir, 0.25f, 0.0f});
}

void PlayerStateDead::Update() {
	timer_++;
	
	Vector3 vel = player_->GetVelocity();
	vel.x *= 0.85f;
	player_->SetVelocity(vel);
	
	float t = std::clamp((float)timer_ / 30.0f, 0.0f, 1.0f);
	player_->GetTransformBody().rotation_.x = Mathematics::EaseOut(0.0f, 90.0f, t);
	
	player_->GetTransformRightArm().rotation_.x = -20.0f;
	player_->GetTransformLeftArm().rotation_.x = -20.0f;
	player_->GetTransformTorso().rotation_.x = 0.0f;
}