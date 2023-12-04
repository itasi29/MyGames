#include "FileManager.h"
#include <DxLib.h>
#include <cassert>
#include "ImageFile.h"
#include "SoundFile.h"

FileManager::~FileManager()
{
    for (auto& file : m_fileTable)
    {
        file.second->Delete();
    }
}

std::shared_ptr<File> FileManager::LoadGraphic(const std::wstring& path, bool isEternal)
{
    // ��d���[�h�h�~
    // �����A���[�h�ς݂̃t�@�C�����������炻�̃t�@�C����Ԃ�
    if (m_fileTable.find(path) != m_fileTable.end())
    {
        auto file = m_fileTable.at(path);

        // ImageFile�ɃL���X�g
        std::shared_ptr<ImageFile> imgFile = 
            std::dynamic_pointer_cast<ImageFile>(file);
        assert(imgFile != nullptr);
        // ���L�҃J�E���g�𑝉������܂�
        file->m_count++;
        // �e�[�u���ɂ���t�@�C���I�u�W�F�N�g���R�s�[���Ďg��
        // �R�s�[�R���X�g���N�g���g�p
        return std::make_shared<ImageFile>(*imgFile);
    }
    // �����A�e�[�u���Ƀ��[�h�ς݃p�X���Ȃ��ꍇ�ɂ�
    // �������ʂ�
    int handle = LoadGraph(path.c_str());
    assert(handle >= 0);
    m_fileTable[path] = std::make_shared<ImageFile>(*this);
    // �����̃t�@�C���e�[�u���ɏ���o�^����
    auto& file = m_fileTable[path];
    file->m_handle = handle;
    file->m_path = path;
    file->m_count = 1;
    file->m_isEternal = isEternal;
    // �e�[�u���ɂ���t�@�C���I�u�W�F�N�g�̃R�s�[��Ԃ�
    // �f�X�g���N�^���@�\�����邽�߂����āu�R�s�[�v�ɂ��Ă�
    auto imgFile = std::dynamic_pointer_cast<ImageFile>(file);
    return std::make_shared<ImageFile>(*imgFile);
    //return std::make_shared<ImageFile>
    //    (*std::dynamic_pointer_cast<ImageFile>(file));
}

std::shared_ptr<File> FileManager::LoadSound(const std::wstring& path, bool isEternal)
{
    // ��d���[�h�h�~
// �����A���[�h�ς݂̃t�@�C�����������炻�̃t�@�C����Ԃ�
    if (m_fileTable.find(path) != m_fileTable.end())
    {
        auto file = m_fileTable.at(path);

        // SoundFile�ɃL���X�g
        std::shared_ptr<SoundFile> sndFile =
            std::dynamic_pointer_cast<SoundFile>(file);
        assert(sndFile != nullptr);
        // ���L�҃J�E���g�𑝉������܂�
        file->m_count++;
        // �e�[�u���ɂ���t�@�C���I�u�W�F�N�g���R�s�[���Ďg��
        // �R�s�[�R���X�g���N�g���g�p
        return std::make_shared<SoundFile>(*sndFile);
    }
    // �����A�e�[�u���Ƀ��[�h�ς݃p�X���Ȃ��ꍇ�ɂ�
    // �������ʂ�
    int handle = LoadSoundMem(path.c_str());
    assert(handle >= 0);
    m_fileTable[path] = std::make_shared<SoundFile>(*this);
    // �����̃t�@�C���e�[�u���ɏ���o�^����
    auto& file = m_fileTable[path];
    file->m_handle = handle;
    file->m_path = path;
    file->m_count = 1;
    file->m_isEternal = isEternal;
    // �e�[�u���ɂ���t�@�C���I�u�W�F�N�g�̃R�s�[��Ԃ�
    // �f�X�g���N�^���@�\�����邽�߂����āu�R�s�[�v�ɂ��Ă�
    auto sndFile = std::dynamic_pointer_cast<SoundFile>(file);
    return std::make_shared<SoundFile>(*sndFile);
}

void FileManager::Delete(const std::wstring& path)
{
    // �Ȃ������疳��
    if (m_fileTable.find(path) == m_fileTable.end()) return;

    auto file = m_fileTable.at(path);
    if (file->m_isEternal) return;

    file->m_count--;    // ���L�҃J�E���g���f�N�������g
    // ���L�҃[���Ȃ�{���ɍ폜
    if (file->m_count == 0)
    {
        m_fileTable.erase(path);
        file->Delete();
    }
}
