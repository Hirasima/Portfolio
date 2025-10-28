using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------
//制作者　平島　迅
//----------------------------------------------

public class GroundingJudgmenter: MonoBehaviour
{
    [SerializeField] private float m_playerWidth;    //プレイヤーの幅
    [SerializeField] private float m_playerHeight;   //プレイヤーの高さ
    [SerializeField] private float m_offsetX;         //判定の中心からのズレ

    private bool m_bOffsetFlipX;       //オフセット反転フラグ


    [SerializeField] private LayerMask groundLayer;    //地面として判別するオブジェクトのレイヤー

    // Start is called before the first frame update
    void Start()
    {
        m_bOffsetFlipX = false;
    }

    // Update is called once per frame
    void Update()
    {
        DrowRay();
    }

    //接地しているときにbool型でtrueを返す関数
    public bool IsGrounded()
    {
        bool result = false;

        //レイを飛ばして下に地面があるか調べる
        //スタックを防ぐため二本レイを飛ばす

        //プレイヤーのpostionからプレイヤーの幅の分左右にずらして二本飛ばす
        float offset = m_offsetX;
        if (m_bOffsetFlipX) offset = -offset;
        Vector2 rightRayOrigin = new Vector2(transform.position.x + m_playerWidth + offset / 2.0f, transform.position.y);
        Vector2 leftRayOrigin = new Vector2(transform.position.x - m_playerWidth + offset / 2.0f, transform.position.y);

        RaycastHit2D rightRay = Physics2D.Raycast(rightRayOrigin, Vector2.down, m_playerHeight / 2.0f, groundLayer);
        RaycastHit2D leftRay = Physics2D.Raycast(leftRayOrigin, Vector2.down, m_playerHeight / 2.0f, groundLayer);

        //どちらかのレイが地面に接触していたら結果をtureにする
        if (rightRay == true || leftRay == true)
        {
            result = true;
        }

        return result;
    }

    //---------------------------------------
    //Get関数
    //---------------------------------------

    //オフセットを取得
    public float GetOffsetX()
    {
        return m_offsetX;
    }

    //---------------------------------------
    //Set関数
    //---------------------------------------

    //X方向のオフセットを設定
    public void SetOffsetX(float _offsetX)
    {
        m_offsetX = _offsetX;
    }

    //X方向のオフセットを反転するか設定
    public void SetOffsetFlipX(bool _flip)
    {
        m_bOffsetFlipX = _flip;
    }

    //デバック用関数
    //レイを描画
    void DrowRay()
    {
        //レイの原点を設定
        float offset = m_offsetX;
        if (m_bOffsetFlipX) offset = -offset;
        Vector2 rightRayOrigin = new Vector2(transform.position.x + m_playerWidth + offset / 2.0f, transform.position.y);
        Vector2 leftRayOrigin = new Vector2(transform.position.x - m_playerWidth + offset / 2.0f, transform.position.y);

        //レイを描画
        Debug.DrawRay(rightRayOrigin, new Vector3(0, -m_playerHeight / 2.0f, 0), Color.red);
        Debug.DrawRay(leftRayOrigin, new Vector3(0, -m_playerHeight / 2.0f, 0), Color.red);
    }
}
