using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------
//����ҁ@�����@�v
//----------------------------------------------

public class Gate : MonoBehaviour
{
    //�Q�[�g���J���邽�߂̃X�C�b�`�̔z��
    [SerializeField] private GameObject[] m_switch_go;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //�����S�ẴX�C�b�`���I���Ȃ�
        if(CheckSwitchTurnOn())
        {
            //�J���邽�߂̃X�C�b�`�̐��J��Ԃ�
            for (int i = 0; i < m_switch_go.Length; ++i)
            {
                //�S�ẴX�C�b�`���A�N�e�B�u��
                m_switch_go[i].GetComponent<Switch>().SetSwitchStatus(false);
            }

            //�Q�[�g���J����
            OnOpen();
        }
    }

    //�Q�[�g���J����
    void OnOpen()
    {
        Destroy(gameObject);
    }

    //�X�C�b�`���I�����`�F�b�N
    bool CheckSwitchTurnOn()
    {
        bool result = false;    //���ʕۑ��p
        int count = 0;          //�J�E���^�[

        //�J���邽�߂̃X�C�b�`�̐��J��Ԃ�
        for (int i = 0; i < m_switch_go.Length; ++i)
        {
            //�����X�C�b�`�I���Ȃ�
            if(m_switch_go[i].GetComponent<Switch>().GetIsTurnOn())
            {
                //�J�E���^�[��+1
                ++count;
            }
        }

        //�S�ẴX�C�b�`���I���Ȃ猋�ʂ�true��
        if(count == m_switch_go.Length) result = true;

        return result;
    }
}
