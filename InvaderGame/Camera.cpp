#include "Camera.h"

#include "GameSystem.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"

const float Camera::Magnification = 24.0f;

// 最終的にはカメラパラメータを考慮して実装する必要がある。
// 3D環境開発時に行う
Vector3 Camera::WorldToViewportPoint(Vector3 worldPos)
{
	Matrix matrix = Matrix();
	matrix.g_matrix = {
		{Magnification / (GameSystem::WINDOW_WIDTH / 2.0f), 0, 0},
		{0, Magnification / (GameSystem::WINDOW_HEIGHT / 2.0f), 0},
		{0, 0, 1}
	};

	Matrix matrix_pos = Matrix::ConvertToMatrix(worldPos);

	Matrix result = matrix * matrix_pos;

	return Vector3(result.g_matrix[0][0], result.g_matrix[1][0], result.g_matrix[2][0]);
}

Vector2 Camera::WorldToBox2DWorld(Vector3 worldPos)
{
	Matrix matrix = Matrix();
	matrix.g_matrix = {
		{Magnification / 60.0f, 0, 0},
		{0, Magnification / 60.0f, 0},
		{0, 0, Magnification / 60.0f}
	};

	Matrix matrix_pos = Matrix::ConvertToMatrix(worldPos);

	Matrix result = matrix * matrix_pos;

	return Vector2(result.g_matrix[0][0], result.g_matrix[1][0]);
}

Vector3 Camera::Box2DWorldToWorld(Vector2 worldPos)
{
	Matrix matrix = Matrix();
	matrix.g_matrix = {
		{60.0f / Magnification, 0, 0},
		{0, 60.0f / Magnification, 0},
		{0, 0, 60.0f / Magnification}
	};

	Matrix matrix_pos = Matrix::ConvertToMatrix(worldPos.ToVector3());

	Matrix result = matrix * matrix_pos;

	return Vector3(result.g_matrix[0][0], result.g_matrix[1][0], result.g_matrix[2][0]);
}