using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//制作者　神谷優輝
//----------------------------------------------

public class Move : MonoBehaviour
{
    //移動速度
    private float m_moveSpeed;
    //移動方向(前の入力）
    private Vector2 m_preMoveDir;
    //移動方向(今の入力）
    private Vector2 m_postMoveDir;
    //移動するかどうか
    private bool m_isMoveing;
    //アッタチされているオブジェクトのRigidbodyを取得
    Rigidbody2D _rb;

    // Start is called before the first frame update
    void Start()
    {
        m_moveSpeed = 0.0f;
        m_preMoveDir = new Vector2(99.0f, 99.0f);
        m_isMoveing = false;
        _rb = this.GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        //オブジェクトにRigidbodyがアタッチされてない場合
        if (!_rb)
        {
            Debug.Log("オブジェクトにRigidbodyをアタッチしてください。");
            return;
        }

        //移動するフラグが上がっていたら処理
        if (m_isMoveing == true)
        {
            ToMove();
        }
    }
    /// <summary> 
    ///移動に関する値をセットする
    /// </summary>
    /// <param name="_movespeed">移動速度</param>
    /// <param name="_dir">移動方向</param>
    /// <param name="_ismove">移動処理を行うか</param>
    public void SetMovement(float _movespeed, Vector2 _dir, bool _ismove)
    {
        //移動速度を設定する
        m_moveSpeed = _movespeed;
        //移動方向を設定する
        m_preMoveDir = _dir;
        //移動するフラグをたてる
        m_isMoveing = _ismove;
    }

    //移動方向に速度分移動する
    private void ToMove()
    {
        if (!_rb) { return;}

        Vector2 _nowPos = _rb.position;     //現在の座標を取得する

        //入力された方向が変わるまで移動し続ける
        //現在の座標に移動方向＊速度分、移動させる
        _nowPos += new Vector2(m_preMoveDir.x * m_moveSpeed, m_preMoveDir.y * m_moveSpeed);
        //移動させた分を反映する
        _rb.position = _nowPos;
    }
}
