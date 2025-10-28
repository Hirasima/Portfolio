using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//-------------------------------------
//����ҁF�_�J�D�P
//-------------------------------------

public class MoveObject : MonoBehaviour
{
    [SerializeField]
    private float m_maxRange;               //�ړ��͈͂̏��
    [SerializeField]
    private float m_minRange;               //�ړ��͈͂̉���
    [SerializeField]
    private float m_moveSpeed;              //�ړ����x
    private Vector2 m_startPosition;        //���������W
    private Vector2 m_currentPosition;      //�ړ���̍��W

    private enum MoveDirection
    {
        None = -1,      //���͂Ȃ�
        Vertical = 0,   //���� 0
        Horizontal = 2, //���� 2
    }
    [SerializeField] private MoveDirection m_Direction;

    // Start is called before the first frame update
    void Start()
    {
        //�n�܂������̍��W��ۑ�����
        m_startPosition = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        Moveing(m_Direction);
    }

    void Moveing(MoveDirection _moveDirection)
    {
        //�ړ���̍��W���擾
        m_currentPosition = this.transform.position;

        //�ړ������ɂ���ď�����ς���
        switch(_moveDirection)
        {
            //��������
            case MoveDirection.Horizontal:
                //�ړ��͈͂̏���𒴂����牺�Ɉړ�����
                if (m_startPosition.x + m_maxRange < m_currentPosition.x)
                {
                    //�ړ����x���{(�v���X)��������[(�}�C�i�X)�ɂ���
                    if (m_moveSpeed > 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //�ړ��͈͂̉����𒴂������Ɉړ�����
                if (m_startPosition.x - m_minRange > m_currentPosition.x)
                {
                    //�ړ����x���[(�}�C�i�X)�Ȃ�{(�v���X)�ɂ���
                    if (m_moveSpeed < 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //�ړ������J�n
                transform.position = new Vector2(m_currentPosition.x + m_moveSpeed, m_currentPosition.y);
                
                break;

            //��������
            case MoveDirection.Vertical:
                //�ړ��͈͂̏���𒴂����牺�Ɉړ�����
                if (m_startPosition.y + m_maxRange < m_currentPosition.y)
                {
                    //�ړ����x���{(�v���X)��������[(�}�C�i�X)�ɂ���
                    if (m_moveSpeed > 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //�ړ��͈͂̉����𒴂������Ɉړ�����
                if (m_startPosition.y - m_minRange > m_currentPosition.y)
                {
                    //�ړ����x���[(�}�C�i�X)�Ȃ�{(�v���X)�ɂ���
                    if (m_moveSpeed < 0.0f)
                    {
                        m_moveSpeed *= -1.0f;
                    }
                }

                //�ړ������J�n
                transform.position = new Vector2(m_currentPosition.x, m_currentPosition.y + m_moveSpeed);

                break;

            //���͂Ȃ�
            case MoveDirection.None:
                Debug.LogWarning("�ړ����������Ă�����Ă��܂���");
                break;
        }
    }
    private void OnCollisionEnter2D(Collision2D collision)
    {
        //���������I�u�W�F�N�g�̃^�O���v���C���[�Ȃ珈��
        if (collision.gameObject.tag == "Player")
        {
            //�v���C���[���q�I�u�W�F�N�g�ɂ���
            collision.gameObject.transform.SetParent(this.gameObject.transform);
        }
    }
    private void OnCollisionExit2D(Collision2D collision)
    {
        //���������I�u�W�F�N�g�̃^�O���v���C���[�Ȃ珈��
        if (collision.gameObject.tag == "Player")
        {
            //�v���C���[���q�I�u�W�F�N�g�ɂ���
            collision.gameObject.transform.SetParent(null);
        }
    }
}
