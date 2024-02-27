#include "StringUtility.h"
#include <DxLib.h>
#include <cassert>

std::wstring StringUtility::StringToWString(const std::string& str)
{
    // in : ��񑤂�string(���炩���ߕ������Ă���)
    // out : �߂�l��wstring(�����񐔂��킩��Ȃ�)
    
    // MultiByteToWideChar�́A�Q��Ăяo�����Ƃ����҂���Ă���B
    // �P��ڂ̌Ăяo���́A�ϊ��㕶����̃T�C�Y�𓾂邽��
    // �Q��ڂ̌Ăяo���ŁA�ԊҌ㕶����𓾂�

    std::wstring ret;   // �ŏI�I�ɕԂ����C�h������(�ŏ��͉��������Ă��Ȃ�)

    // �P��ڂ̌Ăяo��
    int result = MultiByteToWideChar(
        CP_ACP,     // ��̕�����̃R�[�h�y�[�W�������܂�(ACP��ANSI������=ShitfJIS)
        0,          // �ϊ��I�v�V�����ł����A����͓��Ɏw��Ȃ��ł��̂�0
        str.data(), // ���̕�����̐擪�|�C���^
        static_cast<int>(str.size()), // ���̕�����̕�����
        nullptr,    // out, optional�Ȃ̂ŁAnullptr��n���Ă���
        0);         // �����ł͈Ӗ��Ȃ��̂�0�ɂ��Ă���

    // ��x�ڂ̌Ăяo���̖߂�l���A�ϊ��㕶����̕������ɂȂ��Ă��܂��B
    assert(result >= 0);

    ret.resize(result); // �󂯎��镶����̃o�b�t�@���m�ۂ���

    result = MultiByteToWideChar(
        CP_ACP,     // ���̃R�[�h�y�[�W
        0,          // �I�v�V����
        str.data(), // ���̕�����ւ̃A�h���X
        static_cast<int>(str.size()), // ��������̕�����
        ret.data(), // �ϊ��㕶����̃A�h���X
        static_cast<int>(ret.size()));// �ϊ���̕�����

    return ret;
}

std::vector<std::string> StringUtility::Split(std::string& str, char del)
{
    // ��؂�J�n�ʒu
    int first = 0;
    // ��؂�ŏI�ʒu
    int last = str.find_first_of(del);

    std::vector<std::string> result;

    while (first < str.size())
    {
        // ��؂������̂����ʂɓ���Ă���
        std::string subStr(str, first, last - first);

        result.push_back(subStr);

        // �ʒu�X�V
        first = last + 1;
        last = str.find_first_of(del, first);
        if (last == std::string::npos)
        {
            last = str.size();
        }
    }

    return result;
}
