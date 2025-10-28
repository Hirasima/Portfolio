using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

//----------------------------------------------
//����ҁ@�_�J�D�P
//----------------------------------------------

//10/26 ��z �`�F�b�N�|�C���g��ǉ����܂����B
//21�`23�A65�`67�A324�`334�s

public class PlayerCotroller : MonoBehaviour
{
    private bool m_isGrasping;
    private float m_moveSpeed;                  //�ړ����x
    private float m_jumpPower;                  //�W�����v��
    private Vector2 m_drection;                 //�ړ�����
    private Vector2 m_leftStickValue;           //���X�e�B�b�N������͂��ꂽ�l

    //----------------------------------------------
    //��z
    //10/26 �`�F�b�N�|�C���g�̒ǉ�
    //10/27�A�j���[�V�����̒ǉ�
    //----------------------------------------------

    private bool[] m_animCheckFlag;             //�v���C���[�A�j���[�V�����z��t���O

    public static Vector2 m_startPosition;      //�X�^�[�g�n�_
    public static Vector2 m_respawnPosition;    //���X�|�[���n�_
    public static bool m_checkPointFlag = false;//�`�F�b�N�|�C���g�ɐG�ꂽ��

    //----------------------------------------------

    //�ړ����ɏ���Ă���Ƃ��̕␳�l
    private float m_correctionMoveSpeed;        //�ړ����x�̕␳�l
    private Vector2 m_correctionMoveDirection;  //�ړ������̕␳�l

    //�ǂݎ���p
    readonly float m_defaultSpeed = 0.07f;      //�ʏ푬�x

    //�e�R���|�[�l���g��ۑ�����ϐ�
    private Animator m_anim;
    private Rigidbody2D m_rb;
    private Move m_move;
    private GroundingJudgmenter m_groundJudge;
    private PlayerBlockInteraction m_blockInteraction;
    private GamepadStick m_gamepadStick;

    //----------------------------------------------
    //��z
    //10/30 Dead->Operation
    //----------------------------------------------
    [SerializeField]
    private GameManager m_gameManager;

    private enum PlayerStatus
    {
        Alive,      //����
        Operation,  //�������
    }
    PlayerStatus m_status;

    //----------------------------------------------

    private enum InputDirection
    {
        None = -1,  //���͂Ȃ�
        Up,     //�� 0
        Down,   //�� 1
        Right,  //�E 2
        Left,   //�� 3
    }

    //----------------------------------------------
    //��z
    //10/26 �`�F�b�N�|�C���g�̒ǉ�
    //10/27 �A�j���[�V�����̒ǉ�
    //----------------------------------------------

    //�v���C���[�A�j���[�V�����̏��
    private enum PlayerAnimation
    {
        Normal, //�ʏ�
        Grab,   //�͂�ł���
    }
    PlayerAnimation m_animation;

    //----------------------------------------------

