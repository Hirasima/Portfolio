using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

//----------------------------------------------
//制作者　神谷優輝
//----------------------------------------------

//10/26 矢越 チェックポイントを追加しました。
//21～23、65～67、324～334行

public class PlayerCotroller : MonoBehaviour
{
    private bool m_isGrasping;
    private float m_moveSpeed;                  //移動速度
    private float m_jumpPower;                  //ジャンプ力
    private Vector2 m_drection;                 //移動方向
    private Vector2 m_leftStickValue;           //左スティックから入力された値

    //----------------------------------------------
    //矢越
    //10/26 チェックポイントの追加
    //10/27アニメーションの追加
    //----------------------------------------------

    private bool[] m_animCheckFlag;             //プレイヤーアニメーション配列フラグ

    public static Vector2 m_startPosition;      //スタート地点
    public static Vector2 m_respawnPosition;    //リスポーン地点
    public static bool m_checkPointFlag = false;//チェックポイントに触れたか

    //----------------------------------------------

    //移動床に乗っているときの補正値
    private float m_correctionMoveSpeed;        //移動速度の補正値
    private Vector2 m_correctionMoveDirection;  //移動方向の補正値

    //読み取り専用
    readonly float m_defaultSpeed = 0.07f;      //通常速度

    //各コンポーネントを保存する変数
    private Animator m_anim;
    private Rigidbody2D m_rb;
    private Move m_move;
    private GroundingJudgmenter m_groundJudge;
    private PlayerBlockInteraction m_blockInteraction;
    private GamepadStick m_gamepadStick;

    //----------------------------------------------
    //矢越
    //10/30 Dead->Operation
    //----------------------------------------------
    [SerializeField]
    private GameManager m_gameManager;

    private enum PlayerStatus
    {
        Alive,      //生存
        Operation,  //操作説明
    }
    PlayerStatus m_status;

    //----------------------------------------------

    private enum InputDirection
    {
        None = -1,  //入力なし
        Up,     //上 0
        Down,   //下 1
        Right,  //右 2
        Left,   //左 3
    }

    //----------------------------------------------
    //矢越
    //10/26 チェックポイントの追加
    //10/27 アニメーションの追加
    //----------------------------------------------

    //プレイヤーアニメーションの状態
    private enum PlayerAnimation
    {
        Normal, //通常
        Grab,   //掴んでいる
    }
    PlayerAnimation m_animation;

    //----------------------------------------------

    private void Start()
    {
        //変数を初期化する
        m_isGrasping = false;
        m_moveSpeed = m_defaultSpeed;
        m_jumpPower = 350.0f;
        m_drection = new Vector2(0.0f, 0.0f);
        m_status = PlayerStatus.Alive;       

        //----------------------------------------------
        //矢越
        //10/26 チェックポイントの追加
        //10/27アニメーションの追加
        //----------------------------------------------
        
        //アニメーションの配列を初期化する
        //通常状態のアニメーション
        //0.Player_Idle  1.Player_Walk  2.Playe_Jump
        //掴み状態のアニメーション
        //3.Player_Grab_Idle  4.Player_Grab_Throw  5.Player_Grab_Walk  6.Player_Grab_Jump
        m_animCheckFlag = new bool[7] { true, false, false, true, false, false, false };

        //プレイヤーアニメーションを初期化する
        m_animation = PlayerAnimation.Normal;
        //スタート地点を初期化する
        m_startPosition = transform.position;
        //チェックポイントフラグが上がっている場合
        if (m_checkPointFlag) transform.position = m_respawnPosition;

        //----------------------------------------------

        //必要なコンポーネントを取得する
        m_rb = this.gameObject.GetComponent<Rigidbody2D>();
        m_anim = this.GetComponent<Animator>();
        m_move = this.gameObject.GetComponent<Move>();
        m_groundJudge = this.gameObject.GetComponent<GroundingJudgmenter>();
        m_blockInteraction = this.gameObject.GetComponent<PlayerBlockInteraction>();
        m_gamepadStick = this.gameObject.GetComponent<GamepadStick>();

        

    }

    private void Update()
    {
        //----------------------------------------------
        //矢越
        //10/30 操作説明状態の追加
        //----------------------------------------------

        ReceiveOperationIn();

        //----------------------------------------------

        //プレイヤーの状態で処理を変える
        switch (m_status)
        {
            //操作説明状態なら
            case PlayerStatus.Operation:
                break;

            //生存状態なら
            case PlayerStatus.Alive:
                Jump();
                GetInputDevice();
                HoldObject();
                OnMoveObject();
                PlayAnimation();
                break;            
        }
    }

