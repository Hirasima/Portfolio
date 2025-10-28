using UnityEngine;

//----------------------------------------------
//制作者　矢越栄吉
//----------------------------------------------

public class Goal : MonoBehaviour
{
    [Header("ゴール判定"), SerializeField]
    private bool m_goalPassibleFlag;    // ゴール可能かどうか判定するフラグ

    // 各コンポーネントを保存する変数
    [SerializeField]
    private Key m_key;                  // カギ
    private Animator m_anim;            // Animator

    // Start is called before the first frame update
    void Start()
    {
        // 変数を初期化する
        m_goalPassibleFlag = false;

        // コンポーネントを取得する
        m_anim = GetComponent<Animator>();
    }

    // ゲームマネージャーにゴール可能フラグを渡すための関数
    public bool GetGoal()
    {
        return m_goalPassibleFlag;
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        // プレイヤーと接触していたら
        if (collision.gameObject.tag == "Player")
        {
            // ゴール可能フラグを上げる
            m_goalPassibleFlag = true;

            // カギを所持している場合
            if (m_key.GetKey())
            {
                // "Open"アニメーションを再生する
                m_anim.SetInteger("Goal_Anim", 1);
            }
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        // プレイヤーと離れたら
        if (collision.gameObject.tag == "Player")
        {
            // ゴール可能フラグを下げる
            m_goalPassibleFlag = false;
        }
    }
}
