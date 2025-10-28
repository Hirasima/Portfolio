using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//����ҁ@�����@�v
//----------------------------------------------

public class Block : MonoBehaviour
{
    enum BlockStatus
    {
        placed,
        grabbed,
        thrown
    };
    [SerializeField] private BlockStatus m_status;       //�u���b�N�̌��݂̏�ԕۑ��p
    enum Direction
    {
        right,
        left
    };
    private Direction m_grabDire;    //�͂܂ꂽ����

    private int m_stopFrameCount;
    
    [SerializeField] private float m_throwPower;    //��������͂̋���

    private Transform m_grabberTransform;   //�͂�ł���I�u�W�F�N�g��transform
    private Vector3 m_grabberPos;           //�͂�ł���I�u�W�F�N�g�̈ʒu

    private Rigidbody2D _rb;            //Rigidbody�ۑ��p
    [SerializeField] private LayerMask m_groundLayer;

    // Start is called before the first frame update
    void Start()
    {
        //�R���|�[�l���g�̏�����
        _rb = GetComponent<Rigidbody2D>();
        _rb.bodyType = RigidbodyType2D.Dynamic;
        SetBlockStatus(BlockStatus.placed);

        //�v���p�e�B�̏�����
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
            //�u�u����Ă���v�Ƃ�
            case BlockStatus.placed:

                break;

            //�u�͂܂�Ă���v�Ƃ�
            case BlockStatus.grabbed:
                //�͂�ł���I�u�W�F�N�g�̑��Έʒu���L�[�v
                //���݈ʒu = �͂�ł���I�u�W�F�N�g�̌��݈ʒu + �͂܂ꂽ���̒͂�ł���I�u�W�F�N�g�Ƃ̋���
                this.transform.position = m_grabberTransform.position + m_grabberPos;

                break;

            //�u�������Ă���v�Ƃ�
            case BlockStatus.thrown:
                //X������velocity���[�� �܂��� �������Ă�������Ƌt��velocity���������Ă�����
                if (_rb.velocity.x * dire[(int)m_grabDire] <= 0.0f)
                {
                    //�~�܂��Ă���t���[�����ɉ��Z
                    ++m_stopFrameCount;
                }
                //�܂������Ă����烊�Z�b�g
                else m_stopFrameCount = 0;

                //�~�܂��Ă���t���[������l�𒴂�����
                if(m_stopFrameCount >= 10)
                {
                    //�X�e�[�^�X���u�u����Ă���v�ɕύX
                    SetBlockStatus(BlockStatus.placed);
                    //�J�E���g��������
                    m_stopFrameCount = 0;
                }

                break;
        }
    }

    //�u���ꂽ�Ƃ��ɌĂ�ł��炤�֐�
    public void OnPlaced()
    {

        //�X�e�[�^�X���u�u����Ă���v�ɕύX
        SetBlockStatus(BlockStatus.placed);

        //�͂�ł���I�u�W�F�N�g���ݒ肳��Ă�����o�^����
        if (m_grabberTransform != null)
        {
            m_grabberTransform = null;
        }
    }

    //�͂܂ꂽ�Ƃ��ɌĂ�ł��炤�֐�
    public void OnGrabbed(Transform _grabber)
    {
        //�X�e�[�^�X���u�͂܂�Ă���v�ɕύX
        SetBlockStatus(BlockStatus.grabbed);

        //�͂�ł���I�u�W�F�N�g��o�^
        m_grabberTransform = _grabber;
        //�͂�ł���I�u�W�F�N�g����̑��Έʒu��ۑ�
        m_grabberPos = this.transform.position - m_grabberTransform.position;
    }

    //������ꂽ�Ƃ��ɌĂ�ł��炤�֐�
    public void OnThrown()
    {

        //�X�e�[�^�X���u�������Ă���v�ɕύX
        SetBlockStatus(BlockStatus.thrown);

        Throw(m_grabDire);

        //�e�I�u�W�F�N�g���ݒ肳��Ă�����o�^����
        if (m_grabberTransform != null)
        {
            m_grabberTransform = null;
        }
    }

    //��������֐�
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

    //�͂܂�Ă��������ݒ�
    public void SetGrabbedDirection(int _handleDire)
    {
        m_grabDire = (Direction)_handleDire;
    }

    //�u���b�N�̏�Ԃ�ݒ�
    private void SetBlockStatus(BlockStatus _status)
    {
        //�u�͂܂�Ă�v�ւ̕ύX
        if (_status == BlockStatus.grabbed)
        {
            _rb.gravityScale = 0.0f;

            _rb.constraints = RigidbodyConstraints2D.FreezeRotation;
        }
        //�u�������Ă���v�ւ̕ύX
        else if (_status == BlockStatus.thrown)
        {
            _rb.gravityScale = 0.0f;

            _rb.constraints = RigidbodyConstraints2D.FreezePositionY | RigidbodyConstraints2D.FreezeRotation;
        }
        //�u�u����Ă���v�ւ̕ύX
        else
        {
            m_stopFrameCount = 0;

            _rb.gravityScale = 1.0f;

            _rb.constraints = RigidbodyConstraints2D.FreezePositionX | RigidbodyConstraints2D.FreezeRotation;
        }
        m_status = _status;
    }
}
