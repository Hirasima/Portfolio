using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//����ҁ@�_�J�D�P
//----------------------------------------------

public class Move : MonoBehaviour
{
    //�ړ����x
    private float m_moveSpeed;
    //�ړ�����(�O�̓��́j
    private Vector2 m_preMoveDir;
    //�ړ�����(���̓��́j
    private Vector2 m_postMoveDir;
    //�ړ����邩�ǂ���
    private bool m_isMoveing;
    //�A�b�^�`����Ă���I�u�W�F�N�g��Rigidbody���擾
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
        //�I�u�W�F�N�g��Rigidbody���A�^�b�`����ĂȂ��ꍇ
        if (!_rb)
        {
            Debug.Log("�I�u�W�F�N�g��Rigidbody���A�^�b�`���Ă��������B");
            return;
        }

        //�ړ�����t���O���オ���Ă����珈��
        if (m_isMoveing == true)
        {
            ToMove();
        }
    }
    /// <summary> 
    ///�ړ��Ɋւ���l���Z�b�g����
    /// </summary>
    /// <param name="_movespeed">�ړ����x</param>
    /// <param name="_dir">�ړ�����</param>
    /// <param name="_ismove">�ړ��������s����</param>
    public void SetMovement(float _movespeed, Vector2 _dir, bool _ismove)
    {
        //�ړ����x��ݒ肷��
        m_moveSpeed = _movespeed;
        //�ړ�������ݒ肷��
        m_preMoveDir = _dir;
        //�ړ�����t���O�����Ă�
        m_isMoveing = _ismove;
    }

    //�ړ������ɑ��x���ړ�����
    private void ToMove()
    {
        if (!_rb) { return;}

        Vector2 _nowPos = _rb.position;     //���݂̍��W���擾����

        //���͂��ꂽ�������ς��܂ňړ���������
        //���݂̍��W�Ɉړ����������x���A�ړ�������
        _nowPos += new Vector2(m_preMoveDir.x * m_moveSpeed, m_preMoveDir.y * m_moveSpeed);
        //�ړ����������𔽉f����
        _rb.position = _nowPos;
    }
}
