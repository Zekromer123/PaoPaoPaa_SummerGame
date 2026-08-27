#include "GameScene.h"
#include "Mathematics.h"
#include <cassert>
using namespace KamataEngine;
GameScene::~GameScene() {
	player_.reset();
	delete spriteHpBar_;
	delete spriteHpFillGreen_;
	delete spriteHpFillYellow_;
	delete spriteHpFillRed_;
	delete spriteHpIcon_;
	delete modelBody_;
	delete modelHead_;
	delete modelLeftArm_;
	delete modelRightArm_;
	delete modelLeftForearm_;
	delete modelRightForearm_;
	delete modelLeftThigh_;
	delete modelLeftCalf_;
	delete modelRightThigh_;
	delete modelRightCalf_;
	delete blockModel_;
	delete debugCamera_;
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	// Camera
	camera_.Initialize();
	camera_.farZ = 100.0f;
	// Debug Camera
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(1000.0f);

	// Player
	modelBody_ = Model::CreateFromOBJ("Body", true);
	modelHead_ = Model::CreateFromOBJ("Head", true);
	modelLeftArm_ = Model::CreateFromOBJ("Left_Arm", true);
	modelLeftForearm_ = Model::CreateFromOBJ("Left_Forearm", true);
	modelRightArm_ = Model::CreateFromOBJ("Right_Arm", true);
	modelRightForearm_ = Model::CreateFromOBJ("Right_Forearm", true);
	modelLeftThigh_ = Model::CreateFromOBJ("Left_Thigh", true);
	modelLeftCalf_ = Model::CreateFromOBJ("Left_Calf", true);
	modelRightThigh_ = Model::CreateFromOBJ("Right_Thigh", true);
	modelRightCalf_ = Model::CreateFromOBJ("Right_Calf", true);
	// Melee
	modelMeleeBody_ = Model::CreateFromOBJ("meleeBody", true);
	modelMeleeHead_ = Model::CreateFromOBJ("meleeHead", true);
	modelMeleeLeftArm_ = Model::CreateFromOBJ("meleeLeft_Arm", true);
	modelMeleeLeftForearm_ = Model::CreateFromOBJ("meleeLeft_Forearm", true);
	modelMeleeRightArm_ = Model::CreateFromOBJ("meleeRight_Arm", true);
	modelMeleeRightForearm_ = Model::CreateFromOBJ("meleeRight_Forearm", true);
	modelMeleeLeftThigh_ = Model::CreateFromOBJ("meleeLeft_Thigh", true);
	modelMeleeLeftCalf_ = Model::CreateFromOBJ("meleeLeft_Calf", true);
	modelMeleeRightThigh_ = Model::CreateFromOBJ("meleeRight_Thigh", true);
	modelMeleeRightCalf_ = Model::CreateFromOBJ("meleeRight_Calf", true);
	// Range
	modelRangeBody_ = Model::CreateFromOBJ("rangeBody", true);
	modelRangeHead_ = Model::CreateFromOBJ("rangeHead", true);
	modelRangeLeftArm_ = Model::CreateFromOBJ("rangeLeft_Arm", true);
	modelRangeLeftForearm_ = Model::CreateFromOBJ("rangeLeft_Forearm", true);
	modelRangeRightArm_ = Model::CreateFromOBJ("rangeRight_Arm", true);
	modelRangeRightForearm_ = Model::CreateFromOBJ("rangeRight_Forearm", true);
	modelRangeLeftThigh_ = Model::CreateFromOBJ("rangeLeft_Thigh", true);
	modelRangeLeftCalf_ = Model::CreateFromOBJ("rangeLeft_Calf", true);
	modelRangeRightThigh_ = Model::CreateFromOBJ("rangeRight_Thigh", true);
	modelRangeRightCalf_ = Model::CreateFromOBJ("rangeRight_Calf", true);
	// Boss
	modelBossBody_ = Model::CreateFromOBJ("bossBody", true);
	modelBossHead_ = Model::CreateFromOBJ("bossHead", true);
	modelBossLeftArm_ = Model::CreateFromOBJ("bossLeft_Arm", true);
	modelBossLeftForearm_ = Model::CreateFromOBJ("bossLeft_Forearm", true);
	modelBossRightArm_ = Model::CreateFromOBJ("bossRight_Arm", true);
	modelBossRightForearm_ = Model::CreateFromOBJ("bossRight_Forearm", true);
	modelBossLeftThigh_ = Model::CreateFromOBJ("bossLeft_Thigh", true);
	modelBossLeftCalf_ = Model::CreateFromOBJ("bossLeft_Calf", true);
	modelBossRightThigh_ = Model::CreateFromOBJ("bossRight_Thigh", true);
	modelBossRightCalf_ = Model::CreateFromOBJ("bossRight_Calf", true);

	texHpBar_ = TextureManager::Load("HpUI/hpBar.png");
	texHpIcon_ = TextureManager::Load("HpUI/hpIcon.png");
	texHpFillGreen_ = TextureManager::Load("HpUI/greenBar.png");
	texHpFillYellow_ = TextureManager::Load("HpUI/yellowBar.png");
	texHpFillRed_ = TextureManager::Load("HpUI/redBar.png");

	spriteHpIcon_ = Sprite::Create(texHpIcon_, Vector2(30.0f, 30.0f));
	spriteHpBar_ = Sprite::Create(texHpBar_, Vector2(95.0f, 35.0f));
	spriteHpFillGreen_ = Sprite::Create(texHpFillGreen_, Vector2(95.0f, 35.0f));
	spriteHpFillYellow_ = Sprite::Create(texHpFillYellow_, Vector2(95.0f, 35.0f));
	spriteHpFillRed_ = Sprite::Create(texHpFillRed_, Vector2(95.0f, 35.0f));
	if (spriteHpFillGreen_) {
		hpFillBaseSize_ = spriteHpFillGreen_->GetSize();
	}

	attackUI_ = TextureManager::Load("zxc.png");
	spriteAttackUI_ = Sprite::Create(attackUI_, Vector2(100.0f, 30.0f));

	// Enemy
	projectileModel_ = Model::CreateFromOBJ("Bullet", true);

	cameraController_ = std::make_unique<CameraController>();
	// MapChip
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCSV("Resources/field01.csv");
	// blockModel_ = Model::CreateFromOBJ("block", true);

	// Skydome
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	backgroundManager_ = std::make_unique<BackgroundManager>();
	backgroundManager_->Initialize();

	GenerateFieldObjects();

	cameraController_->Initialize(player_.get());
	cameraController_->SetTarget(player_.get());
	cameraController_->Reset();
}

