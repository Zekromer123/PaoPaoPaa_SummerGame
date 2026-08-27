#pragma once

class Player;
class BasePlayerState {
public:
	virtual ~BasePlayerState() = default;
	virtual void Initialize(Player* player) { player_ = player; }
	virtual void Update() = 0;

	// for imgui
	virtual bool IsAttacking() const { return false; }
	virtual bool IsHitboxActive() const { return false; }
	virtual const char* GetStateName() const { return "Unknown"; }

protected:
	Player* player_ = nullptr;
};
