#include "TitleScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;
void TitleScene::Initialize() {

	titleModel_ = Model::CreateFromOBJ("title", true);
	subTitleModel_ = Model::CreateFromOBJ("pressSpace", true);


	//Camera
	camera_ = new Camera();
	camera_->Initialize();
	camera_->translation_ = {0.0f, 0.0f, -10.0f};
	// Title
	worldTransformTitle_.Initialize();
	worldTransformTitle_.translation_ = {-3.0f, 1.5f, 0.0f};
	worldTransformTitle_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformTitle_.rotation_.y = Mathematics::DegreesToRadians(180.0f);
	// Press Space
	worldTransformSubTitle_.Initialize();
	worldTransformSubTitle_.translation_ = {-1.0f, -2.0f, 0.0f};
	worldTransformSubTitle_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformSubTitle_.rotation_.y = Mathematics::DegreesToRadians(180.0f);

	// Fade
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Status::FadeIn, 1.5f);

}

void TitleScene::Update() {
	camera_->UpdateMatrix();
	// Title Animation
	animationTimer_ += 0.05f;
	float floatingOffset = std::sin(animationTimer_) * 0.2f;
	worldTransformTitle_.translation_.y = 1.5f + floatingOffset;
	// Sub Title Animation
	worldTransformSubTitle_.rotation_.x += 1.7f;

	worldTransformTitle_.matWorld_ = Mathematics::MakeAffineMatrix(worldTransformTitle_.scale_, worldTransformTitle_.rotation_, worldTransformTitle_.translation_);
	worldTransformTitle_.TransferMatrix();
	worldTransformSubTitle_.matWorld_ = Mathematics::MakeAffineMatrix(worldTransformSubTitle_.scale_, worldTransformSubTitle_.rotation_, worldTransformSubTitle_.translation_);
	worldTransformSubTitle_.TransferMatrix();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Status::FadeOut, 1.5f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
	camera_->UpdateMatrix();
}

void TitleScene::Draw() {
	Model::PreDraw();
	if (titleModel_ != nullptr) {
		titleModel_->Draw(worldTransformTitle_, *camera_);
	}
	if (subTitleModel_ != nullptr) {
		subTitleModel_->Draw(worldTransformSubTitle_, *camera_);
	}
	Model::PostDraw();
	fade_->Draw();
}
