#include "Skydome.h"
#include "Mathematics.h"

Skydome::~Skydome() {
	if (mModel) {
		delete mModel;
		mModel = nullptr;
	}
}

void Skydome::Initialize() {
	mModel = KamataEngine::Model::CreateFromOBJ("SkyDome",true);
	mWorldTransform.Initialize();
	mWorldTransform.scale_ = {100.0f, 100.0f, 100.0f};
	mWorldTransform.translation_ = {50.0f, -10.0f, 0.0f};
}

void Skydome::Update() 
{
	mWorldTransform.matWorld_ = Mathematics::MakeAffineMatrix(
		mWorldTransform.scale_, 
		mWorldTransform.rotation_, 
		mWorldTransform.translation_
	);
	mWorldTransform.TransferMatrix(); 
}

void Skydome::Draw(KamataEngine::Camera& camera) { 
	 if (mModel) {
		mModel->Draw(mWorldTransform, camera);
	}
}
