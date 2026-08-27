#include "Fade.h"
#include <KamataEngine.h>

using namespace KamataEngine;

void Fade::Initialize() {
	textureHandle = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(textureHandle, Vector2(0, 0));
	sprite_->SetSize(Vector2(1280, 720));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() 
{
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(1.0f - (counter_ / duration_), 0.0f, 1.0f)));
		break;
	case Status::FadeOut:
		counter_ += 1.0f / 60.0f;
		if (counter_>= duration_) 
		{
			counter_ = duration_;
			
		}
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	default:
		break;
	}
}

void Fade::Draw() 
{
	if (status_==Status::None) {
		return;
	}
	Sprite::PreDraw();
	sprite_->Draw();
	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration)
{
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() 
{ status_ = Status::None; }

bool Fade::IsFinished() const
{ 
	switch (status_) {
	case Status::FadeIn:
	case Status::FadeOut:
		if (counter_>= duration_) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}
