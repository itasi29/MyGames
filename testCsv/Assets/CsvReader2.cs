/* こっちのCSVローダはデータの保存までしてくれる便利な奴 */
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CsvReader2 : MonoBehaviour
{
    // 保存用データ
    public TextData[] textData;

    // Start is called before the first frame update
    void Start()
    {
        // csvファイルの読み込み
        TextAsset csv = Resources.Load("test", typeof(TextAsset)) as TextAsset;

        // CSVSerializerの機能を使ってデータを流し込む
        // CSVSerializerのAssetStorリンク
        // https://assetstore.unity.com/packages/tools/integration/csv-serialize-135763?locale=ja-JP
        textData = CSVSerializer.Deserialize<TextData>(csv.text);

        // データの確認はこれをCommponentしたオブジェクトを見てね
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
