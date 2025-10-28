using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//����ҁ@�����@�v
//----------------------------------------------

public class Switch : MonoBehaviour
{
    //�X�C�b�`�̏��
    enum SwitchStatus
    {
        sleep,
        wake,
    }
    SwitchStatus m_status;

    //�X�C�b�`���I�����ǂ���
    private bool m_isTurnOn;

    //�X�C�b�`�I�����̉摜
    [SerializeField] private Sprite m_onSprite;

    //�X�C�b�`�I�t���̉摜
    private Sprite m_offSprite;

    //�X�v���C�g�����_���[
    private SpriteRenderer _sr;

    // Start is called before the first frame update
    void Start()
    {
        m_isTurnOn = false;
        m_status = SwitchStatus.wake;
        _sr = this.GetComponent<SpriteRenderer>();
        m_offSprite = _sr.sprite;
    }

    // Update is called once per frame
    void Update()
    {
        //�N�����Ȃ�
        if(m_status == SwitchStatus.wake)
        {
            if(m_isTurnOn)
            {
                //�摜��؂�ւ���
                _sr.sprite = m_onSprite;
            }
            else
            {
                //�摜��؂�ւ���
                _sr.sprite = m_offSprite;
            }
        }
        //��N�����Ȃ�
        else
        {
            //�R���W�����𖳌�
            this.GetComponent<Collider2D>().enabled = false;
            //�摜��؂�ւ���
            _sr.sprite = m_onSprite;
            //�摜���Â�����
            _sr.color = Color.gray;
        }
    }

    //�X�C�b�`�̏�Ԃ�ݒ�
    public void SetSwitchStatus(bool _status)
    {
        if(_status)
        {
            m_status = SwitchStatus.wake;
        }
        else
        {
            m_status = SwitchStatus.sleep;
        }
    }

    //�X�C�b�`���I���Ȃ�true�A�I�t�Ȃ�false���擾
    public bool GetIsTurnOn()
    {
        return m_isTurnOn;
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        //�G�ꂽ�̂��v���C���[�܂��̓u���b�N�̎�
        if(collision.gameObject.tag == "Player" || collision.gameObject.tag == "Block")
        {
            //�X�C�b�`�I��
            m_isTurnOn = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        //�G�ꂽ�̂��v���C���[�܂��̓u���b�N�̎�
        if (collision.gameObject.tag == "Player" || collision.gameObject.tag == "Block")
        {
            //�X�C�b�`�I�t
            m_isTurnOn = false;
        }
    }
}
