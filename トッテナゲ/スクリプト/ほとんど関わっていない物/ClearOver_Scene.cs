using UnityEngine;
using UnityEngine.SceneManagement;

//----------------------------------------------
//����ҁ@��z�h�g
//----------------------------------------------

public class ClearOver_Scene : MonoBehaviour
{
    private string m_sceneName;         // ���݂̃V�[���̖��O���擾����   
    private string m_gameSceneName;     // �Q�[���}�l�[�W���[����󂯂�������V�[�������擾����

    private void Start()
    {
        // �J�E���g�_�E���^�C�}�[������������
        CountDownTimer.IntializationTimer();
        
        // ���݂̃V�[���̖��O���i�[����
        m_sceneName = SceneManager.GetActiveScene().name;
        // ���O�̃X�e�[�W�V�[���̖��O���擾����
        m_gameSceneName = GameManager.GetPreserveScene();
    }

    // Update is called once per frame
    private void Update()
    {
        // �v���C���[
        // ���X�|�[���n�_���X�^�[�g�n�_�ɍX�V����
        PlayerCotroller.m_respawnPosition = PlayerCotroller.m_startPosition;
        // �`�F�b�N�|�C���g�t���O��������
        PlayerCotroller.m_checkPointFlag = false;


        // �����ꂩ�̃{�^���������ꂽ��V�[�����ڍs����
        if (!Input.anyKey) { return; }

        // ���݂̃V�[����"�Q�[���N���A"�̏ꍇ
        if (m_sceneName == "GameClear")
        {
            // ���O�̃V�[����"�X�e�[�W1"�̏ꍇ
            if (m_gameSceneName == "Stage1")
            {
                // "�X�e�[�W2"�Ɉڍs����
                SceneManager.LoadScene("Stage2");
            }
            // ���O�̃V�[����"�X�e�[�W1"�̏ꍇ
            if (m_gameSceneName == "Stage2")
            {
                // "�X�e�[�W3"�Ɉڍs����
                SceneManager.LoadScene("Stage3");
            }
        }

        // ���݂̃V�[����"�Q�[���I�[�o�["�̏ꍇ
        if (m_sceneName == "GameOver")
        {
            // "���O�̃Q�[���V�[��"�ɖ߂�
            SceneManager.LoadScene(m_gameSceneName);
        }
    }
}
