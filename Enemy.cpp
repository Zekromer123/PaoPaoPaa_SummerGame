#include "Enemy.h"
#include "Player.h"
#include <algorithm>
#include <cmath>

using namespace KamataEngine;

void Enemy::Initialize(
    Model* modelBody, Model* modelHead, Model* modelLeftArm, Model* modelLeftForearm, Model* modelRightArm, Model* modelRightForearm, Model* modelLeftThigh, Model* modelLeftCalf,
    Model* modelRightThigh, Model* modelRightCalf, Model* projectileModel, Camera* camera, const Vector3& position, EnemyType type) {
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
	projectileModel_ = projectileModel;
	camera_ = camera;
	type_ = type;
	punchSound_ = Audio::GetInstance()->LoadWave("SFX/enemyPunch.wav");

	if (type_ == EnemyType::kBoss) {
		maxHP_ = 1500;
		hp_ = maxHP_;
	} else if (type_ == EnemyType::kRangedMob) {
		maxHP_ = 300;
		hp_ = maxHP_;
	} else {
		maxHP_ = 500;
		hp_ = maxHP_;
	}

	state_ = EnemyState::kIdle;
	stateTimer_ = 0;
	attackCooldown_ = 0;

	// Transforms
	transformBody_.Initialize();
	transformTorso_.Initialize();
	transformHead_.Initialize();
	transformLeftArm_.Initialize();
	transformLeftForearm_.Initialize();
	transformRightArm_.Initialize();
	transformRightForearm_.Initialize();
	transformLeftThigh_.Initialize();
	transformLeftCalf_.Initialize();
	transformRightThigh_.Initialize();
	transformRightCalf_.Initialize();

	// Parent-Child
	transformTorso_.parent_ = &transformBody_;
	transformHead_.parent_ = &transformBody_;
	transformLeftArm_.parent_ = &transformBody_;
	transformLeftForearm_.parent_ = &transformLeftArm_;
	transformRightArm_.parent_ = &transformBody_;
	transformRightForearm_.parent_ = &transformRightArm_;
	transformLeftThigh_.parent_ = &transformBody_;
	transformLeftCalf_.parent_ = &transformLeftThigh_;
	transformRightThigh_.parent_ = &transformBody_;
	transformRightCalf_.parent_ = &transformRightThigh_;

	transformBody_.translation_ = position;

	if (type_ == EnemyType::kBoss) {
		transformBody_.scale_ = {2.0f, 2.0f, 2.0f};
	}

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
}

void Enemy::Update(Player* player, std::vector<std::unique_ptr<EnemyProjectile>>& projectiles) {
	if (IsDead())
		return;

	stateTimer_++;
	if (attackCooldown_ > 0)
		attackCooldown_--;

	
	UpdateAI(player, projectiles);

	velocity_.y -= 0.03f;
	transformBody_.translation_ = transformBody_.translation_ + velocity_;

	// Land on floor
	float floorHeight = (type_ == EnemyType::kBoss) ? 2.5f : 1.8f;
	if (transformBody_.translation_.y < floorHeight) {
		transformBody_.translation_.y = floorHeight;
		velocity_.y = 0.0f;
	}
	velocity_.x *= 0.8f; 

	UpdateAnimation();

	
	UpdateWorldTransform(transformBody_);
	UpdateWorldTransform(transformTorso_);
	UpdateWorldTransform(transformHead_);
	UpdateWorldTransform(transformLeftArm_);
	UpdateWorldTransform(transformLeftForearm_);
	UpdateWorldTransform(transformRightArm_);
	UpdateWorldTransform(transformRightForearm_);
	UpdateWorldTransform(transformLeftThigh_);
	UpdateWorldTransform(transformLeftCalf_);
	UpdateWorldTransform(transformRightThigh_);
	UpdateWorldTransform(transformRightCalf_);
}

