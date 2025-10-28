using UnityEngine;
using UnityEngine.SceneManagement;

//----------------------------------------------
//制作者　矢越栄吉
//----------------------------------------------

public class GameManager : MonoBehaviour
{
    public static string m_sceneName;   // シーンの名前を保存する
    [SerializeField]
    private bool m_operationFlag;       // 操作説明 ON OFF のフラグ 
    private bool m_fallheckFlag;        // 画面外に落ちたか判定する

    // オブジェクトを格納するための変数
    [SerializeField]
    private Key m_key;                  // カギ
    [SerializeField]
    private Goal m_goal;                // ゴール
    [SerializeField]
    private CountDownTimer m_downTimer; // 制限時間
    [SerializeField]
    private GameObject m_operation;     // 操作説明
    private GamepadStick m_gamepadStick;// コントローラー


    // Start is called before the first frame update
    private void Start()
    {
        // フラグを下げる
        m_fallheckFlag = false;
        m_operationFlag = false;
        // オブジェクトを格納する

        m_gamepadStick = this.gameObject.GetComponent<GamepadStick>();

        // シーンの名前を保存する
        PreserveSceneName();
    }

    // Update is called once per frame
    private void Update()
    {
        // シーン再読み込み
        RoadThisScene();
        // ゲームオーバーシーンを呼ぶため
        RoadGameOver();
        // ゲームクリアシーンを呼ぶため
        RoadGameClear();

        // 操作説明を表示する
        DiplayOperation();
    }

    // シーンリセット
    private void RoadThisScene()
    {
        // Rキーが押されるまたは、画面外に落下したら
        if (Input.GetKeyDown(KeyCode.R) ||
            m_fallheckFlag ||
            Input.GetKeyDown("joystick button 4"))
        {
            // 直前のシーンを再読み込みする
            SceneManager.LoadScene(m_sceneName);
        }
    }

    // ゲームオーバーシーンを読み込む
    private void RoadGameOver()
    {
        //カウントが０になったら
        if (m_downTimer.GetTimer() <= 0.0f)
        {
            // ゲームオーバーシーンに移行する
            SceneManager.LoadScene("GameOver");
        }
    }

    // ゲームクリアシーンを読み込む
    private void RoadGameClear()
    {
        // どちらかのフラグが下がっていたら実行しない
        if (!(m_key.GetKey() && m_goal.GetGoal())) { return; }

        // スティック上またはアローキー上が押されたらステージクリア
        if ((m_gamepadStick.GetGamepadValue() == Vector2.up) ||
           ((m_gamepadStick.GetGamepadValue().y > 0.8f) && 
           ((m_gamepadStick.GetGamepadValue().x < 0.6f) || (m_gamepadStick.GetGamepadValue().x > -0.6f)))||
            (Input.GetKeyDown(KeyCode.UpArrow)))
        {
            // クリアステージが最終なら
            if (m_sceneName == "Stage3")
            {
                // オールクリアシーンに移行する
                SceneManager.LoadScene("AllClear");
            }
            else
            {
                // ゲームクリアシーンに移行する
                SceneManager.LoadScene("GameClear");
            }             
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        // プレイヤーと接触したら
        if (collision.gameObject.tag == "Player")
        {
            // 落下フラグを上げる
            m_fallheckFlag = true;
        }
    }

    // 現在のシーン名を保存する
    public static void PreserveSceneName()
    {
        m_sceneName = SceneManager.GetActiveScene().name;
    }
    // 現在のシーン名を返す
    // ゲームクリア・ゲームオーバーシーンで受け取る
    public static string GetPreserveScene()
    {
        return m_sceneName;
    }

    // 操作説明を表示する
    private void DiplayOperation()
    {
        // "RBボタンが押された" かつ"操作説明表示がOFF" の場合
        if (Input.GetKeyDown("joystick button 5") && !m_operationFlag)
        {
            // フラグを上げる
            m_operationFlag = true;
            // アクティブ状態にする
            m_operation.SetActive(true);
        }
        // "RBボタンが押された" かつ"操作説明表示がON" の場合
        else if (Input.GetKeyDown("joystick button 5") && m_operationFlag)
        {
            // フラグを下げる
            m_operationFlag = false;
            // 非アクティブ状態にする
            m_operation.SetActive(false);
        }
    }

    // プレイヤーコントローラーに操作説明 ON OFF のフラグを渡すための関数
    public bool GetOperation()
    {
        return m_operationFlag;
    }
}
