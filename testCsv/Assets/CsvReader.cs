/* こっちのCSVローダは読み込みはやってくれるけどデータの変換等は自分でやんなきゃいけない */
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class CsvReader : MonoBehaviour
{
    // Csvファイルのデータを入れておく変数？
    TextAsset csv;
    // データを保存しておくリスト
    List<string[]> csvData = new List<string[]>();


    void Start()
    {
        // csvファイルの読み込み
        csv = Resources.Load("test") as TextAsset;
        // TextAssetはすべての情報が連結されている状態らしいので変換
        StringReader reader = new StringReader(csv.text);

        // いらない情報分だけいつも通り先に読み込む
        reader.ReadLine();

        // 次の行が読み込めなくなるまで続ける
        while (reader.Peek() != -1)
        {
            // 1行読み込む
            string line = reader.ReadLine();

            // ","区切りでリストデータに追加
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
