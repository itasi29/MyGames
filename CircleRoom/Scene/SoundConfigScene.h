#pragma once
#include "Scene.h"

/// <summary>
/// ���ʒ����V�[��
/// </summary>
class SoundConfigScene :
    public Scene
{
public:
    SoundConfigScene(GameManager& mgr);
    ~SoundConfigScene();

    void Update(Input& input);
    void Draw();

private:

	/// <summary>
	/// �Q�[�W�̕`��
	/// </summary>
	/// <param name="drawX">���[�`��ʒu</param>
	/// <param name="drawY">��[�`��ʒu</param>
	/// <param name="rate">�{�����[������</param>
	void DrawGauge(int drawX, int drawY, float rate);

private:
	int m_frame = 0;
};

