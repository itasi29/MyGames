#include "Draw.h"
#include <DxLib.h>
#include <array>

void MyEngine::DrawGraph(int x, int y, int handle, int psH, bool isTrans, int secondH, int thirdH, int forceH)
{
	int w, h;
	GetGraphSize(handle, &w, &h);

	std::array<VERTEX2DSHADER, 4> vertices;

	for (auto& v : vertices)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(0, 0, 0, 0);
		v.rhw = 1.0f;
	}

	// ��`�ɂȂ�悤�ɕ��ׂĂ���
	// Z�̎��ɂȂ�悤�ɕ��ׂ܂�
	// ����
	vertices[0].pos = VGet(x, y, 0.0f);
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;
	// �E��
	vertices[1].pos = VGet(x + w, y, 0.0f);
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	// ����
	vertices[2].pos = VGet(x, y + h, 0.0f);
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;
	// �E��
	vertices[3].pos = VGet(x + w, y + h, 0.0f);
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;

	// ����
	if (isTrans)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	// �s����
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	}
	// �e�N�X�`���ݒ�
	SetUseTextureToShader(0, handle);
	if (secondH != -1)
	{
		SetUseTextureToShader(1, secondH);
	}
	if (thirdH != -1)
	{
		SetUseTextureToShader(2, thirdH);
	}
	if (forceH != -1)
	{
		SetUseTextureToShader(3, forceH);
	}
	// �V�F�[�_�[�ݒ�
	SetUsePixelShader(psH);

	DrawPrimitive2DToShader(vertices.data(), vertices.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

void MyEngine::DrawRectGraph(int x, int y, int srcX, int srcY, int srcW, int srcH, int handle, int psH, bool isTrans, int secondH, int thirdH, int forceH)
{
	int w, h;
	GetGraphSize(handle, &w, &h);

	std::array<VERTEX2DSHADER, 4> vertices;

	for (auto& v : vertices)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(0, 0, 0, 0);
		v.rhw = 1.0f;
	}

	float uvStartX = static_cast<float>(srcX) / static_cast<float>(w);
	float uvStartY = static_cast<float>(srcY) / static_cast<float>(h);
	float uvEndX = static_cast<float>(srcX + srcW) / static_cast<float>(w);
	float uvEndY = static_cast<float>(srcY + srcH) / static_cast<float>(h);

	// ����
	vertices[0].pos = VGet(x, y, 0.0f);
	vertices[0].u = uvStartX;
	vertices[0].v = uvStartY;
	// �E��
	vertices[1].pos = VGet(x + srcW, y, 0.0f);
	vertices[1].u = uvEndX;
	vertices[1].v = uvStartY;
	// ����
	vertices[2].pos = VGet(x, y + srcH, 0.0f);
	vertices[2].u = uvStartX;
	vertices[2].v = uvEndY;
	// �E��
	vertices[3].pos = VGet(x + srcW, y + srcH, 0.0f);
	vertices[3].u = uvEndX;
	vertices[3].v = uvEndY;

	// ����
	if (isTrans)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	// �s����
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	}
	// �e�N�X�`���ݒ�
	SetUseTextureToShader(0, handle);
	if (secondH != -1)
	{
		SetUseTextureToShader(1, secondH);
	}
	if (thirdH != -1)
	{
		SetUseTextureToShader(2, thirdH);
	}
	if (forceH != -1)
	{
		SetUseTextureToShader(3, forceH);
	}
	// �V�F�[�_�[�ݒ�
	SetUsePixelShader(psH);

	std::array<uint16_t, 6> indices;

	indices = { 0, 1, 3, 0, 3, 2 };

	DrawPrimitiveIndexed2DToShader(vertices.data(), vertices.size(), indices.data(), indices.size(), DX_PRIMTYPE_TRIANGLELIST);
}