    private void Start()
    {
        //�ϐ�������������
        m_isGrasping = false;
        m_moveSpeed = m_defaultSpeed;
        m_jumpPower = 350.0f;
        m_drection = new Vector2(0.0f, 0.0f);
        m_status = PlayerStatus.Alive;       

        //----------------------------------------------
        //��z
        //10/26 �`�F�b�N�|�C���g�̒ǉ�
        //10/27�A�j���[�V�����̒ǉ�
        //----------------------------------------------
        
        //�A�j���[�V�����̔z�������������
        //�ʏ��Ԃ̃A�j���[�V����
        //0.Player_Idle  1.Player_Walk  2.Playe_Jump
        //�͂ݏ�Ԃ̃A�j���[�V����
        //3.Player_Grab_Idle  4.Player_Grab_Throw  5.Player_Grab_Walk  6.Player_Grab_Jump
        m_animCheckFlag = new bool[7] { true, false, false, true, false, false, false };

        //�v���C���[�A�j���[�V����������������
        m_animation = PlayerAnimation.Normal;
        //�X�^�[�g�n�_������������
        m_startPosition = transform.position;
        //�`�F�b�N�|�C���g�t���O���オ���Ă���ꍇ
        if (m_checkPointFlag) transform.position = m_respawnPosition;

        //----------------------------------------------

        //�K�v�ȃR���|�[�l���g���擾����
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
        //��z
        //10/30 ���������Ԃ̒ǉ�
        //----------------------------------------------

        ReceiveOperationIn();

        //----------------------------------------------

        //�v���C���[�̏�Ԃŏ�����ς���
        switch (m_status)
        {
            //���������ԂȂ�
            case PlayerStatus.Operation:
                break;

            //������ԂȂ�
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
    //��z
    //10/27�A�j���[�V�����̒ǉ�
    //----------------------------------------------
    private void LateUpdate()
    {   
        //"������"�A�j���[�V�������Ō�܂ōĐ����ꂽ�ꍇ
        if (m_anim.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f && m_anim.GetInteger("Player_Anim") == 4)
        {
            //"������"�A�j���[�V�����̃t���O��������
            m_animCheckFlag[4] = false;
        }
    }

    //----------------------------------------------

    //�W�����v����
    void Jump()
    {
        //NULL�`�F�b�N
        if (!m_rb){ return; }
        if (!m_groundJudge){ return; }

        //�n�ʂɐڒn���Ă����珈��
        if (m_groundJudge.IsGrounded())
        {
            //�Q�[���p�b�h��A�AB�{�^���̂ǂ��炩�������ꂽ�珈������
            if (Input.GetKeyDown("joystick button 0") || Input.GetKeyDown("joystick button 1") ||
                Input.GetKeyDown(KeyCode.Space))
            {
                m_rb.velocity = new Vector2(0.0f, 0.0f);
                //������ɗ͂�������
                m_rb.AddForce(new Vector2(0.0f, 1.0f * m_jumpPower));
            }
        }
    }

    //���͂��󂯁A����������֐�
    void GetInputDevice()
    {
        //�e�R���|�[�l���g��NULL�Ȃ珈�����Ȃ�
        if (!m_rb) { return; }
        if (!m_move) { return; }
        if (!m_blockInteraction) { return; }
        if (!m_gamepadStick) { return; }

        m_leftStickValue = m_gamepadStick.GetGamepadValue();
        //Debug.Log(m_leftStickValue);

        //���Ɉړ�
        if ((m_leftStickValue.x < 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f)))
        {
            //���͂��ꂽ�L�[����ړ������̃x�N�g����Ԃ�
            //��jA(��)�����͂��ꂽ��@���@Vector2(-1.0f,0.0)�����^�[������
            m_drection = GetMoveDirection(InputDirection.Left);
        }

        //�E�Ɉړ�
        if ((m_leftStickValue.x > 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f)))
        {
            //���͂��ꂽ�L�[����ړ������̃x�N�g����Ԃ�
            m_drection = GetMoveDirection(InputDirection.Right);
        }

        //��L�[�̓���
        if ((m_leftStickValue == Vector2.up) || 
            ((m_leftStickValue.y > 0.8f) && ((m_leftStickValue.x < 0.6f)||(m_leftStickValue.x > -0.6f))))
        {
            //���̂Ƃ��돈���Ȃ�
            Debug.Log("�オ���͂���܂���");
            m_drection = GetMoveDirection(InputDirection.None);
        }

        //���L�[�̓���
        if ((m_leftStickValue == Vector2.down) ||
            ((m_leftStickValue.y < -0.8f) && ((m_leftStickValue.x < 0.6f) || (m_leftStickValue.x > -0.6f))))
        {
            //���̂Ƃ��돈���Ȃ�
            Debug.Log("�������͂���܂���");
            m_drection = GetMoveDirection(InputDirection.None);
        }

            //���͂��Ȃ��ꍇ
            if ((m_leftStickValue == Vector2.zero))
        {
            m_drection = GetMoveDirection(InputDirection.None);
        }
        
        //�ړ������J�n
        //SetMovement�֐��Ɉ�����n��
        //�������F�ړ����x(float)
        //�������F�ړ�����(Vector2)
        //��O�����F�ړ����邩�ǂ���(bool) => true�ňړ�����
        m_move.SetMovement(m_moveSpeed, m_drection, true);

        //�u���b�N�������Ă��Ȃ������珈��
        if (m_blockInteraction.GetIsGrasping() == false)
        {
            //----------------------------------------------
            //10/28 ��z
            //----------------------------------------------
            //"������"�A�j���[�V�������Đ����Ȃ烊�^�[��
            if (m_animCheckFlag[4]) { return; }
            //----------------------------------------------

            if (m_drection == Vector2.left)
            {
                //���͂ɍ��킹�Č�������]������
                transform.rotation = Quaternion.Euler(0.0f, 180.0f, 0.0f);
                //���C���t���b�v�����邩�ǂ���
                m_groundJudge.SetOffsetFlipX(false);
            }

            if (m_drection == Vector2.right)
            {
                //���͂ɍ��킹�Č�������]������
                transform.rotation = Quaternion.Euler(0.0f, 0.0f, 0.0f);
                //���C���t���b�v�����邩�ǂ���
                m_groundJudge.SetOffsetFlipX(true);
            }
        }
    
        //�L�[��������Ă��Ȃ��ꍇ
        if (m_drection == Vector2.zero)
        {
            m_drection = GetMoveDirection(InputDirection.None);
            m_move.SetMovement(m_moveSpeed, m_drection, false);
        }
    }