void Enemy::UpdateAI(Player* player, std::vector<std::unique_ptr<EnemyProjectile>>& projectiles) {
	if (!player)
		return;

	float playerX = player->GetWorldPosition().x;
	float enemyX = transformBody_.translation_.x;
	float distX = playerX - enemyX;
	float absDist = std::fabs(distX);

	
	transformBody_.rotation_.y = (distX > 0) ? +90.0f : -90.0f;

	switch (state_) {
	case EnemyState::kIdle:
		if (type_ == EnemyType::kMeleeMob || type_ == EnemyType::kBoss) {
			if (absDist > 1.3f && absDist < 10.0f) {
				state_ = EnemyState::kApproach;
				stateTimer_ = 0;

			} else if (absDist <= 1.3f && attackCooldown_ <= 0) {
				state_ = EnemyState::kAttack;
				stateTimer_ = 0;
			}
		} else if (type_ == EnemyType::kRangedMob) {
			if ((absDist > 6.0f || absDist < 4.5f) && absDist < 12.0f) {
				state_ = EnemyState::kApproach;
			} else if (absDist >= 4.5f && absDist <= 6.0f && attackCooldown_ <= 0) {
				state_ = EnemyState::kAttack;
				stateTimer_ = 0;
			}
		}
		break;

	case EnemyState::kApproach:
		if (type_ == EnemyType::kMeleeMob || type_ == EnemyType::kBoss) {
			if (absDist > 1.0f) {
				float moveSpeed = (type_ == EnemyType::kBoss) ? 0.035f : 0.03f;
				transformBody_.translation_.x += (distX > 0) ? moveSpeed : -moveSpeed;
			} else {
				state_ = EnemyState::kIdle;
				stateTimer_ = 0;
			}
		} else if (type_ == EnemyType::kRangedMob) {
			if (absDist > 6.0f) {

				transformBody_.translation_.x += (distX > 0) ? 0.03f : -0.03f;
			} else if (absDist < 4.5f) {
				transformBody_.translation_.x += (distX > 0) ? -0.03f : 0.03f;
			} else {
				state_ = EnemyState::kIdle;
				stateTimer_ = 0;
			}
		}
		break;

	case EnemyState::kAttack:
		if (type_ == EnemyType::kRangedMob) {
			if (stateTimer_ == 15 && projectileModel_) {
			Audio::GetInstance()->PlayWave(punchSound_, false, 0.3f);
				Vector3 spawnPos = transformBody_.translation_;
				spawnPos.y += 0.3f;
				Vector3 projVel = {(distX > 0 ? +0.15f : -0.15f), 0.0f, 0.0f};

				auto proj = std::make_unique<EnemyProjectile>();
				proj->Initialize(projectileModel_, camera_, spawnPos, projVel);
				projectiles.push_back(std::move(proj));
			}
			if (stateTimer_ >= 35) {
				state_ = EnemyState::kIdle;
				stateTimer_ = 0;
				attackCooldown_ = 60;
			}
		} else {

			if (stateTimer_ >= 30) {
			Audio::GetInstance()->PlayWave(punchSound_, false, 0.3f);
				state_ = EnemyState::kIdle;
				stateTimer_ = 0;
				attackCooldown_ = 60;
			}
		}
		break;

	case EnemyState::kHurt:
		if (stateTimer_ >= 15) {
			state_ = EnemyState::kIdle;
			stateTimer_ = 0;
		}
		break;
	}
}

