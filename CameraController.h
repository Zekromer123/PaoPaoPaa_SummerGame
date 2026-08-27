#pragma once
#include "Player.h"
#include "KamataEngine.h"

class Player;

class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

public:
	void Initialize(Player* target);
	void Update();
	void SetTarget(Player* target) { target_ = target; };
	void Reset();
	void SetMovableArea(Rect area) { movableArea_ = area; };
	KamataEngine::Vector3 Lerp(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2, float t);
	KamataEngine::Camera& GetCamera() { return cameraController_; };

private:
	KamataEngine::Camera cameraController_{};
	Player* target_ = nullptr;
	KamataEngine::Vector3 targetOffset_ = {0.0f, 0.0f, -10.5f}; // Zoom in / Zoom out Camera
	Rect movableArea_ = {5.0f, 100.0f, 6.0f, 100.0f};
	Rect margin_ = {-2.0f, 2.0f, -1.0f, 2.0f};//Dead Zone
	static inline const float kInterpolationRate = 0.08f;
	static inline const float kVelocityBias = 2.0f;
};
