using UnityEngine;
using UnityEngine.SceneManagement;

//----------------------------------------------
//制作者　矢越栄吉
//----------------------------------------------

public class ClearOver_Scene : MonoBehaviour
{
    private string m_sceneName;         // 現在のシーンの名前を取得する   
    private string m_gameSceneName;     // ゲームマネージャーから受けっ取ったシーン名を取得する

    private void Start()
    {
        // カウントダウンタイマーを初期化する
        CountDownTimer.IntializationTimer();
        
        // 現在のシーンの名前を格納する
        m_sceneName = SceneManager.GetActiveScene().name;
        // 直前のステージシーンの名前を取得する
        m_gameSceneName = GameManager.GetPreserveScene();
    }

    // Update is called once per frame
    private void Update()
    {
        // プレイヤー
        // リスポーン地点をスタート地点に更新する
        PlayerCotroller.m_respawnPosition = PlayerCotroller.m_startPosition;
        // チェックポイントフラグを下げる
        PlayerCotroller.m_checkPointFlag = false;


        // いずれかのボタンが押されたらシーンを移行する
        if (!Input.anyKey) { return; }

        // 現在のシーンが"ゲームクリア"の場合
        if (m_sceneName == "GameClear")
        {
            // 直前のシーンが"ステージ1"の場合
            if (m_gameSceneName == "Stage1")
            {
                // "ステージ2"に移行する
                SceneManager.LoadScene("Stage2");
            }
            // 直前のシーンが"ステージ1"の場合
            if (m_gameSceneName == "Stage2")
            {
                // "ステージ3"に移行する
                SceneManager.LoadScene("Stage3");
            }
        }

        // 現在のシーンが"ゲームオーバー"の場合
        if (m_sceneName == "GameOver")
        {
            // "直前のゲームシーン"に戻る
            SceneManager.LoadScene(m_gameSceneName);
        }
    }
}