void Enemy::UpdateAnimation() {
	float t = (float)stateTimer_;

	if (state_ == EnemyState::kApproach) {
		
		float walkAngle = std::sin(t * 0.2f) * 20.0f;
		transformRightThigh_.rotation_.x = +walkAngle;
		transformLeftThigh_.rotation_.x = -walkAngle;
		transformRightArm_.rotation_.x = -walkAngle;
		transformLeftArm_.rotation_.x = +walkAngle;
	} else if (state_ == EnemyState::kAttack) {
		if (type_ == EnemyType::kRangedMob) {

			transformRightArm_.rotation_.x = (t < 15) ? 45.0f : -60.0f;
		} else {

			transformRightArm_.rotation_.x = (t < 12) ? 30.0f : -70.0f;
			transformRightForearm_.rotation_.x = (t < 12) ? 60.0f : 0.0f;
		}
	} else if (state_ == EnemyState::kHurt) {

		transformTorso_.rotation_.x = +25.0f;
		transformRightArm_.rotation_.x = -40.0f;
		transformLeftArm_.rotation_.x = -40.0f;
	} else if (state_ == EnemyState::kDead) {
		float lerp = std::clamp((float)stateTimer_ / 30.0f, 0.0f, 1.0f);

		transformBody_.rotation_.x = Mathematics::EaseOut(0.0f, -90.0f, lerp);
		transformBody_.translation_.y = Mathematics::EaseOut(2.0f, 0.3f, lerp);
		transformRightArm_.rotation_.x = -30.0f;
		transformLeftArm_.rotation_.x = -30.0f;
		transformTorso_.rotation_.x = 0.0f;
		if (stateTimer_ > 35) {
			float fadeT = std::clamp((float)(stateTimer_ - 35) / 25.0f, 0.0f, 1.0f);
			float s = Mathematics::EaseIn(1.0f, 0.0f, fadeT);
			float baseScale = (type_ == EnemyType::kBoss) ? 2.0f : 1.0f;
			transformBody_.scale_ = {baseScale * s, baseScale * s, baseScale * s};
		}
	} else {
		// Idle
		transformRightArm_.rotation_.x = 0.0f;
		transformLeftArm_.rotation_.x = 0.0f;
		transformRightThigh_.rotation_.x = 0.0f;
		transformLeftThigh_.rotation_.x = 0.0f;
		transformTorso_.rotation_.x = 0.0f;
	}
}

void Enemy::TakeDamage(int damage, const Vector3& knockback) {
	if (state_ == EnemyState::kDead)
		return;
	if (state_ == EnemyState::kHurt)
		return;

	hp_ -= damage;
	state_ = EnemyState::kHurt;
	stateTimer_ = 0;
	velocity_ = knockback;

	if (hp_ <= 0) {
		hp_ = 0;
		state_ = EnemyState::kDead;
		stateTimer_ = 0;
		velocity_ = {knockback.x * 1.5f, 0.25f, 0.0f};
	}
}

void Enemy::UpdateWorldTransform(WorldTransform& transform) {
	Matrix4x4 localMatrix = Mathematics::MakeAffineMatrix(transform.scale_, transform.rotation_, transform.translation_);
	if (transform.parent_) {
		transform.matWorld_ = Mathematics::Multiply(localMatrix, transform.parent_->matWorld_);
	} else {
		transform.matWorld_ = localMatrix;
	}
	transform.TransferMatrix();
}

void Enemy::Draw() {
	if (IsDead())
		return;

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

AABB Enemy::GetAABB() const {
	float halfWidth = (type_ == EnemyType::kBoss) ? 1.8f : 0.6f;
	float halfHeight = (type_ == EnemyType::kBoss) ? 2.5f : 1.2f;
	Vector3 center = transformBody_.translation_;
	center.y += halfHeight;

	AABB aabb;
	aabb.min = {center.x - halfWidth, center.y - halfHeight, center.z - halfWidth};
	aabb.max = {center.x + halfWidth, center.y + halfHeight, center.z + halfWidth};
	return aabb;
}
Vector3 Enemy::GetWorldPosition() const {
	Vector3 worldPos;
	worldPos.x = transformBody_.matWorld_.m[3][0];
	worldPos.y = transformBody_.matWorld_.m[3][1];
	worldPos.z = transformBody_.matWorld_.m[3][2];
	return worldPos;
}