#pragma once
#include "BasePlayerState.h"

class PlayerStateJump : public BasePlayerState{
public:
	void Initialize(Player* player) override;
	void Update() override;

	private:
	static inline const float kJumpSpeed = 0.5f;
};
