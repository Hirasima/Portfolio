using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Button : MonoBehaviour
{
    private bool m_IsPressed;   //�{�^����������Ă��邩�ǂ���

    // Start is called before the first frame update
    void Start()
    {
        //�ϐ��̏�����
        m_IsPressed = false;
    }

    //�������Ă��鎞��������
    private void OnTriggerStay2D(Collider2D collision)
    {
        //�v���C���[���u���b�N���G��Ă��鎞��������
        if (collision.tag == "Player" || collision.tag == "Block")
        {
            m_IsPressed = true;
        }
        
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.tag == "Player" || collision.tag == "Block")
        {
            m_IsPressed = false;
        }
    }

    /// <summary>
    /// �{�^����������Ă��邩�ǂ�����Ԃ�
    /// </summary>
    /// <returns></returns>
    public bool GetIsPressing()
    {
        return m_IsPressed;
    }
}