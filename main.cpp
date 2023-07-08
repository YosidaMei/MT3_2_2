#include <Novice.h>
#include "Matrix.h"
#include <imgui.h>
#include "Function.h"

const char kWindowTitle[] = "LD2A_11_ヨシダメイ";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate = {};
	Vector3 translate = { 0,0,0 };
	Vector3 cameraPosition = { 0,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0,0 };

	Sphere sphereA = { {0,0,0},0.5f };
	Plane plane = { {0,1,0},1 };
	uint32_t color = WHITE;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		//球
		if (keys[DIK_W]) {
			sphereA.center.z += 0.02f;
		}
		if (keys[DIK_S]) {
			sphereA.center.z -= 0.02f;
		}
		if (keys[DIK_A]) {
			sphereA.center.x -= 0.02f;
		}
		if (keys[DIK_D]) {
			sphereA.center.x += 0.02f;
		}
		if (keys[DIK_E]) {
			sphereA.center.y -= 0.02f;
		}
		if (keys[DIK_Q]) {
			sphereA.center.y += 0.02f;
		}
		//カメラの移動
		if (keys[DIK_UP]) {
			cameraPosition.y += 0.02f;
		}
		if (keys[DIK_DOWN]) {
			cameraPosition.y -= 0.02f;
		}
		if (keys[DIK_LEFT]) {
			cameraPosition.x -= 0.02f;
		}
		if (keys[DIK_RIGHT]) {
			cameraPosition.x += 0.02f;
		}

		if (IsCollisionS2P(sphereA, plane)==true) {
			color = RED;
		}
		else
		{
			color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawSphere(sphereA, worldViewProjectionMatrix, viewportMatrix, color);
		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, WHITE);

		//デバッグ
		ImGui::InputFloat3("sphereA.center", &sphereA.center.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat("sphereA.radius", &sphereA.radius);
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		plane.normal = Normalize(plane.normal);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