void GameScene::Update() {
	debugCamera_->Update();
	if (skydome_) {
		skydome_->Update();
	}
	if (backgroundManager_) {
		backgroundManager_->Update();
	}
	if (player_) {
		player_->Update();
	}
	if (cameraController_) {
		cameraController_->Update();
	}

	for (auto& enemy : enemies_) {
		if (enemy) {
			enemy->Update(player_.get(), projectiles_);
		}
	}
	for (auto& proj : projectiles_) {
		if (proj) {
			proj->Update();
		}
	}

	// PLayer and Enemy Collision Detection
	if (player_ && player_->IsHitboxActive()) {
		AABB playerHitbox = player_->GetAttackHitboxAABB();
		int attackDamage = player_->GetAttackDamage();
		for (auto& enemy : enemies_) {
			if (enemy && !enemy->IsDying()) {
				AABB enemyAABB = enemy->GetAABB();
				if (Mathematics::IsCollision(playerHitbox, enemyAABB)) {
					float knockDir = (player_->GetDirection() == Player::LRDirection::kRight) ? +0.25f : -0.25f;
					enemy->TakeDamage(attackDamage, {knockDir, 0.2f, 0.0f});
				}
			}
		}
	}
	// Player with Enemy Collision Detection
	if (player_ && !player_->IsDead()) {
		AABB playerAABB = player_->GetAABB();
		for (auto& enemy : enemies_) {
			if (enemy && !enemy->IsDying() && enemy->IsAttacking()) {
				AABB enemyAABB = enemy->GetAABB();
				if (Mathematics::IsCollision(enemyAABB, playerAABB)) {
					int enemyDmg = (enemy->GetType() == EnemyType::kBoss) ? 20 : 10;
					float knockDir = (enemy->GetWorldPosition().x < player_->GetWorldPosition().x) ? +0.3f : -0.3f;
					player_->TakeDamage(enemyDmg, {knockDir, 0.2f, 0.0f});
				}
			}
		}
	}
	// Player with Enemy Projectile Collision Detection
	if (player_ && !player_->IsDead()) {
		AABB playerAABB = player_->GetAABB();
		for (auto& proj : projectiles_) {
			if (proj && !proj->IsDead()) {
				AABB projAABB = proj->GetAABB();
				if (Mathematics::IsCollision(projAABB, playerAABB)) {
					float knockDir = (proj->GetAABB().min.x < playerAABB.min.x) ? +0.25f : -0.25f;
					player_->TakeDamage(10, {knockDir, 0.1f, 0.0f});
					proj->SetDead();
				}
				
			}
		}
	}
	if (player_) {
		float hpRatio = (float)player_->GetHp() / (float)player_->GetMaxHp();
		hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);
		Vector2 currentSize = Vector2(hpFillBaseSize_.x * hpRatio, hpFillBaseSize_.y);
		if (spriteHpFillGreen_)
			spriteHpFillGreen_->SetSize(currentSize);
		if (spriteHpFillYellow_)
			spriteHpFillYellow_->SetSize(currentSize);
		if (spriteHpFillRed_)
			spriteHpFillRed_->SetSize(currentSize);
	}
	// Game Over Condition
	if (player_ && player_->IsDead()) {
		sceneTransitionTimer_++;
		if (sceneTransitionTimer_ >= 90) { 
			isFinished_ = true;
			isGameOver_ = true; 
		}
	}
	// Game Clear Condition
	bool isBossDefeated = false;
	for (auto& enemy : enemies_) {
		if (enemy && enemy->GetType() == EnemyType::kBoss && enemy->IsDead()) {
			isBossDefeated = true;
			break;
		}
	}
	if (isBossDefeated) {
		sceneTransitionTimer_++;
		if (sceneTransitionTimer_ >= 90) {
			isFinished_ = true;
			isGameClear_ = true; 
		}
	}

