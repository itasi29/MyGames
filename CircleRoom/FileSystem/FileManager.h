#pragma once

#include <unordered_map>	// �}�b�v�p(�t�@�C���p�X�e�[�u���p)
#include <string>			// �t�@�C���p�X���o���Ƃ��悤
#include <memory>			// File��shared_ptr��ێ����Ă����悤

class File;

/// <summary>
/// �t�@�C���̃}�b�v�����A�t�@�C�����Ǘ�����
/// </summary>
class FileManager
{
public:
	// �t�@�C���p�X��������L�[�A�l��File�I�u�W�F�N�g�Ƃ����e�[�u���^
	using FileTable_t = std::unordered_map<std::wstring,
											std::shared_ptr<File>>;
	FileTable_t m_fileTable;

public:
	~FileManager();
	/// <summary>
	/// �w��̃p�X�̉摜�f�[�^�����[�h���Ă��̃t�@�C���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <param name="isEternal">�풓�t���O</param>
	/// <returns>�t�@�C���I�u�W�F�N�g</returns>
	std::shared_ptr<File> LoadGraphic(const std::wstring& path, bool isEternal = false);
	/// <summary>
	/// �w��̃p�X�̃T�E���h�f�[�^�����[�h���Ă��̃t�@�C���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <param name="isEternal">�풓�t���O</param>
	/// <returns>�t�@�C���I�u�W�F�N�g</returns>
	std::shared_ptr<File> LoadSound(const std::wstring& path, bool isEternal = false);

	/// <summary>
	/// ���\�[�X�̍폜���s��
	/// ���ۂɂ͓����J�E���g�����āA0�ɂȂ����炻�ꂼ��̃��\�[�X��
	/// �폜�𖽗߂��܂��B
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <memo>�t�@�C���p�X��n���̂̓}�b�v���猟�����邽��</memo>
	void Delete(const std::wstring& path);
};

