using UnityEngine;

//----------------------------------------------
//制作者　矢越栄吉
//----------------------------------------------

public class Key : MonoBehaviour
{
    [Header("カギの所持状態"), SerializeField]
    private bool m_keyActiveFlag;       // カギがプレイヤーに取得されたか判定するフラグ
    private Vector3 m_stopPosition;     // カギの停止位置
    [Header("追尾速度"), SerializeField]
    private float m_trackingSpeed;          // 追尾速度
    private Transform m_playerTransform;    // プレイヤーの位置情報を取得する

    //各コンポーネントを保存する変数
    private CapsuleCollider2D m_col;

    // Start is called before the first frame update
    private void Start()
    {
        // 変数を初期化する
        m_keyActiveFlag = false;                            // カギ取得フラグ
        m_stopPosition = new Vector3(0.5f, 0.0f, 0.0f);     // カギの停止位置

        //必要なコンポーネントを取得する
        m_col = gameObject.GetComponent<CapsuleCollider2D>();
    }

    // Update is called once per frame
    private void Update()
    {
        // キーアクティブフラグが下がっている場合は実行しない
        if (!m_keyActiveFlag) { return; }
       
        // プレイヤーが右を向いている時
        if (m_playerTransform.rotation == Quaternion.Euler(0.0f, 0.0f, 0.0f))
        {
            // プレイヤーを追尾し、"左側"に停止する
            transform.position = Vector3.Lerp(transform.position,
                m_playerTransform.transform.position - m_stopPosition, m_trackingSpeed * Time.deltaTime);
        }
        // プレイヤーが左を向いている時
        else
        {
            // プレイヤーを追尾し、"右側"に停止する
            transform.position = Vector3.Lerp(transform.position,
                m_playerTransform.transform.position + m_stopPosition, m_trackingSpeed * Time.deltaTime);
        }
    }

    // ゲームマネージャーにカギ取得フラグを渡すための関数
    public bool GetKey()
    {
        return m_keyActiveFlag;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        // プレイヤーと接触したら
        if (collision.gameObject.tag == "Player")
        {
            // 当たったプレイヤーの位置情報を格納する
            m_playerTransform = collision.transform;
            // 当たり判定をなくす
            m_col.enabled = false;
            // キーアクティブフラグを上げる
            m_keyActiveFlag = true;
        }
    }
}