    //�u���b�N��͂�œ�����֐�
    void HoldObject()
    {
        //NULL�Ȃ珈�����Ȃ�
        if (!m_blockInteraction) { return; }

        //Y�{�^���Ńu���b�N��͂�
        if (Input.GetKeyDown("joystick button 2") || Input.GetKeyDown("joystick button 3") ||
            Input.GetKeyDown(KeyCode.H))
        {
            //�͂ނ��Ƃ��\�Ȃ珈��
            if (m_blockInteraction.GetCanGrab())
            {
                //�����͂�ł��Ȃ���ԂȂ珈��
                if (!m_blockInteraction.GetIsGrasping())
                {
                    //�u���b�N��͂�
                    m_blockInteraction.GrabHandle();
                    m_isGrasping = true;
                    return;
                }
            }
            
            //�u���b�N��͂�ł����ԂȂ珈��
            if (m_blockInteraction.GetIsGrasping())
            {
                //�������͏�ԂȂ炻�̏�ɒu��
                if ((m_leftStickValue == Vector2.down) ||
                    ((m_leftStickValue.y < -0.8f) && ((m_leftStickValue.x < 0.6f) || (m_leftStickValue.x > -0.6f))))
                {
                    m_blockInteraction.PlaceHandle();
                    m_isGrasping = false;
                    return;
                }
                //����ȊO�͓�����
                m_blockInteraction.ThrowHandle();

                //----------------------------------------------
                //��z
                //10/27�A�j���[�V�����̒ǉ�
                //----------------------------------------------

                //"������"�A�j���[�V�����̃t���O���グ��
                m_animCheckFlag[4] = true;
                //�͂݃t���O��������
                m_isGrasping = false;

                //----------------------------------------------
            }
        }
    }

