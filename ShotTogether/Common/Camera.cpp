#include "Common/Camera.h"

namespace
{
	constexpr float kDefPosY = 3.0f;
	constexpr float kDefPosZ = -10.0f;

	constexpr float kLerpRate = 0.1f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	pos = Vec3(0.0f, kDefPosY, kDefPosZ);
	SetCameraPositionAndTarget_UpVecY(pos.GetVECTOR(), VGet(0.0f, kDefPosY, 1.0f));
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	// 処理なし.
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const std::shared_ptr<ActorBase>& actor)
{
	// MEMO:今回はカメラは動かさない
}