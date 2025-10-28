using UnityEngine;
using UnityEngine.UI;
//--------------------------------
//10/28 �_�J
//TextMeshPor���g�����߂�using
using TMPro;
//--------------------------------

//----------------------------------------------
//����ҁ@��z�h�g
//----------------------------------------------

public class CountDownTimer : MonoBehaviour
{
    public static float m_countDownTimer = 200.0f;     // ��������

    // �I�u�W�F�N�g���i�[���邽�߂̕ϐ�
    private Text m_text;

    //---------------------------------------------------------------
    //10/28�@�_�J
    //TextMeshPro�ɑΉ�����悤�ɐV�����ϐ��Ə�����ǉ����܂���
    private TextMeshProUGUI m_tectMesh;
    //---------------------------------------------------------------

    // Start is called before the first frame update
    void Start()
    {   
        // �I�u�W�F�N�g���i�[����
        m_text = GetComponent<Text>();

        //-----------------------------------------
        //10/28 �_�J
        //�A�^�b�`����Ă���TextMesh���擾����
        m_tectMesh = this.GetComponent<TextMeshProUGUI>();
        //-----------------------------------------
    }

    // Update is called once per frame
    private void Update()
    {
        // �J�E���g�_�E�����J�n
        m_countDownTimer -= Time.deltaTime;

        //---------------------------------------------------------
        //10/28 �_�J
        // ���S���̊m��
        if (m_text && !m_tectMesh)
        {
            // �����ɒ����ĕ\������
            m_text.text = Mathf.CeilToInt(m_countDownTimer).ToString();
        }
        //---------------------------------------------------------


        //---------------------------------------------------------
        //10/28 �_�J�@
        //TextMeshPro���A�^�b�`����Ă��邩��
        if (!m_tectMesh) { return; }

        //Text���A�^�b�`����Ă��Ȃ��Ƃ�����
        if (!m_text)
        {
            m_tectMesh.text = Mathf.CeilToInt(m_countDownTimer).ToString();
        }

        //---------------------------------------------------------
    }

    // �Q�[���}�l�[�W���[�Ƀ^�C�}�[�̒l��n�����߂̊֐�
    public float GetTimer()
    {
        return m_countDownTimer;
    }

    // �^�C�g���E�Q�[���N���A�E�Q�[���I�[�o�[�V�[���Ń^�C�}�[������������
    public static void IntializationTimer()
    {
        m_countDownTimer = 200.0f;
    }
}
