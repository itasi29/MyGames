/* ��������CSV���[�_�̓f�[�^�̕ۑ��܂ł��Ă����֗��ȓz */
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CsvReader2 : MonoBehaviour
{
    // �ۑ��p�f�[�^
    public TextData[] textData;

    // Start is called before the first frame update
    void Start()
    {
        // csv�t�@�C���̓ǂݍ���
        TextAsset csv = Resources.Load("test", typeof(TextAsset)) as TextAsset;

        // CSVSerializer�̋@�\���g���ăf�[�^�𗬂�����
        // CSVSerializer��AssetStor�����N
        // https://assetstore.unity.com/packages/tools/integration/csv-serialize-135763?locale=ja-JP
        textData = CSVSerializer.Deserialize<TextData>(csv.text);

        // �f�[�^�̊m�F�͂����Commponent�����I�u�W�F�N�g�����Ă�
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
