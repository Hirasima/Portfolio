using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//--------------------------------------
//����ҁF�_�J�D�P
//--------------------------------------

//�p�r�Ƃ��Ă͓����蔻����s�����Ƃ��ł��Ȃ�
//�I�u�W�F�N�g�ɑ΂��āA����̃I�u�W�F�N�g���q�I�u�W�F�N�g�Ƃ��Đݒ肵�A
//���̃X�N���v�g���A�^�b�`���āA�ڐG�����I�u�W�F�N�g���擾����
public class ObjectJudge : MonoBehaviour
{
    private GameObject UnderObject;     //�����̃I�u�W�F�N�g��ۑ�����

    private void OnTriggerStay2D(Collider2D collision)
    {
        UnderObject = collision.gameObject;
    }
    /// <summary> 
    ///�����̃I�u�W�F�N�g��Ԃ�
    /// </summary>
    /// <returns></returns>
    public GameObject GetUnderObject()
    {
        return UnderObject;
    }
}
