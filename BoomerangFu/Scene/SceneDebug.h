#pragma once
#include "SceneBase.h"
class SceneDebug final : public SceneBase
{
public:
	SceneDebug();
	~SceneDebug() {}

	// �V�[���ɓ���Ƃ��̏���������
	void Init() override;
	// ���t���[���s���X�V����
	void Update(bool isFade) override;
	// ���t���[���s���`�揈��
	void Draw() const override;
	// �V�[���𔲂���Ƃ��̏���
	void End() override;
	// �V�[���ύX����
	std::shared_ptr<SceneBase> Change() override;
};

