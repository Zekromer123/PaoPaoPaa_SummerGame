#pragma once
#include "BasePlayerState.h"
#include <KamataEngine.h>
class PlayerStateDead : public BasePlayerState {
public:
	void Initialize(Player* player) override;
	void Update() override;

	bool IsAttacking() const override { return false; }
	bool IsHitboxActive() const override { return false; }
	const char* GetStateName() const override { return "Dead"; }

private:
	int timer_ = 0;
};