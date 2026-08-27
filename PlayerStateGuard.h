#pragma once
#include "BasePlayerState.h"
class PlayerStateGuard :public BasePlayerState{
public:
	void Initialize(Player* player)override;
	void Update();

	 bool IsAttacking() const override { return false; }
	bool IsHitboxActive() const override { return false; }

	private:
	float guardTimer_ = 0.0f;
};
