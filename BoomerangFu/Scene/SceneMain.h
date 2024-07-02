#pragma once
#include "SceneBase.h"
class SceneMain final : public SceneBase
{
public:
	SceneMain();
	~SceneMain() {}

	// シーンに入るときの初期化処理
	void Init() override;
	// 毎フレーム行う更新処理
	void Update(bool isFade) override;
	// 毎フレーム行う描画処理
	void Draw() const override;
	// シーンを抜けるときの処理
	void End() override;
	// シーン変更処理
	std::shared_ptr<SceneBase> Change() override;
};

