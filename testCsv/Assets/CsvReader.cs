/* ��������CSV���[�_�͓ǂݍ��݂͂���Ă���邯�ǃf�[�^�̕ϊ����͎����ł��Ȃ��Ⴂ���Ȃ� */
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class CsvReader : MonoBehaviour
{
    // Csv�t�@�C���̃f�[�^�����Ă����ϐ��H
    TextAsset csv;
    // �f�[�^��ۑ����Ă������X�g
    List<string[]> csvData = new List<string[]>();


    void Start()
    {
        // csv�t�@�C���̓ǂݍ���
        csv = Resources.Load("test") as TextAsset;
        // TextAsset�͂��ׂĂ̏�񂪘A������Ă����Ԃ炵���̂ŕϊ�
        StringReader reader = new StringReader(csv.text);

        // ����Ȃ���񕪂��������ʂ��ɓǂݍ���
        reader.ReadLine();

        // ���̍s���ǂݍ��߂Ȃ��Ȃ�܂ő�����
        while (reader.Peek() != -1)
        {
            // 1�s�ǂݍ���
            string line = reader.ReadLine();

            // ","��؂�Ń��X�g�f�[�^�ɒǉ�
            csvData.Add(line.Split(','));
        }

        foreach (var data in csvData)
        {
            Debug.Log("No:" + data[0] + ", Name:" + data[1] + ", Hp:" + data[2] + ", Attack:" + data[3]);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
