#include <Novice.h>
#include <cmath>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

struct Vector3
{
	float x, y, z;
};

struct Matrix4x4
{
	float m[4][4];
};

Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 scaleMatrix{};
	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[2][2] = scale.z;
	scaleMatrix.m[3][3] = 1.0f;
	return scaleMatrix;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4 rotateXMatrix{};
	rotateXMatrix.m[0][0] = 1.0f;
	rotateXMatrix.m[1][1] = cosTheta;
	rotateXMatrix.m[1][2] = sinTheta;
	rotateXMatrix.m[2][1] = -sinTheta;
	rotateXMatrix.m[2][2] = cosTheta;
	rotateXMatrix.m[3][3] = 1.0f;
	return rotateXMatrix;
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4 rotateYMatrix{};
	rotateYMatrix.m[0][0] = cosTheta;
	rotateYMatrix.m[0][2] = -sinTheta;
	rotateYMatrix.m[1][1] = 1.0f;
	rotateYMatrix.m[2][0] = sinTheta;
	rotateYMatrix.m[2][2] = cosTheta;
	rotateYMatrix.m[3][3] = 1.0f;
	return rotateYMatrix;
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4 rotateZMatrix{};
	rotateZMatrix.m[0][0] = cosTheta;
	rotateZMatrix.m[0][1] = sinTheta;
	rotateZMatrix.m[1][0] = -sinTheta;
	rotateZMatrix.m[1][1] = cosTheta;
	rotateZMatrix.m[2][2] = 1.0f;
	rotateZMatrix.m[3][3] = 1.0f;
	return rotateZMatrix;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = m1.m[i][0] * m2.m[0][j] +
				m1.m[i][1] * m2.m[1][j] +
				m1.m[i][2] * m2.m[2][j] +
				m1.m[i][3] * m2.m[3][j];
		}
	}
	return result;
}

Matrix4x4 MakeRotationMatrix(const Vector3& rotate)
{
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 result = Multiply(Multiply(rotateX, rotateY), rotateZ);
	return result;
}

Matrix4x4 MakeTranslationMatrix(const Vector3& translate)
{
	Matrix4x4 translationMatrix{
	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};

	translationMatrix.m[3][0] = translate.x;
	translationMatrix.m[3][1] = translate.y;
	translationMatrix.m[3][2] = translate.z;
	return translationMatrix;
}

Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
{
	return Multiply(a, b);
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotationMatrix = MakeRotationMatrix(rotate);
	Matrix4x4 translationMatrix = MakeTranslationMatrix(translate);

	Matrix4x4 affineMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	return affineMatrix;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + row * kRowHeight,
				"%6.02f", matrix.m[row][column]);
		}
	}
}
const char kWindowTitle[] = "GC2B_14_ハラ_アイ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 scale{ 1.2f,0.79f,-2.1f };
	Vector3 rotate{ 0.4f,1.43f,-0.8f };
	Vector3 translate{ 2.7f,-4.15f,1.57f };
	Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, worldMatrix);

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
