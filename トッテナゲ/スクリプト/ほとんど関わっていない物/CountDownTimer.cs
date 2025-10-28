using UnityEngine;
using UnityEngine.UI;
//--------------------------------
//10/28 神谷
//TextMeshPorを使うためのusing
using TMPro;
//--------------------------------

//----------------------------------------------
//制作者　矢越栄吉
//----------------------------------------------

public class CountDownTimer : MonoBehaviour
{
    public static float m_countDownTimer = 200.0f;     // 制限時間

    // オブジェクトを格納するための変数
    private Text m_text;

    //---------------------------------------------------------------
    //10/28　神谷
    //TextMeshProに対応するように新しく変数と処理を追加しました
    private TextMeshProUGUI m_tectMesh;
    //---------------------------------------------------------------

    // Start is called before the first frame update
    void Start()
    {   
        // オブジェクトを格納する
        m_text = GetComponent<Text>();

        //-----------------------------------------
        //10/28 神谷
        //アタッチされているTextMeshを取得する
        m_tectMesh = this.GetComponent<TextMeshProUGUI>();
        //-----------------------------------------
    }

    // Update is called once per frame
    private void Update()
    {
        // カウントダウンを開始
        m_countDownTimer -= Time.deltaTime;

        //---------------------------------------------------------
        //10/28 神谷
        // 安全性の確保
        if (m_text && !m_tectMesh)
        {
            // 整数に直して表示する
            m_text.text = Mathf.CeilToInt(m_countDownTimer).ToString();
        }
        //---------------------------------------------------------


        //---------------------------------------------------------
        //10/28 神谷　
        //TextMeshProがアタッチされているかつ
        if (!m_tectMesh) { return; }

        //Textがアタッチされていないとき処理
        if (!m_text)
        {
            m_tectMesh.text = Mathf.CeilToInt(m_countDownTimer).ToString();
        }

        //---------------------------------------------------------
    }

    // ゲームマネージャーにタイマーの値を渡すための関数
    public float GetTimer()
    {
        return m_countDownTimer;
    }

    // タイトル・ゲームクリア・ゲームオーバーシーンでタイマーを初期化する
    public static void IntializationTimer()
    {
        m_countDownTimer = 200.0f;
    }
}
