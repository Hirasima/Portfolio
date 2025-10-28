using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------
//����ҁ@�����@�v
//----------------------------------------------

public class GroundingJudgmenter: MonoBehaviour
{
    [SerializeField] private float m_playerWidth;    //�v���C���[�̕�
    [SerializeField] private float m_playerHeight;   //�v���C���[�̍���
    [SerializeField] private float m_offsetX;         //����̒��S����̃Y��

    private bool m_bOffsetFlipX;       //�I�t�Z�b�g���]�t���O


    [SerializeField] private LayerMask groundLayer;    //�n�ʂƂ��Ĕ��ʂ���I�u�W�F�N�g�̃��C���[

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

    //�ڒn���Ă���Ƃ���bool�^��true��Ԃ��֐�
    public bool IsGrounded()
    {
        bool result = false;

        //���C���΂��ĉ��ɒn�ʂ����邩���ׂ�
        //�X�^�b�N��h�����ߓ�{���C���΂�

        //�v���C���[��postion����v���C���[�̕��̕����E�ɂ��炵�ē�{��΂�
        float offset = m_offsetX;
        if (m_bOffsetFlipX) offset = -offset;
        Vector2 rightRayOrigin = new Vector2(transform.position.x + m_playerWidth + offset / 2.0f, transform.position.y);
        Vector2 leftRayOrigin = new Vector2(transform.position.x - m_playerWidth + offset / 2.0f, transform.position.y);

        RaycastHit2D rightRay = Physics2D.Raycast(rightRayOrigin, Vector2.down, m_playerHeight / 2.0f, groundLayer);
        RaycastHit2D leftRay = Physics2D.Raycast(leftRayOrigin, Vector2.down, m_playerHeight / 2.0f, groundLayer);

        //�ǂ��炩�̃��C���n�ʂɐڐG���Ă����猋�ʂ�ture�ɂ���
        if (rightRay == true || leftRay == true)
        {
            result = true;
        }

        return result;
    }

    //---------------------------------------
    //Get�֐�
    //---------------------------------------

    //�I�t�Z�b�g���擾
    public float GetOffsetX()
    {
        return m_offsetX;
    }

    //---------------------------------------
    //Set�֐�
    //---------------------------------------

    //X�����̃I�t�Z�b�g��ݒ�
    public void SetOffsetX(float _offsetX)
    {
        m_offsetX = _offsetX;
    }

    //X�����̃I�t�Z�b�g�𔽓]���邩�ݒ�
    public void SetOffsetFlipX(bool _flip)
    {
        m_bOffsetFlipX = _flip;
    }

    //�f�o�b�N�p�֐�
    //���C��`��
    void DrowRay()
    {
        //���C�̌��_��ݒ�
        float offset = m_offsetX;
        if (m_bOffsetFlipX) offset = -offset;
        Vector2 rightRayOrigin = new Vector2(transform.position.x + m_playerWidth + offset / 2.0f, transform.position.y);
        Vector2 leftRayOrigin = new Vector2(transform.position.x - m_playerWidth + offset / 2.0f, transform.position.y);

        //���C��`��
        Debug.DrawRay(rightRayOrigin, new Vector3(0, -m_playerHeight / 2.0f, 0), Color.red);
        Debug.DrawRay(leftRayOrigin, new Vector3(0, -m_playerHeight / 2.0f, 0), Color.red);
    }
}