    //----------------------------------------------
    //矢越
    //10/27アニメーションの追加
    //----------------------------------------------
    private void LateUpdate()
    {   
        //"投げる"アニメーションが最後まで再生された場合
        if (m_anim.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f && m_anim.GetInteger("Player_Anim") == 4)
        {
            //"投げる"アニメーションのフラグを下げる
            m_animCheckFlag[4] = false;
        }
    }

    //----------------------------------------------

    //ジャンプする
    void Jump()
    {
        //NULLチェック
        if (!m_rb){ return; }
        if (!m_groundJudge){ return; }

        //地面に接地していたら処理
        if (m_groundJudge.IsGrounded())
        {
            //ゲームパッドのA、Bボタンのどちらかが押されたら処理する
            if (Input.GetKeyDown("joystick button 0") || Input.GetKeyDown("joystick button 1") ||
                Input.GetKeyDown(KeyCode.Space))
            {
                m_rb.velocity = new Vector2(0.0f, 0.0f);
                //上方向に力を加える
                m_rb.AddForce(new Vector2(0.0f, 1.0f * m_jumpPower));
            }
        }
    }

    //入力を受け、処理をする関数
    void GetInputDevice()
    {
        //各コンポーネントがNULLなら処理しない
        if (!m_rb) { return; }
        if (!m_move) { return; }
        if (!m_blockInteraction) { return; }
        if (!m_gamepadStick) { return; }

        m_leftStickValue = m_gamepadStick.GetGamepadValue();
        //Debug.Log(m_leftStickValue);

        //左に移動
        if ((m_leftStickValue.x < 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f)))
        {
            //入力されたキーから移動方向のベクトルを返す
            //例）A(左)が入力されたら　→　Vector2(-1.0f,0.0)をリターンする
            m_drection = GetMoveDirection(InputDirection.Left);
        }

