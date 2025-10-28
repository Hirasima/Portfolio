using UnityEngine;

//----------------------------------------------
//����ҁ@��z�h�g
//----------------------------------------------

public class Goal : MonoBehaviour
{
    [Header("�S�[������"), SerializeField]
    private bool m_goalPassibleFlag;    // �S�[���\���ǂ������肷��t���O

    // �e�R���|�[�l���g��ۑ�����ϐ�
    [SerializeField]
    private Key m_key;                  // �J�M
    private Animator m_anim;            // Animator

    // Start is called before the first frame update
    void Start()
    {
        // �ϐ�������������
        m_goalPassibleFlag = false;

        // �R���|�[�l���g���擾����
        m_anim = GetComponent<Animator>();
    }

    // �Q�[���}�l�[�W���[�ɃS�[���\�t���O��n�����߂̊֐�
    public bool GetGoal()
    {
        return m_goalPassibleFlag;
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        // �v���C���[�ƐڐG���Ă�����
        if (collision.gameObject.tag == "Player")
        {
            // �S�[���\�t���O���グ��
            m_goalPassibleFlag = true;

            // �J�M���������Ă���ꍇ
            if (m_key.GetKey())
            {
                // "Open"�A�j���[�V�������Đ�����
                m_anim.SetInteger("Goal_Anim", 1);
            }
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        // �v���C���[�Ɨ��ꂽ��
        if (collision.gameObject.tag == "Player")
        {
            // �S�[���\�t���O��������
            m_goalPassibleFlag = false;
        }
    }
}
