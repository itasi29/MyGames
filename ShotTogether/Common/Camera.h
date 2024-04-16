#pragma once
#include <DxLib.h>
#include <memory>
#include "Geometry.h"

class ActorBase;

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
	Camera();							// コンストラクタ.
	~Camera();							// デストラクタ.

	void Update(const std::shared_ptr<ActorBase>& actor);	// 更新.

	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos.GetVECTOR(); }

private:
	Vec3	pos;			// ポジション.
};
