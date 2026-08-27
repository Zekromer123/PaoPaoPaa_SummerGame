#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include <Windows.h>

enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kGameOver,
	kGameClear,
};

Scene scene = Scene::kUnknown;
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameClearScene* gameClearScene = nullptr;
GameOverScene* gameOverScene = nullptr;
uint32_t bgmHandle = 0;
uint32_t bgmVoiceHandle = 0;


void ChangeScene()
{
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->isFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene && gameScene->IsFinished()) {
			if (gameScene->IsGameOver()) {
				scene = Scene::kGameOver;
				delete gameScene;
				gameScene = nullptr;
				Audio::GetInstance()->StopWave(bgmVoiceHandle);
				bgmVoiceHandle = 0;
				gameOverScene = new GameOverScene();
				gameOverScene->Initialize();
			} else if (gameScene->IsGameClear()) {
				scene = Scene::kGameClear;
				delete gameScene;
				Audio::GetInstance()->StopWave(bgmVoiceHandle);
				bgmVoiceHandle = 0;
				gameScene = nullptr;
				gameClearScene = new GameClearScene();
				gameClearScene->Initialize();
			}
		}
		break;
	case Scene::kGameOver:
		if (gameOverScene->isFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			if (bgmVoiceHandle == 0) {
				bgmVoiceHandle = Audio::GetInstance()->PlayWave(bgmHandle, true, 0.5f);
			}
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	case Scene::kGameClear:
		if (gameClearScene->isFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameClearScene = nullptr;
			if (bgmVoiceHandle == 0) {
				bgmVoiceHandle = Audio::GetInstance()->PlayWave(bgmHandle, true, 0.5f);
			}
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	default:
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameOver:
		gameOverScene->Update();
		break;
	case Scene::kGameClear:
		gameClearScene->Update();
		break;
	default:
		break;
	}
}
void DrawScene() {
	switch (scene) {

	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	case Scene::kGameClear:
		gameClearScene->Draw();
		break;
	default:
		break;
	}
};


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	using namespace KamataEngine;
	Initialize(L"パオパオパー");
	DirectXCommon* dxCommonPtr = DirectXCommon::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	scene = Scene::kTitle;

	titleScene = new TitleScene();
	titleScene->Initialize();

	bgmHandle = Audio::GetInstance()->LoadWave("SFX/paopaoBGM.wav");
	bgmVoiceHandle = Audio::GetInstance()->PlayWave(bgmHandle, true, 0.3f);



	while (true) {
		imguiManager->Begin();
		if (Update()) {
			break;
		}
		ChangeScene();
		UpdateScene();
		imguiManager->End();
		// 画面がここから
		dxCommonPtr->PreDraw();
		DrawScene();
		imguiManager->Draw();
		// ここまで
		dxCommonPtr->PostDraw();
	}
	delete titleScene;
	delete gameScene;
	titleScene = nullptr;
	gameScene = nullptr;
	Finalize();

	return 0;
}
