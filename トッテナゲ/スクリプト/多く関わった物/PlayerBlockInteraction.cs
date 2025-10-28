using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//����ҁ@�����@�v
//----------------------------------------------

public class PlayerBlockInteraction : MonoBehaviour
{
    [SerializeField] private bool m_bGrasping;   //�͂�ł��邩
    [SerializeField] private bool m_bCanGrab;    //�͂߂邩

    //�R���|�[�l���g�ۑ��p
    [SerializeField] private Collider2D m_canGrabHandle;     //���ݒ͂߂������ۑ�

    // Start is called before the first frame update
    void Start()
    {
        //�ϐ��̏�����
        m_bGrasping = false;
        m_bCanGrab = false;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //�u���Ƃ��ɌĂ�ł��炤�֐�
    public void PlaceHandle()
    {
        //�͂�ł��Ȃ��Ƃ��͏����𔲂���
        if(m_bGrasping == false) { return; }

        //�͂�ł�������̃X�N���v�g���擾���Ď����Ɂu�u���v���Ƃ�`����
        Handle handleScript = m_canGrabHandle.gameObject.GetComponent<Handle>();
        handleScript.OnPlaced();

        m_canGrabHandle = null;

        //�u�͂�ł��Ȃ��v��Ԃɂ���
        m_bGrasping = false;
    }

    //�͂ނƂ��ɌĂ�ł��炤�֐�
    public void GrabHandle()
    {
        //�͂ގ���肪�����ꍇ����������
        if(m_bCanGrab == false || m_bGrasping == true || m_canGrabHandle == null) { return; }

        ////�͂�ł�������̃X�N���v�g���擾���Ď����Ɂu�͂ށv���Ƃ�`����
        Handle handleScript = m_canGrabHandle.gameObject.GetComponent<Handle>();
        handleScript.OnGrabbed(this.transform);

        //�u�͂�ł���v��Ԃɂ���
        m_bGrasping = true;
        m_bCanGrab = false;
    }

    //������Ƃ��ɌĂ�ł��炤�֐�
    public void ThrowHandle()
    {
        //�͂�ł��Ȃ��Ƃ��͏����𔲂���
        if (m_bGrasping == false) { return; }

        //�͂�ł�������̃X�N���v�g���擾���Ď����Ɂu������v���Ƃ�`����
        Handle handleScript = m_canGrabHandle.gameObject.GetComponent<Handle>();
        handleScript.OnThrown();

        m_canGrabHandle = null;

        //�u�͂�ł��Ȃ��v��Ԃɂ���
        m_bGrasping = false;
    }

    //-----------------------------------
    //�Q�b�^�[(Get�֐�)
    //-----------------------------------

    //���ݒ͂�ł��邩��Ԃ��֐�
    public bool GetIsGrasping()
    {
        return m_bGrasping;
    }

    //���ݒ͂߂邩��Ԃ��֐�
    public bool GetCanGrab()
    {
        return m_bCanGrab;
    }

    //-----------------------------------
    //�Փˊ֐�
    //-----------------------------------

    //�G�ꂽ�Ƃ��ɂ�΂��֐�
    private void OnTriggerStay2D(Collider2D _collision)
    {
        if(_collision.gameObject.tag == "Handle" && m_canGrabHandle == null)
        {
            m_bCanGrab = true;
            m_canGrabHandle = _collision;
            _collision.GetComponent<Handle>().SetIsShine(true);
        }
    }

    //�G��Ȃ��Ȃ����Ƃ��ɂ�΂��֐�
    private void OnTriggerExit2D(Collider2D _collision)
    {
        if (_collision.gameObject.tag == "Handle" && m_bGrasping == false)
        {
            _collision.GetComponent<Handle>().SetIsShine(false);
            m_bCanGrab = false;
            m_canGrabHandle = null;
        }
    }
}
