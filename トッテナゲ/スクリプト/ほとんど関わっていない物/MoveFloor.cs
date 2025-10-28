using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveFloor : MonoBehaviour
{
    private bool m_moveStartFlag;                //移動を始めるためのフラグ
    private bool m_isMove;
    [SerializeField]
    private float m_maxRange;               //移動範囲の上限
    [SerializeField]
    private float m_minRange;               //移動範囲の下限
    [SerializeField]
    private float m_moveSpeed;              //移動速度
    private Vector2 m_startPosition;        //初期化座標
    private Vector2 m_currentPosition;      //移動後の座標
    private Vector2 m_moveDirection;        //移動方向
    private Move m_move;                    //Moveコンポーネント格納用

    private enum MoveDirection
    {
        None = -1,      //入力なし
        Vertical = 0,   //垂直 0
        Horizontal = 2, //水平 2
    }
    [SerializeField] private MoveDirection m_Direction;


    private void Start()
    {
        //変数の初期化
        m_moveStartFlag = false;
        m_isMove = true;
        m_startPosition = this.transform.position;
        m_moveDirection = Vector2.zero;

        //自身にアタッチされているコンポーネントを取得する
        m_move = this.GetComponent<Move>();
        
        if (m_moveSpeed == 0.0f)
        {
            Debug.Log(gameObject.name + " の移動速度が初期値で設定されています");
            m_moveSpeed = 0.05f;
        }
    }

    private void Update()
    {
        //Move.csがアタッチされてない場合処理しない
        if(!m_move)
        {
            Debug.Log("Moveコンポーネントをアタッチしてください。");
            return; 
        }
                
        //移動方向によって処理を変える
        switch(m_Direction)
        {
            //入力されてない場合
            case MoveDirection.None:
                Debug.Log("進行方向を入力してください。");
                break;

            //水平方向の処理
            case MoveDirection.Horizontal:
                //移動後の座標を取得する
                m_currentPosition = this.transform.position;
                
                //移動範囲の上限を上回ったら移動方向を変える
                if (m_startPosition.x + m_maxRange < m_currentPosition.x )
                {
                    //下方向へ移動
                    m_moveDirection = GetMoveDirection(m_Direction + 1);
                    //移動開始
                    m_move.SetMovement(m_moveSpeed, m_moveDirection,m_isMove);
                    return;
                }

                //移動範囲の下限を下回ったら移動方向を変える
                if (!m_moveStartFlag || m_startPosition.x - m_minRange > m_currentPosition.x)
                {
                    //上方向へ移動
                    m_moveDirection = GetMoveDirection(m_Direction);
                    //移動開始
                    m_move.SetMovement(m_moveSpeed, m_moveDirection,m_isMove);
                }

                //移動が始まったらフラグを下ろす
                if (!m_moveStartFlag)
                {
                    m_moveStartFlag = true;
                }
                break;

            //垂直方向の処理
            case MoveDirection.Vertical:
                //移動後の座標を取得する
                m_currentPosition = this.transform.position;

                // 移動範囲の上限を上回ったら移動方向を変える
                if (m_startPosition.y + m_maxRange < m_currentPosition.y)
                {
                    //下方向へ移動
                    m_moveDirection = GetMoveDirection(m_Direction + 1);
                    //移動開始
                    m_move.SetMovement(m_moveSpeed, m_moveDirection, m_isMove);
                }

                //移動範囲の下限を下回ったら移動方向を変える
                if (!m_moveStartFlag || m_startPosition.y - m_minRange > m_currentPosition.y)
                {
                    //上方向へ移動
                    m_moveDirection = GetMoveDirection(m_Direction);
                    //移動開始
                    m_move.SetMovement(m_moveSpeed, m_moveDirection, m_isMove);
                }

                //移動が始まったらフラグを下ろす
                if (!m_moveStartFlag)
                {
                    m_moveStartFlag = true;
                }
                break;
        }
    }

    public float GetMoveSpeed()
    {
        return m_moveSpeed;
    }

    public Vector2 GetMoveDirection()
    {
        return m_moveDirection;
    }

    //入力されたキーをベクトルにして返す
    Vector2 GetMoveDirection(MoveDirection _Inputdir)
    {
        //入力なしの場合
        if (_Inputdir == MoveDirection.None)
        {
            return Vector2.zero;
        }

        Vector2 inputdirection = new Vector2(0.0f, 0.0f);

        //有効な方向が入力された場合
        Vector2[] directionVectors = {
            Vector2.up,
            Vector2.down,
            Vector2.right,
            Vector2.left,
        };

        return directionVectors[(int)_Inputdir];
    }
}