#ifdef _DEBUG
	if (player_) {
		ImGui::Begin("Player Status Debug");
		ImGui::Text("State Saat Ini: %s", player_->GetStateName());
		ImGui::Text("Sedang Menyerang: %s", player_->IsAttacking() ? "YA" : "TIDAK");
		ImGui::Text("Hitbox Aktif: %s", player_->IsHitboxActive() ? "AKTIF" : "OFF");
		ImGui::End();
	}
	// 4. UI Health Bar Boss (ImGui)
	for (auto& enemy : enemies_) {
		if (enemy && enemy->GetType() == EnemyType::kBoss && !enemy->IsDying()) {
			ImGui::Begin("BOSS HEALTH");
			float hpRatio = (float)enemy->GetHP() / (float)enemy->GetMaxHP();
			ImGui::ProgressBar(hpRatio, ImVec2(300, 25), "BOSS HP");
			ImGui::End();
		}
	}
	if (player_) {
		ImGui::Begin("PLAYER STATUS");
		float hpRatio = (float)player_->GetHp() / (float)player_->GetMaxHp();
		ImGui::Text("HP: %d / %d", player_->GetHp(), player_->GetMaxHp());
		ImGui::ProgressBar(hpRatio, ImVec2(250, 20), "PLAYER HP");
		if (player_->IsInvincible()) {
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "[ INVINCIBLE ]");
		}
		ImGui::End();
	}
	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	if (isDebugCameraActive_) {
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
	} else {
		camera_.matView = cameraController_->GetCamera().matView;
		camera_.matProjection = cameraController_->GetCamera().matProjection;
	}
#else  // Release Mode
	camera_.matView = cameraController_->GetCamera().matView;
	camera_.matProjection = cameraController_->GetCamera().matProjection;
