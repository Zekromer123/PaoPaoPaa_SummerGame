#pragma once
#include "BasePlayerState.h"
class PlayerStateWalk : public BasePlayerState {
public:
	void Update() override;

private:
	static inline const float kWalkSpeed = 0.2f;

};