        //右に移動
        if ((m_leftStickValue.x > 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f)))
        {
            //入力されたキーから移動方向のベクトルを返す
            m_drection = GetMoveDirection(InputDirection.Right);
        }

        //上キーの入力
        if ((m_leftStickValue == Vector2.up) || 
            ((m_leftStickValue.y > 0.8f) && ((m_leftStickValue.x < 0.6f)||(m_leftStickValue.x > -0.6f))))
        {
            //今のところ処理なし
            Debug.Log("上が入力されました");
            m_drection = GetMoveDirection(InputDirection.None);
        }

        //下キーの入力
        if ((m_leftStickValue == Vector2.down) ||
            ((m_leftStickValue.y < -0.8f) && ((m_leftStickValue.x < 0.6f) || (m_leftStickValue.x > -0.6f))))
        {
            //今のところ処理なし
            Debug.Log("下が入力されました");
            m_drection = GetMoveDirection(InputDirection.None);
        }

            //入力がない場合
            if ((m_leftStickValue == Vector2.zero))
        {
            m_drection = GetMoveDirection(InputDirection.None);
        }
        
        //移動処理開始
        //SetMovement関数に引数を渡す
        //第一引数：移動速度(float)
        //第二引数：移動方向(Vector2)
        //第三引数：移動するかどうか(bool) => trueで移動する
        m_move.SetMovement(m_moveSpeed, m_drection, true);

        //ブロックを持っていなかったら処理
        if (m_blockInteraction.GetIsGrasping() == false)
        {
            //----------------------------------------------
            //10/28 矢越
            //----------------------------------------------
            //"投げる"アニメーションが再生中ならリターン
            if (m_animCheckFlag[4]) { return; }
            //----------------------------------------------

            if (m_drection == Vector2.left)
            {
                //入力に合わせて向きを回転させる
                transform.rotation = Quaternion.Euler(0.0f, 180.0f, 0.0f);
                //レイをフリップさせるかどうか
                m_groundJudge.SetOffsetFlipX(false);
            }

            if (m_drection == Vector2.right)
            {
                //入力に合わせて向きを回転させる
                transform.rotation = Quaternion.Euler(0.0f, 0.0f, 0.0f);
                //レイをフリップさせるかどうか
                m_groundJudge.SetOffsetFlipX(true);
            }
        }
    
        //キーが押されていない場合
        if (m_drection == Vector2.zero)
        {
            m_drection = GetMoveDirection(InputDirection.None);
            m_move.SetMovement(m_moveSpeed, m_drection, false);
        }
    }

    //ブロックを掴んで投げる関数
    void HoldObject()
    {
        //NULLなら処理しない
        if (!m_blockInteraction) { return; }

        //Yボタンでブロックを掴む
        if (Input.GetKeyDown("joystick button 2") || Input.GetKeyDown("joystick button 3") ||
            Input.GetKeyDown(KeyCode.H))
        {
            //掴むことが可能なら処理
            if (m_blockInteraction.GetCanGrab())
            {
                //何も掴んでいない状態なら処理
                if (!m_blockInteraction.GetIsGrasping())
                {
                    //ブロックを掴む
                    m_blockInteraction.GrabHandle();
                    m_isGrasping = true;
                    return;
                }
            }
            
            //ブロックを掴んでいる状態なら処理
            if (m_blockInteraction.GetIsGrasping())
            {
                //下が入力状態ならその場に置く
                if ((m_leftStickValue == Vector2.down) ||
                    ((m_leftStickValue.y < -0.8f) && ((m_leftStickValue.x < 0.6f) || (m_leftStickValue.x > -0.6f))))
                {
                    m_blockInteraction.PlaceHandle();
                    m_isGrasping = false;
                    return;
                }
                //それ以外は投げる
                m_blockInteraction.ThrowHandle();

                //----------------------------------------------
                //矢越
                //10/27アニメーションの追加
                //----------------------------------------------

                //"投げる"アニメーションのフラグを上げる
                m_animCheckFlag[4] = true;
                //掴みフラグを下げる
                m_isGrasping = false;

                //----------------------------------------------
            }
        }
    }

    //動くオブジェクトの上にいるときの処理
    void OnMoveObject()
    {
        ObjectJudge tempObjectJudgeComp;//一時的に子オブジェクトのコンポーネントを保存する
        MoveFloor tempMoveFloorComp;    //一時的に受け取ったオブジェクトのコンポーネントを保存する        
        GameObject childObject;         //子オブジェクトを保存する
        GameObject underObject;         //受け取ったオブジェクトを保存する

        //0番目の子オブジェクトを取得
        childObject = transform.GetChild(0).gameObject;

        //子オブジェクトからコンポーネントを取得する
        tempObjectJudgeComp = childObject.GetComponent<ObjectJudge>();

        //コンポーネントを取得出来なかったら処理しない
        if (!tempObjectJudgeComp) { return; }

        //足元のオブジェクトを受け取る
        underObject = tempObjectJudgeComp.GetUnderObject();

        //オブジェクトを取得できなかったら処理しない
        if (!underObject) { return; }

        //取得したオブジェクトにアタッチされたコンポーネントを取得
        tempMoveFloorComp = underObject.GetComponent<MoveFloor>();

        //コンポーネントを取得できなかったら処理しない
        if (!tempMoveFloorComp) { return; }

        //移動していないときの処理
        if (underObject.tag == "MoveBlock" ) 
        {
            if (!m_groundJudge.IsGrounded())
            {
                Debug.Log("キターーーーーーーーー！！！！");
            }
            if (m_drection == Vector2.zero && m_groundJudge.IsGrounded())
            {
                //動く床と同じ移動方向に同じ速度でPlayerを動かす
                m_move.SetMovement(tempMoveFloorComp.GetMoveSpeed(),
                                   tempMoveFloorComp.GetMoveDirection(),
                                   true);
            }
            else
            {
                //移動中は補正する値を受け取る
                m_correctionMoveSpeed = tempMoveFloorComp.GetMoveSpeed();
                m_correctionMoveDirection = tempMoveFloorComp.GetMoveDirection();

                //下方向の移動方向の時だけ補正をかける
                if (m_correctionMoveDirection == Vector2.down)
                {
                    m_move.SetMovement(m_moveSpeed, m_drection + m_correctionMoveDirection, true);
                }
            }
        }
    }

    //入力されたキーをベクトルにして返す
    Vector2 GetMoveDirection(InputDirection _Inputdir)
    {
        //入力なしの場合
        if (_Inputdir == InputDirection.None)
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

    //----------------------------------------------
    //矢越
    //10/27 アニメーションの追加
    //10/26 チェックポイントの追加
    //10/30 操作説明状態の追加
    //----------------------------------------------

    //アニメーションを設定する
    private void PlayAnimation()
    {
        //----------------------------------------------
        //アニメーションを状態の設定
        //----------------------------------------------

        //取っ手を掴んでいる場合
        if (m_isGrasping || m_animCheckFlag[4])
        {
            //"掴んでいる"アニメーションにする
            m_animation = PlayerAnimation.Grab;
            
        }
        else
        {
            //"通常"アニメーションにする
            m_animation = PlayerAnimation.Normal;
        }

        //----------------------------------------------
        //プレイヤーアニメーション配列フラグの上げ下げ
        //----------------------------------------------

        //"投げる"アニメーション
        //258行でフラグを上げる(HoldObject())
        //122行でフラグを下げる(LateUpdate())

        //"ジャンプ"アニメーション
        //地面に接地していない場合
        if (!m_groundJudge.IsGrounded()) 
        {
            //"ジャンプ"アニメーションのフラグを上げる
            m_animCheckFlag[2] = true;
            m_animCheckFlag[6] = true;
        }
        else
        {
            //"ジャンプ"アニメーションのフラグを下げる
            m_animCheckFlag[2] = false;
            m_animCheckFlag[6] = false;
        }

        //"移動"アニメーション
        //横移動入力がされている場合
        if ((m_leftStickValue.x < 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f))||
            (m_leftStickValue.x > 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f)))
        {
            //"移動"アニメーションのフラグを上げる
            m_animCheckFlag[1] = true;
            m_animCheckFlag[5] = true;
        }
        else
        {
            //"移動"アニメーションのフラグを下げる
            m_animCheckFlag[1] = false;
            m_animCheckFlag[5] = false;
        }

        //----------------------------------------------
        //再生するアニメーションを設定する
        //----------------------------------------------

        switch (m_animation)
        {
            //"掴んでいる"アニメーションに設定されていたら
            case PlayerAnimation.Grab:
                //"投げる"アニメーションを再生(Player_Grab_Throw)
                if (m_animCheckFlag[4])
                {
                    m_anim.SetInteger("Player_Anim", 4); 
                }
                //"ジャンプ"アニメーションを再生(Player_Grab_Jump)
                else if (m_animCheckFlag[6])
                { 
                    m_anim.SetInteger("Player_Anim", 6); 
                }
                //"移動"アニメーションを再生(Player_Grab_Walk)
                else if (m_animCheckFlag[5]) 
                {
                    m_anim.SetInteger("Player_Anim", 5); 
                }
                //"待機"アニメーションを再生(Player_Grab_Idle)
                else 
                {
                    m_anim.SetInteger("Player_Anim", 3); 
                }
                break;

            //"通常"アニメーションに設定されていたら
            case PlayerAnimation.Normal:
                //"ジャンプ"アニメーションを再生(Player_Jump)
                if (m_animCheckFlag[2]) 
                {
                    m_anim.SetInteger("Player_Anim", 2); 
                }
                //"移動"アニメーションを再生(Player_Walk)
                else if (m_animCheckFlag[1]) 
                {
                    m_anim.SetInteger("Player_Anim", 1); 
                }
                //"待機"アニメーションを再生(Player_Idle)
                else 
                {
                    m_anim.SetInteger("Player_Anim", 0); 
                }
                break;
        }    
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        //"チェックポイントに触れる"かつ"チェックポイントフラグが下がっている"場合
        if (collision.gameObject.tag == "CheckPoint" && !m_checkPointFlag)
        {
            //リスポーン地点を触れた場所に更新
            m_respawnPosition = transform.position;
            // チェックポイントフラグを上げる
            m_checkPointFlag = true;
        }
    }

    // 操作説明 ON OFF のフラグを受け取る
    private void ReceiveOperationIn()
    {
        // フラグが上がっていたら
        if (m_gameManager.GetOperation())
        {
            m_status = PlayerStatus.Operation;

            //移動とアニメーションを停止する
            m_move.SetMovement(m_moveSpeed,m_drection,false);
            
            //ブロックをつかんでいる時の待機アニメーション
            if (m_animation == PlayerAnimation.Grab)
            {
                m_anim.SetInteger("Player_Anim", 3);
            }
            //通常時の待機アニメーション
            else
            {
                m_anim.SetInteger("Player_Anim", 0);
            }
        }
        else
        {
            m_status = PlayerStatus.Alive;
        }
    }

    //----------------------------------------------
}
