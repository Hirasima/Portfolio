using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//-------------------------------------
//制作者：神谷優輝
//-------------------------------------

public class MoveObject : MonoBehaviour
{
    [SerializeField]
    private float m_maxRange;               //移動範囲の上限
    [SerializeField]
    private float m_minRange;               //移動範囲の下限
    [SerializeField]
    private float m_moveSpeed;              //移動速度
    private Vector2 m_startPosition;        //初期化座標
    private Vector2 m_currentPosition;      //移動後の座標

    private enum MoveDirection
    {
        None = -1,      //入力なし
        Vertical = 0,   //垂直 0
        Horizontal = 2, //水平 2
    }
    [SerializeField] private MoveDirection m_Direction;

    // Start is called before the first frame update
    void Start()
    {
        //始まった時の座標を保存する
        m_startPosition = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        Moveing(m_Direction);
    }

    void Moveing(MoveDirection _moveDirection)
    {
        //移動後の座標を取得
        m_currentPosition = this.transform.position;

        //移動方向によって処理を変える
        switch(_moveDirection)
        {
            //水平方向
            case MoveDirection.Horizontal:
                //移動範囲の上限を超えたら下に移動する
                if (m_startPosition.x + m_maxRange < m_currentPosition.x)
                {
                    //移動速度が＋(プラス)だったらー(マイナス)にする
                    if (m_moveSpeed > 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //移動範囲の下限を超えたら上に移動する
                if (m_startPosition.x - m_minRange > m_currentPosition.x)
                {
                    //移動速度がー(マイナス)なら＋(プラス)にする
                    if (m_moveSpeed < 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //移動処理開始
                transform.position = new Vector2(m_currentPosition.x + m_moveSpeed, m_currentPosition.y);
                
                break;

            //垂直方向
            case MoveDirection.Vertical:
                //移動範囲の上限を超えたら下に移動する
                if (m_startPosition.y + m_maxRange < m_currentPosition.y)
                {
                    //移動速度が＋(プラス)だったらー(マイナス)にする
                    if (m_moveSpeed > 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //移動範囲の下限を超えたら上に移動する
                if (m_startPosition.y - m_minRange > m_currentPosition.y)
                {
                    //移動速度がー(マイナス)なら＋(プラス)にする
                    if (m_moveSpeed < 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //移動処理開始
                transform.position = new Vector2(m_currentPosition.x, m_currentPosition.y + m_moveSpeed);

                break;

            //入力なし
            case MoveDirection.None:
                Debug.LogWarning("移動方向がしていされていません");
                break;
        }
    }
    private void OnCollisionEnter2D(Collision2D collision)
    {
        //当たったオブジェクトのタグがプレイヤーなら処理
        if (collision.gameObject.tag == "Player")
        {
            //プレイヤーを子オブジェクトにする
            collision.gameObject.transform.SetParent(this.gameObject.transform);
        }
    }
    private void OnCollisionExit2D(Collision2D collision)
    {
        //当たったオブジェクトのタグがプレイヤーなら処理
        if (collision.gameObject.tag == "Player")
        {
            //プレイヤーを子オブジェクトにする
            collision.gameObject.transform.SetParent(null);
        }
    }
}