    //�����I�u�W�F�N�g�̏�ɂ���Ƃ��̏���
    void OnMoveObject()
    {
        ObjectJudge tempObjectJudgeComp;//�ꎞ�I�Ɏq�I�u�W�F�N�g�̃R���|�[�l���g��ۑ�����
        MoveFloor tempMoveFloorComp;    //�ꎞ�I�Ɏ󂯎�����I�u�W�F�N�g�̃R���|�[�l���g��ۑ�����        
        GameObject childObject;         //�q�I�u�W�F�N�g��ۑ�����
        GameObject underObject;         //�󂯎�����I�u�W�F�N�g��ۑ�����

        //0�Ԗڂ̎q�I�u�W�F�N�g���擾
        childObject = transform.GetChild(0).gameObject;

        //�q�I�u�W�F�N�g����R���|�[�l���g���擾����
        tempObjectJudgeComp = childObject.GetComponent<ObjectJudge>();

        //�R���|�[�l���g���擾�o���Ȃ������珈�����Ȃ�
        if (!tempObjectJudgeComp) { return; }

        //�����̃I�u�W�F�N�g���󂯎��
        underObject = tempObjectJudgeComp.GetUnderObject();

        //�I�u�W�F�N�g���擾�ł��Ȃ������珈�����Ȃ�
        if (!underObject) { return; }

        //�擾�����I�u�W�F�N�g�ɃA�^�b�`���ꂽ�R���|�[�l���g���擾
        tempMoveFloorComp = underObject.GetComponent<MoveFloor>();

        //�R���|�[�l���g���擾�ł��Ȃ������珈�����Ȃ�
        if (!tempMoveFloorComp) { return; }

        //�ړ����Ă��Ȃ��Ƃ��̏���
        if (underObject.tag == "MoveBlock" ) 
        {
            if (!m_groundJudge.IsGrounded())
            {
                Debug.Log("�L�^�[�[�[�[�[�[�[�[�[�I�I�I�I");
            }
            if (m_drection == Vector2.zero && m_groundJudge.IsGrounded())
            {
                //�������Ɠ����ړ������ɓ������x��Player�𓮂���
                m_move.SetMovement(tempMoveFloorComp.GetMoveSpeed(),
                                   tempMoveFloorComp.GetMoveDirection(),
                                   true);
            }
            else
            {
                //�ړ����͕␳����l���󂯎��
                m_correctionMoveSpeed = tempMoveFloorComp.GetMoveSpeed();
                m_correctionMoveDirection = tempMoveFloorComp.GetMoveDirection();

                //�������̈ړ������̎������␳��������
                if (m_correctionMoveDirection == Vector2.down)
                {
                    m_move.SetMovement(m_moveSpeed, m_drection + m_correctionMoveDirection, true);
                }
            }
        }
    }

    //���͂��ꂽ�L�[���x�N�g���ɂ��ĕԂ�
    Vector2 GetMoveDirection(InputDirection _Inputdir)
    {
        //���͂Ȃ��̏ꍇ
        if (_Inputdir == InputDirection.None)
        {
            return Vector2.zero;
        }

        Vector2 inputdirection = new Vector2(0.0f, 0.0f);

        //�L���ȕ��������͂��ꂽ�ꍇ
        Vector2[] directionVectors = {
            Vector2.up,
            Vector2.down,
            Vector2.right,
            Vector2.left,
        };

        return directionVectors[(int)_Inputdir];
    }

    //----------------------------------------------
    //��z
    //10/27 �A�j���[�V�����̒ǉ�
    //10/26 �`�F�b�N�|�C���g�̒ǉ�
    //10/30 ���������Ԃ̒ǉ�
    //----------------------------------------------

    //�A�j���[�V������ݒ肷��
    private void PlayAnimation()
    {
        //----------------------------------------------
        //�A�j���[�V��������Ԃ̐ݒ�
        //----------------------------------------------

        //������͂�ł���ꍇ
        if (m_isGrasping || m_animCheckFlag[4])
        {
            //"�͂�ł���"�A�j���[�V�����ɂ���
            m_animation = PlayerAnimation.Grab;
            
        }
        else
        {
            //"�ʏ�"�A�j���[�V�����ɂ���
            m_animation = PlayerAnimation.Normal;
        }

        //----------------------------------------------
        //�v���C���[�A�j���[�V�����z��t���O�̏グ����
        //----------------------------------------------

        //"������"�A�j���[�V����
        //258�s�Ńt���O���グ��(HoldObject())
        //122�s�Ńt���O��������(LateUpdate())

        //"�W�����v"�A�j���[�V����
        //�n�ʂɐڒn���Ă��Ȃ��ꍇ
        if (!m_groundJudge.IsGrounded()) 
        {
            //"�W�����v"�A�j���[�V�����̃t���O���グ��
            m_animCheckFlag[2] = true;
            m_animCheckFlag[6] = true;
        }
        else
        {
            //"�W�����v"�A�j���[�V�����̃t���O��������
            m_animCheckFlag[2] = false;
            m_animCheckFlag[6] = false;
        }

        //"�ړ�"�A�j���[�V����
        //���ړ����͂�����Ă���ꍇ
        if ((m_leftStickValue.x < 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f))||
            (m_leftStickValue.x > 0.0f) && ((m_leftStickValue.y < 0.6f) && (m_leftStickValue.y > -0.6f)))
        {
            //"�ړ�"�A�j���[�V�����̃t���O���グ��
            m_animCheckFlag[1] = true;
            m_animCheckFlag[5] = true;
        }
        else
        {
            //"�ړ�"�A�j���[�V�����̃t���O��������
            m_animCheckFlag[1] = false;
            m_animCheckFlag[5] = false;
        }

