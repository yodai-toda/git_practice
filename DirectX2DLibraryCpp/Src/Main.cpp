#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;
int g_playerHand = -1;
int g_enemyHand;
int next = -1;
// DWORD g_StartTime = 0;        // ゲームを起動したときの時間を持つ変数
// DWORD g_ElapsedTime = 0;    // ゲームを起動してからの経過時間を持つ変数

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();


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
	// g_StartTime = timeGetTime();    // timeGetTimeは現在時刻を表す関数

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
	if (g_playerHand == -1)
	{
		if (Engine::IsKeyboardKeyPushed(DIK_0) == true)
		{
			g_playerHand = 0;
		}

		else if (Engine::IsKeyboardKeyPushed(DIK_1) == true)
		{
			g_playerHand = 1;
		}

		else if (Engine::IsKeyboardKeyPushed(DIK_2) == true)
		{
			g_playerHand = 2;
		}
		next = 0;
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
	//if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
	//{
	//	// キーが押された瞬間の処理
	//}

	//if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	//{
	//	// キーが押されている間の処理
	//}

	//if (Engine::IsKeyboardKeyReleased(DIK_A))
	//{
	//	// キーが離された瞬間の処理
	//}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);


	// テクスチャ描画	
	
	
	if (g_playerHand == -1)
	{
		Engine::DrawFont(180.0f, 130.0f, "ジャンケンゲーム", FontSize::Large, FontColor::White);
		Engine::DrawFont(140.0f, 270.0f, "グー(0)かチョキ(1)かパー(2)\n\n    を入力してください", FontSize::Regular, FontColor::White);
	}
	else if(next != 1)
	{
		// 自分の手の表示
		Engine::DrawFont(160.0f, 100.0f, "自分の手", FontSize::Large, FontColor::White);
		if (g_playerHand == 0)
		{
			Engine::DrawTexture(90, 130, "rook", 255, 0.0f, 0.6f, 0.6f);
		}
		else if (g_playerHand == 1)
		{
			Engine::DrawTexture(-35, 0, "scisers", 255, 0.0f, 0.5f, 0.4f);
		}
		else if (g_playerHand == 2)
		{
			Engine::DrawTexture(-20, 0, "paper", 255, 0.0f, 0.4f, 0.4f);
		}

		// 相手の手の表示
		Engine::DrawFont(400.0f, 100.0f, "相手の手", FontSize::Large, FontColor::White);
		if (g_enemyHand == 0)
		{
			Engine::DrawTexture(330, 130, "rook", 255, 0.0f, 0.6f, 0.6f);
		}
		else if (g_enemyHand == 1)
		{
			Engine::DrawTexture(205, 0, "scisers", 255, 0.0f, 0.5f, 0.4f);
		}
		else if (g_enemyHand == 2)
		{
			Engine::DrawTexture(220, 0, "paper", 255, 0.0f, 0.4f, 0.4f);
		}

		Engine::DrawFont(200.0f, 380.0f, "スペースで次へ", FontSize::Large, FontColor::White);
	}
	else if (next == 1)
	{
		if (g_playerHand == g_enemyHand)
		{
			Engine::DrawFont(270.0f, 200.0f, "あいこ!", FontSize::Large, FontColor::White);
		}
		else if (g_playerHand == 0 && g_enemyHand == 1 || g_playerHand == 1 && g_enemyHand == 2 || g_playerHand == 2 && g_enemyHand == 0)
		{
			Engine::DrawFont(200.0f, 200.0f, "プレイヤーの勝ち!", FontSize::Large, FontColor::White);
		}
		else if (g_playerHand == 0 && g_enemyHand == 2 || g_playerHand == 1 && g_enemyHand == 0 || g_playerHand == 2 && g_enemyHand == 1)
		{
			Engine::DrawFont(230.0f, 200.0f, "相手の勝ち!", FontSize::Large, FontColor::White);
		}
		Engine::DrawFont(140.0f, 270.0f, "スペースでもう一回できるよ！", FontSize::Regular, FontColor::White);
	}
	
	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

