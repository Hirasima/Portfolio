using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//制作者　平島　迅
//----------------------------------------------

public class Block : MonoBehaviour
{
    enum BlockStatus
    {
        placed,
        grabbed,
        thrown
    };
    [SerializeField] private BlockStatus m_status;       //ブロックの現在の状態保存用
    enum Direction
    {
        right,
        left
    };
    private Direction m_grabDire;    //掴まれた方向

    private int m_stopFrameCount;
    
    [SerializeField] private float m_throwPower;    //投げられる力の強さ

    private Transform m_grabberTransform;   //掴んでいるオブジェクトのtransform
    private Vector3 m_grabberPos;           //掴んでいるオブジェクトの位置

    private Rigidbody2D _rb;            //Rigidbody保存用
    [SerializeField] private LayerMask m_groundLayer;

    // Start is called before the first frame update
    void Start()
    {
        //コンポーネントの初期化
        _rb = GetComponent<Rigidbody2D>();
        _rb.bodyType = RigidbodyType2D.Dynamic;
        SetBlockStatus(BlockStatus.placed);

        //プロパティの初期化
        m_stopFrameCount = 0;
    }

    // Update is called once per frame
    void LateUpdate()
    {
        float[] dire = new float[2]
        {
             1.0f,
            -1.0f
        };

        switch (m_status)
        {
            //「置かれている」とき
            case BlockStatus.placed:

                break;

            //「掴まれている」とき
            case BlockStatus.grabbed:
                //掴んでいるオブジェクトの相対位置をキープ
                //現在位置 = 掴んでいるオブジェクトの現在位置 + 掴まれた時の掴んでいるオブジェクトとの距離
                this.transform.position = m_grabberTransform.position + m_grabberPos;

                break;

            //「投げられている」とき
            case BlockStatus.thrown:
                //X方向のvelocityがゼロ または 投げられている方向と逆にvelocityがかかっていたら
                if (_rb.velocity.x * dire[(int)m_grabDire] <= 0.0f)
                {
                    //止まっているフレーム数に加算
                    ++m_stopFrameCount;
                }
                //まだ動いていたらリセット
                else m_stopFrameCount = 0;

                //止まっているフレームが基準値を超えたら
                if(m_stopFrameCount >= 10)
                {
                    //ステータスを「置かれている」に変更
                    SetBlockStatus(BlockStatus.placed);
                    //カウントを初期化
                    m_stopFrameCount = 0;
                }

                break;
        }
    }

    //置かれたときに呼んでもらう関数
    public void OnPlaced()
    {

        //ステータスを「置かれている」に変更
        SetBlockStatus(BlockStatus.placed);

        //掴んでいるオブジェクトが設定されていたら登録解除
        if (m_grabberTransform != null)
        {
            m_grabberTransform = null;
        }
    }

    //掴まれたときに呼んでもらう関数
    public void OnGrabbed(Transform _grabber)
    {
        //ステータスを「掴まれている」に変更
        SetBlockStatus(BlockStatus.grabbed);

        //掴んでいるオブジェクトを登録
        m_grabberTransform = _grabber;
        //掴んでいるオブジェクトからの相対位置を保存
        m_grabberPos = this.transform.position - m_grabberTransform.position;
    }

    //投げられたときに呼んでもらう関数
    public void OnThrown()
    {

        //ステータスを「投げられている」に変更
        SetBlockStatus(BlockStatus.thrown);

        Throw(m_grabDire);

        //親オブジェクトが設定されていたら登録解除
        if (m_grabberTransform != null)
        {
            m_grabberTransform = null;
        }
    }

    //投げられる関数
    void Throw(Direction _dire)
    {
        Vector2[] vec = new Vector2[2]
        {
            Vector2.right,
            Vector2.left
        };

        _rb.velocity = Vector3.zero;
        _rb.AddForce(vec[(int)_dire] * m_throwPower);
    }

    //掴まれている方向を設定
    public void SetGrabbedDirection(int _handleDire)
    {
        m_grabDire = (Direction)_handleDire;
    }

    //ブロックの状態を設定
    private void SetBlockStatus(BlockStatus _status)
    {
        //「掴まれてる」への変更
        if (_status == BlockStatus.grabbed)
        {
            _rb.gravityScale = 0.0f;

            _rb.constraints = RigidbodyConstraints2D.FreezeRotation;
        }
        //「投げられている」への変更
        else if (_status == BlockStatus.thrown)
        {
            _rb.gravityScale = 0.0f;

            _rb.constraints = RigidbodyConstraints2D.FreezePositionY | RigidbodyConstraints2D.FreezeRotation;
        }
        //「置かれている」への変更
        else
        {
            m_stopFrameCount = 0;

            _rb.gravityScale = 1.0f;

            _rb.constraints = RigidbodyConstraints2D.FreezePositionX | RigidbodyConstraints2D.FreezeRotation;
        }
        m_status = _status;
    }
}