#endif // _DEBUG

	// camera_.matView = cameraController_->GetCamera().matView;
	// camera_.matProjection = cameraController_->GetCamera().matProjection;
	camera_.TransferMatrix();

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = Mathematics::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {
	Model::PreDraw();

	if (skydome_) {
		skydome_->Draw(camera_);
	}
	if (backgroundManager_) {
		backgroundManager_->Draw(camera_);
	}

	// for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	//	for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {
	//		if (!worldTransformBlock) {
	//			continue;
	//		}
	//		if (blockModel_) {
	//			blockModel_->Draw(*worldTransformBlock, camera_);
	//		}
	//	}
	// }

	if (player_) {
		player_->Draw();
	}

	for (auto& enemy : enemies_) {
		if (enemy) {
			enemy->Draw();
		}
	}
	for (auto& proj : projectiles_) {
		if (proj) {
			proj->Draw();
		}
	}

	Model::PostDraw();

	Sprite::PreDraw();
	if (player_) {
		float hpRatio = (float)player_->GetHp() / (float)player_->GetMaxHp();
		if (hpRatio > 0.65f) {
			if (spriteHpFillGreen_)
				spriteHpFillGreen_->Draw();
		} else if (hpRatio > 0.45f) {
			if (spriteHpFillYellow_)
				spriteHpFillYellow_->Draw();
		} else {
			if (spriteHpFillRed_)
				spriteHpFillRed_->Draw();
		}
	}
	if (spriteHpBar_)
		spriteHpBar_->Draw();
	if (spriteHpIcon_)
		spriteHpIcon_->Draw();

	if (spriteAttackUI_)
		spriteAttackUI_->Draw();
	Sprite::PostDraw();
}

void GameScene::CheckAllCollisions() {}

void GameScene::GenerateFieldObjects() {
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	worldTransformBlocks_.resize(numBlockVertical);
	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			MapChipType type = mapChipField_->GetMapChipTypeByIndex(j, i);
			switch (type) {
			case MapChipType::kBlock: {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				worldTransform->matWorld_ = Mathematics::MakeAffineMatrix(worldTransform->scale_, worldTransform->rotation_, worldTransform->translation_);
				worldTransform->TransferMatrix();
				worldTransformBlocks_[i][j] = worldTransform;
				break;
			}
			case MapChipType::kPlayer: {
				assert(player_ == nullptr && "自キャラを二重に配置しようとしています");
				Vector3 position = mapChipField_->GetMapChipPositionByIndex(j, i);
				player_ = std::make_unique<Player>();
				player_->Initialize(
				    modelBody_, modelHead_, modelLeftArm_, modelLeftForearm_, modelRightArm_, modelRightForearm_, modelLeftThigh_, modelLeftCalf_, modelRightThigh_, modelRightCalf_, &camera_,
				    position);
				player_->SetMapChipField(mapChipField_.get());
				break;
			}
			case MapChipType::kEnemy: {
				Vector3 position = mapChipField_->GetMapChipPositionByIndex(j, i);
				uint8_t subID = mapChipField_->GetMapChipSubIDByIndex(j, i);
				EnemyType enemyType = EnemyType::kMeleeMob;
				auto enemy = std::make_unique<Enemy>();
				if (subID == 1) {
					enemyType = EnemyType::kRangedMob; 
					enemy->Initialize(
					    modelRangeBody_, modelRangeHead_, modelRangeLeftArm_, modelRangeLeftForearm_, modelRangeRightArm_, modelRangeRightForearm_, modelRangeLeftThigh_, modelRangeLeftCalf_, modelRangeRightThigh_, modelRangeRightCalf_,
					    projectileModel_, &camera_, position, enemyType);
				} else if (subID == 2) {
					enemyType = EnemyType::kBoss; 
					enemy->Initialize(
					    modelBossBody_, modelBossHead_, modelBossLeftArm_, modelBossLeftForearm_, modelBossRightArm_, modelBossRightForearm_, modelBossLeftThigh_, modelBossLeftCalf_, modelBossRightThigh_, modelBossRightCalf_,
					    projectileModel_, &camera_, position, enemyType);
				} else {
				enemy->Initialize(
				    modelMeleeBody_, modelMeleeHead_, modelMeleeLeftArm_, modelMeleeLeftForearm_, modelMeleeRightArm_, modelMeleeRightForearm_, modelMeleeLeftThigh_, modelMeleeLeftCalf_, modelMeleeRightThigh_, modelMeleeRightCalf_, projectileModel_,
				    &camera_, position, enemyType);
				}
				enemy->SetMapChipField(mapChipField_.get());
				enemies_.push_back(std::move(enemy));
				break;
			}
			}
		}
	}
}
