using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveFloor : MonoBehaviour
{
    private bool m_moveStartFlag;                //�ړ����n�߂邽�߂̃t���O
    private bool m_isMove;
    [SerializeField]
    private float m_maxRange;               //�ړ��͈͂̏��
    [SerializeField]
    private float m_minRange;               //�ړ��͈͂̉���
    [SerializeField]
    private float m_moveSpeed;              //�ړ����x
    private Vector2 m_startPosition;        //���������W
    private Vector2 m_currentPosition;      //�ړ���̍��W
    private Vector2 m_moveDirection;        //�ړ�����
    private Move m_move;                    //Move�R���|�[�l���g�i�[�p

    private enum MoveDirection
    {
        None = -1,      //���͂Ȃ�
        Vertical = 0,   //���� 0
        Horizontal = 2, //���� 2
    }
    [SerializeField] private MoveDirection m_Direction;


    private void Start()
    {
        //�ϐ��̏�����
        m_moveStartFlag = false;
        m_isMove = true;
        m_startPosition = this.transform.position;
        m_moveDirection = Vector2.zero;

        //���g�ɃA�^�b�`����Ă���R���|�[�l���g���擾����
        m_move = this.GetComponent<Move>();
        
        if (m_moveSpeed == 0.0f)
        {
            Debug.Log(gameObject.name + " �̈ړ����x�������l�Őݒ肳��Ă��܂�");
            m_moveSpeed = 0.05f;
        }
    }

    private void Update()
    {
        //Move.cs���A�^�b�`����ĂȂ��ꍇ�������Ȃ�
        if(!m_move)
        {
            Debug.Log("Move�R���|�[�l���g���A�^�b�`���Ă��������B");
            return; 
        }
                
        //�ړ������ɂ���ď�����ς���
        switch(m_Direction)
        {
            //���͂���ĂȂ��ꍇ
            case MoveDirection.None:
                Debug.Log("�i�s��������͂��Ă��������B");
                break;

            //���������̏���
            case MoveDirection.Horizontal:
                //�ړ���̍��W���擾����
                m_currentPosition = this.transform.position;
                
                //�ړ��͈͂̏������������ړ�������ς���
                if (m_startPosition.x + m_maxRange < m_currentPosition.x )
                {
                    //�������ֈړ�
                    m_moveDirection = GetMoveDirection(m_Direction + 1);
                    //�ړ��J�n
                    m_move.SetMovement(m_moveSpeed, m_moveDirection,m_isMove);
                    return;
                }

                //�ړ��͈͂̉��������������ړ�������ς���
                if (!m_moveStartFlag || m_startPosition.x - m_minRange > m_currentPosition.x)
                {
                    //������ֈړ�
                    m_moveDirection = GetMoveDirection(m_Direction);
                    //�ړ��J�n
                    m_move.SetMovement(m_moveSpeed, m_moveDirection,m_isMove);
                }

                //�ړ����n�܂�����t���O�����낷
                if (!m_moveStartFlag)
                {
                    m_moveStartFlag = true;
                }
                break;

            //���������̏���
            case MoveDirection.Vertical:
                //�ړ���̍��W���擾����
                m_currentPosition = this.transform.position;

                // �ړ��͈͂̏������������ړ�������ς���
                if (m_startPosition.y + m_maxRange < m_currentPosition.y)
                {
                    //�������ֈړ�
                    m_moveDirection = GetMoveDirection(m_Direction + 1);
                    //�ړ��J�n
                    m_move.SetMovement(m_moveSpeed, m_moveDirection, m_isMove);
                }

                //�ړ��͈͂̉��������������ړ�������ς���
                if (!m_moveStartFlag || m_startPosition.y - m_minRange > m_currentPosition.y)
                {
                    //������ֈړ�
                    m_moveDirection = GetMoveDirection(m_Direction);
                    //�ړ��J�n
                    m_move.SetMovement(m_moveSpeed, m_moveDirection, m_isMove);
                }

                //�ړ����n�܂�����t���O�����낷
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

    //���͂��ꂽ�L�[���x�N�g���ɂ��ĕԂ�
    Vector2 GetMoveDirection(MoveDirection _Inputdir)
    {
        //���͂Ȃ��̏ꍇ
        if (_Inputdir == MoveDirection.None)
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
}