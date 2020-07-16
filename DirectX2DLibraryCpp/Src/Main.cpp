#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>

//========================================================
// グローバル変数
//========================================================
Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;
int g_playerHand = -1;
int g_enemyHand;
int result = 0;
int next = -1;

//========================================================
// プロトタイプ宣言
//========================================================
void GameProcessing();		// ゲーム処理
void DrawProcessing();		// 描画処理
void PlayerHand();			// プレイヤーの手処理
void EnemyHand();			// 相手の手処理
void ResultScreen();		// 結果処理
void InputByKeyboard();		// キーボード入力処理


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (Engine::Initialize(640, 480, "Sample") == false)
	{
		return 0;
	}

	srand((unsigned)time(NULL));
	g_enemyHand = rand() % 3;

	Engine::LoadTexture("rook", "Res/gu.png");
	Engine::LoadTexture("scisers", "Res/choki.png");
	Engine::LoadTexture("paper", "Res/pa.png");

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ゲーム処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	Engine::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;
}
void GameProcessing()
{
	// 入力データの更新
	Engine::Update();
	
	
	//========================================================
	// キーボードの入力取得
	//========================================================
	if (next == -1)
	{
		InputByKeyboard();
	}
	else if (next == 0)
	{
		if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
		{
			next = 1;
		}
	}
	else if (next == 1)
	{
		
		if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
		{
			g_playerHand = -1;
			next = -1;
			srand((unsigned)time(NULL));
			g_enemyHand = rand() % 3;
		}
	}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// テクスチャ描画		
	
	if (next == -1)
	{
		Engine::DrawFont(180.0f, 130.0f, "ジャンケンゲーム", FontSize::Large, FontColor::White);
		Engine::DrawFont(140.0f, 270.0f, "グー(0)かチョキ(1)かパー(2)\n\n    を入力してください", FontSize::Regular, FontColor::White);
	}
	else if(next == 0)
	{
		// 自分の手の表示
		PlayerHand();

		// 相手の手の表示
		EnemyHand();

		Engine::DrawFont(200.0f, 380.0f, "スペースで次へ", FontSize::Large, FontColor::White);
	}
	else if (next == 1)
	{
		ResultScreen();
		Engine::DrawFont(150.0f, 270.0f, "スペースでもう一回できるよ！", FontSize::Regular, FontColor::White);
	}
	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

void InputByKeyboard()
{
	if (Engine::IsKeyboardKeyPushed(DIK_0) == true)
	{
		g_playerHand = 0;
		next = 0;
	}

	else if (Engine::IsKeyboardKeyPushed(DIK_1) == true)
	{
		g_playerHand = 1;
		next = 0;
	}

	else if (Engine::IsKeyboardKeyPushed(DIK_2) == true)
	{
		g_playerHand = 2;
		next = 0;
	}

}

void PlayerHand()
{
	Engine::DrawFont(135.0f, 100.0f, "自分の手", FontSize::Large, FontColor::White);
	switch (g_playerHand)
	{
	case 0:		
		Engine::DrawTexture(70, 130, "rook", 255, 0.0f, 0.6f, 0.6f);
		break;

	case 1:
		Engine::DrawTexture(-65, 0, "scisers", 255, 0.0f, 0.5f, 0.4f);
		break;

	case 2:
		Engine::DrawTexture(-50, 0, "paper", 255, 0.0f, 0.4f, 0.4f);
		break;
	}
}

void EnemyHand()
{
	Engine::DrawFont(400.0f, 100.0f, "相手の手", FontSize::Large, FontColor::White);
	switch (g_enemyHand)
	{
	case 0:
		Engine::DrawTexture(330, 130, "rook", 255, 0.0f, 0.6f, 0.6f);
		break;

	case 1:
		Engine::DrawTexture(205, 0, "scisers", 255, 0.0f, 0.5f, 0.4f);
		break;

	case 2:
		Engine::DrawTexture(220, 0, "paper", 255, 0.0f, 0.4f, 0.4f);
		break;
	}
}

void ResultScreen()
{
	result = ((3 + g_enemyHand) - g_playerHand) % 3;
	if (result == 0)
	{
		Engine::DrawFont(260.0f, 130.0f, "あいこ!", FontSize::Large, FontColor::White);
		Engine::DrawFont(180.0f, 190.0f, "次は勝ちましょう！", FontSize::Large, FontColor::White);
	}
	else if (result == 1)
	{
		Engine::DrawFont(190.0f, 130.0f, "プレイヤーの勝ち!\n\n     すごい！", FontSize::Large, FontColor::White);
	}
	else if (result == 2)
	{
		Engine::DrawFont(180.0f, 130.0f, "プレイヤーの負け!\n\n次は勝てますように", FontSize::Large, FontColor::White);
	}
	
}

