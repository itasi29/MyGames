#include <DxLib.h>
#include "Piece.h"

namespace
{
	// îºåa
	constexpr int kRadius = 20;

	// ÉJÉâÅ[
	const unsigned int kBlackColor = GetColor(0, 0, 0);
	const unsigned int kWhiteColor = GetColor(255, 255, 255);
}

Piece::Piece():
	m_posX(0),
	m_posY(0)
{

}

Piece::~Piece()
{

}

void Piece::Init()
{

}

void Piece::Update()
{

}

void Piece::Draw() const
{
	DrawCircle(m_posX, m_posY, kRadius, kWhiteColor, true);
}