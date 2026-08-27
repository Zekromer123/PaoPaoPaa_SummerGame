#include "BackgroundManager.h"
#include "Mathematics.h"

BackgroundManager::~BackgroundManager() {
	if (modelTower_) {
		delete modelTower_;
		modelTower_ = nullptr;
	}
	if (modelWindow_) {
		delete modelWindow_;
		modelWindow_ = nullptr;
	}
	for (auto* tower : transformTowers_) {
		delete tower;
	}
	transformTowers_.clear();
	for (auto* window : transformWindows_) {
		delete window;
	}
	transformWindows_.clear();
	if (modelCeiling_) {
		delete modelCeiling_;
		modelCeiling_ = nullptr;
	}
	for (auto* t : transformCeilings_) {
		delete t;
	}
	transformCeilings_.clear();
	if (modelWallFloor_) {
		delete modelWallFloor_;
		modelWallFloor_ = nullptr;
	}
	for (auto* t : transformFloors_) {
		delete t;
	}
	transformFloors_.clear();
	for (auto* t : transformWalls_) {
		delete t;
	}
	transformWalls_.clear();
}

void BackgroundManager::Initialize() {

	modelTower_ = KamataEngine::Model::CreateFromOBJ("Tower", true);
	modelWindow_ = KamataEngine::Model::CreateFromOBJ("Window", true);
	modelWallFloor_ = KamataEngine::Model::CreateFromOBJ("Wood", true);
	modelCeiling_ = KamataEngine::Model::CreateFromOBJ("Ceiling", true);

	transformTowers_.resize(5);
	for (int i = 0; i < 5; ++i) {
		transformTowers_[i] = new KamataEngine::WorldTransform();
		transformTowers_[i]->Initialize();
		transformTowers_[i]->translation_ = {i * 25.0f, -5.0f, 70.0f};     // Tiap 25 meter di Z=70
		transformTowers_[i]->scale_ = {4.0f, 3.0f + (i % 3) * 3.0f, 4.0f}; // Variasi tinggi
	}
	int mapLength = 15;
	transformWindows_.resize(mapLength);
	transformWalls_.resize(mapLength);
	for (int i = 0; i < mapLength; ++i) {
		float margin = -20.0f+(i * 10.0f);
		transformWindows_[i] = new KamataEngine::WorldTransform();
		transformWindows_[i]->Initialize();
		transformWindows_[i]->translation_ = {margin, 2.0f, 20.0f};
		transformWindows_[i]->rotation_.x = 180.0f;
		transformWindows_[i]->scale_ = {4.5f, 4.5f, 1.0f};

		transformWalls_[i] = new KamataEngine::WorldTransform();
		transformWalls_[i]->Initialize();
		transformWalls_[i]->translation_ = {margin, 3.0f, 22.0f};
		transformWalls_[i]->rotation_ = {90.0f, 0.0f, 90.0f};
		transformWalls_[i]->scale_ = {5.0f,7.5f,5.0f};

	}

	transformFloors_.resize(mapLength);
	transformCeilings_.resize(mapLength);
	for (int i = 0; i < mapLength; ++i) {
		transformFloors_[i] = new KamataEngine::WorldTransform();
		transformFloors_[i]->Initialize();
		transformFloors_[i]->translation_ = {i * 10.0f, 0.4f, 0.0f};
		transformFloors_[i]->scale_ = {3.0f, 3.0f, 1.0f};
		transformCeilings_[i] = new KamataEngine::WorldTransform();
		transformCeilings_[i]->Initialize();
		transformCeilings_[i]->translation_ = {i * 10.0f, 8.8f, 0.0f};
		transformCeilings_[i]->scale_ = {3.0f, 7.0f, 1.0f};
	}
}

void BackgroundManager::Update() {
	// Update Matriks Gedung
	for (auto& tower : transformTowers_) {
		tower->matWorld_ = Mathematics::MakeAffineMatrix(tower->scale_, tower->rotation_, tower->translation_);
		tower->TransferMatrix();
	}
	// Update Matriks Jendela
	for (auto& window : transformWindows_) {
		window->matWorld_ = Mathematics::MakeAffineMatrix(window->scale_, window->rotation_, window->translation_);
		window->TransferMatrix();
	}
	// Update Matriks Langit-langit
	for (auto& ceiling : transformCeilings_) {
		ceiling->matWorld_ = Mathematics::MakeAffineMatrix(ceiling->scale_, ceiling->rotation_, ceiling->translation_);
		ceiling->TransferMatrix();
	}
	// Update Matriks Lantai
	for (auto& floor : transformFloors_) {
		floor->matWorld_ = Mathematics::MakeAffineMatrix(floor->scale_, floor->rotation_, floor->translation_);
		floor->TransferMatrix();
	}
	// Update Matriks Dinding
	for (auto& wall : transformWalls_) {
		wall->matWorld_ = Mathematics::MakeAffineMatrix(wall->scale_, wall->rotation_, wall->translation_);
		wall->TransferMatrix();
	}
}

void BackgroundManager::Draw(KamataEngine::Camera& camera) {
	// 1. Gambar Gedung Luar Kota (Paling Belakang Z = 70)
	if (modelTower_) {
		for (auto* tower : transformTowers_) {
			if (tower) {
				modelTower_->Draw(*tower, camera);
			}
		}
	}
	// 2. Gambar Jendela Kaca (Z = 20)
	if (modelWindow_) {
		for (auto* window : transformWindows_) {
			if (window) {
				modelWindow_->Draw(*window, camera);
			}
		}
	}
	// 3. Gambar Langit-langit (Z = 10)
	if (modelCeiling_) {
		for (auto* ceiling : transformCeilings_) {
			if (ceiling) {
				modelCeiling_->Draw(*ceiling, camera);
			}
		}
	}
	// 4. Gambar Lantai (Z = 0)
	if (modelWallFloor_) {
		for (auto* floor : transformFloors_) {
			if (floor) {
				modelWallFloor_->Draw(*floor, camera);
			}
		}
	}
	// 5. Gambar Dinding (Z = 0)
	if (modelWallFloor_) {
		for (auto* wall : transformWalls_) {
			if (wall) {
				modelWallFloor_->Draw(*wall, camera);
			}
		}
	}
}
