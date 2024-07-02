#pragma once

namespace Game
{
	// 画面サイズ
	constexpr int kWindowW = 1280;
	constexpr int kWindowH = 720;

	// FPS
	constexpr int kFPS = 16667;

	// 最大プレイ人数(ローカル)
	constexpr int kMaxPlayNum = 6;

	// 数学系
	constexpr double kPi = 3.14159265358979323846;
	constexpr float kPiF = 3.14159265358979323846f;
	constexpr float kDeg2Rad = kPiF / 180.0f;
	constexpr float kRad2Deg = 180.0f / kPiF;
}