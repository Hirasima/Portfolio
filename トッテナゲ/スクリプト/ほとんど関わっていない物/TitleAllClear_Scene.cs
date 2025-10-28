using UnityEngine;
using UnityEngine.SceneManagement;

//----------------------------------------------
//制作者　矢越栄吉
//----------------------------------------------

public class TitleAllClear_Scene : MonoBehaviour
{
    private string m_sceneName;     // 現在のシーンの名前を取得する

    private void Start()
    {
        // 現在のシーンの名前を格納する
        m_sceneName = SceneManager.GetActiveScene().name;

        // カウントダウンタイマーを初期化する
        CountDownTimer.IntializationTimer();
    }

    // Update is called once per frame
    private void Update()
    {
        // プレイヤー
        // リスポーン地点をスタート地点に更新する
        PlayerCotroller.m_respawnPosition = PlayerCotroller.m_startPosition;
        // チェックポイントフラグを下げる
        PlayerCotroller.m_checkPointFlag = false;

        // いずれかのボタンが押されたら
        if (!Input.anyKey) { return; }
            
        // 現在のシーンがタイトルなら
        if (m_sceneName == "Title")
        {
            // "ステージ1"に移行する
            SceneManager.LoadScene("Stage1");
        }
        if (m_sceneName == "AllClear")
        {
            // "ステージ1"に移行する
            SceneManager.LoadScene("Title");
        }
    }
}
