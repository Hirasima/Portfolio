using UnityEngine;
using UnityEngine.SceneManagement;

//----------------------------------------------
//����ҁ@��z�h�g
//----------------------------------------------

public class GameManager : MonoBehaviour
{
    public static string m_sceneName;   // �V�[���̖��O��ۑ�����
    [SerializeField]
    private bool m_operationFlag;       // ������� ON OFF �̃t���O 
    private bool m_fallheckFlag;        // ��ʊO�ɗ����������肷��

    // �I�u�W�F�N�g���i�[���邽�߂̕ϐ�
    [SerializeField]
    private Key m_key;                  // �J�M
    [SerializeField]
    private Goal m_goal;                // �S�[��
    [SerializeField]
    private CountDownTimer m_downTimer; // ��������
    [SerializeField]
    private GameObject m_operation;     // �������
    private GamepadStick m_gamepadStick;// �R���g���[���[


    // Start is called before the first frame update
    private void Start()
    {
        // �t���O��������
        m_fallheckFlag = false;
        m_operationFlag = false;
        // �I�u�W�F�N�g���i�[����

        m_gamepadStick = this.gameObject.GetComponent<GamepadStick>();

        // �V�[���̖��O��ۑ�����
        PreserveSceneName();
    }

    // Update is called once per frame
    private void Update()
    {
        // �V�[���ēǂݍ���
        RoadThisScene();
        // �Q�[���I�[�o�[�V�[�����ĂԂ���
        RoadGameOver();
        // �Q�[���N���A�V�[�����ĂԂ���
        RoadGameClear();

        // ���������\������
        DiplayOperation();
    }

    // �V�[�����Z�b�g
    private void RoadThisScene()
    {
        // R�L�[���������܂��́A��ʊO�ɗ���������
        if (Input.GetKeyDown(KeyCode.R) ||
            m_fallheckFlag ||
            Input.GetKeyDown("joystick button 4"))
        {
            // ���O�̃V�[�����ēǂݍ��݂���
            SceneManager.LoadScene(m_sceneName);
        }
    }

    // �Q�[���I�[�o�[�V�[����ǂݍ���
    private void RoadGameOver()
    {
        //�J�E���g���O�ɂȂ�����
        if (m_downTimer.GetTimer() <= 0.0f)
        {
            // �Q�[���I�[�o�[�V�[���Ɉڍs����
            SceneManager.LoadScene("GameOver");
        }
    }

    // �Q�[���N���A�V�[����ǂݍ���
    private void RoadGameClear()
    {
        // �ǂ��炩�̃t���O���������Ă�������s���Ȃ�
        if (!(m_key.GetKey() && m_goal.GetGoal())) { return; }

        // �X�e�B�b�N��܂��̓A���[�L�[�オ�����ꂽ��X�e�[�W�N���A
        if ((m_gamepadStick.GetGamepadValue() == Vector2.up) ||
           ((m_gamepadStick.GetGamepadValue().y > 0.8f) && 
           ((m_gamepadStick.GetGamepadValue().x < 0.6f) || (m_gamepadStick.GetGamepadValue().x > -0.6f)))||
            (Input.GetKeyDown(KeyCode.UpArrow)))
        {
            // �N���A�X�e�[�W���ŏI�Ȃ�
            if (m_sceneName == "Stage3")
            {
                // �I�[���N���A�V�[���Ɉڍs����
                SceneManager.LoadScene("AllClear");
            }
            else
            {
                // �Q�[���N���A�V�[���Ɉڍs����
                SceneManager.LoadScene("GameClear");
            }             
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �v���C���[�ƐڐG������
        if (collision.gameObject.tag == "Player")
        {
            // �����t���O���グ��
            m_fallheckFlag = true;
        }
    }

    // ���݂̃V�[������ۑ�����
    public static void PreserveSceneName()
    {
        m_sceneName = SceneManager.GetActiveScene().name;
    }
    // ���݂̃V�[������Ԃ�
    // �Q�[���N���A�E�Q�[���I�[�o�[�V�[���Ŏ󂯎��
    public static string GetPreserveScene()
    {
        return m_sceneName;
    }

    // ���������\������
    private void DiplayOperation()
    {
        // "RB�{�^���������ꂽ" ����"��������\����OFF" �̏ꍇ
        if (Input.GetKeyDown("joystick button 5") && !m_operationFlag)
        {
            // �t���O���グ��
            m_operationFlag = true;
            // �A�N�e�B�u��Ԃɂ���
            m_operation.SetActive(true);
        }
        // "RB�{�^���������ꂽ" ����"��������\����ON" �̏ꍇ
        else if (Input.GetKeyDown("joystick button 5") && m_operationFlag)
        {
            // �t���O��������
            m_operationFlag = false;
            // ��A�N�e�B�u��Ԃɂ���
            m_operation.SetActive(false);
        }
    }

    // �v���C���[�R���g���[���[�ɑ������ ON OFF �̃t���O��n�����߂̊֐�
    public bool GetOperation()
    {
        return m_operationFlag;
    }
}