        //----------------------------------------------
        //�Đ�����A�j���[�V������ݒ肷��
        //----------------------------------------------

        switch (m_animation)
        {
            //"�͂�ł���"�A�j���[�V�����ɐݒ肳��Ă�����
            case PlayerAnimation.Grab:
                //"������"�A�j���[�V�������Đ�(Player_Grab_Throw)
                if (m_animCheckFlag[4])
                {
                    m_anim.SetInteger("Player_Anim", 4); 
                }
                //"�W�����v"�A�j���[�V�������Đ�(Player_Grab_Jump)
                else if (m_animCheckFlag[6])
                { 
                    m_anim.SetInteger("Player_Anim", 6); 
                }
                //"�ړ�"�A�j���[�V�������Đ�(Player_Grab_Walk)
                else if (m_animCheckFlag[5]) 
                {
                    m_anim.SetInteger("Player_Anim", 5); 
                }
                //"�ҋ@"�A�j���[�V�������Đ�(Player_Grab_Idle)
                else 
                {
                    m_anim.SetInteger("Player_Anim", 3); 
                }
                break;

            //"�ʏ�"�A�j���[�V�����ɐݒ肳��Ă�����
            case PlayerAnimation.Normal:
                //"�W�����v"�A�j���[�V�������Đ�(Player_Jump)
                if (m_animCheckFlag[2]) 
                {
                    m_anim.SetInteger("Player_Anim", 2); 
                }
                //"�ړ�"�A�j���[�V�������Đ�(Player_Walk)
                else if (m_animCheckFlag[1]) 
                {
                    m_anim.SetInteger("Player_Anim", 1); 
                }
                //"�ҋ@"�A�j���[�V�������Đ�(Player_Idle)
                else 
                {
                    m_anim.SetInteger("Player_Anim", 0); 
                }
                break;
        }    
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        //"�`�F�b�N�|�C���g�ɐG���"����"�`�F�b�N�|�C���g�t���O���������Ă���"�ꍇ
        if (collision.gameObject.tag == "CheckPoint" && !m_checkPointFlag)
        {
            //���X�|�[���n�_��G�ꂽ�ꏊ�ɍX�V
            m_respawnPosition = transform.position;
            // �`�F�b�N�|�C���g�t���O���グ��
            m_checkPointFlag = true;
        }
    }

    // ������� ON OFF �̃t���O���󂯎��
    private void ReceiveOperationIn()
    {
        // �t���O���オ���Ă�����
        if (m_gameManager.GetOperation())
        {
            m_status = PlayerStatus.Operation;

            //�ړ��ƃA�j���[�V�������~����
            m_move.SetMovement(m_moveSpeed,m_drection,false);
            
            //�u���b�N������ł��鎞�̑ҋ@�A�j���[�V����
            if (m_animation == PlayerAnimation.Grab)
            {
                m_anim.SetInteger("Player_Anim", 3);
            }
            //�ʏ펞�̑ҋ@�A�j���[�V����
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
