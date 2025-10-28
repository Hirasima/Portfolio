using UnityEngine;
using UnityEngine.SceneManagement;

//----------------------------------------------
//����ҁ@��z�h�g
//----------------------------------------------

public class TitleAllClear_Scene : MonoBehaviour
{
    private string m_sceneName;     // ���݂̃V�[���̖��O���擾����

    private void Start()
    {
        // ���݂̃V�[���̖��O���i�[����
        m_sceneName = SceneManager.GetActiveScene().name;

        // �J�E���g�_�E���^�C�}�[������������
        CountDownTimer.IntializationTimer();
    }

    // Update is called once per frame
    private void Update()
    {
        // �v���C���[
        // ���X�|�[���n�_���X�^�[�g�n�_�ɍX�V����
        PlayerCotroller.m_respawnPosition = PlayerCotroller.m_startPosition;
        // �`�F�b�N�|�C���g�t���O��������
        PlayerCotroller.m_checkPointFlag = false;

        // �����ꂩ�̃{�^���������ꂽ��
        if (!Input.anyKey) { return; }
            
        // ���݂̃V�[�����^�C�g���Ȃ�
        if (m_sceneName == "Title")
        {
            // "�X�e�[�W1"�Ɉڍs����
            SceneManager.LoadScene("Stage1");
        }
        if (m_sceneName == "AllClear")
        {
            // "�X�e�[�W1"�Ɉڍs����
            SceneManager.LoadScene("Title");
        }
    }
}
