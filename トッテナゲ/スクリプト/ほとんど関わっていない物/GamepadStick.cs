using UnityEngine;
using UnityEngine.InputSystem;

//-----------------------------------------
//����ҁF�_�J�D�P
//-----------------------------------------
public class GamepadStick : MonoBehaviour
{
    private Vector2 m_leftStickValue;
    private Gamepad m_gamepad;

    private void Start()
    {
        // �Q�[���p�b�h�i�f�o�C�X�擾�j
        m_gamepad = Gamepad.current;
    }

    private void Update()
    {
        if (m_gamepad == null) { return; }

        // �Q�[���p�b�h�̍��̃X�e�B�b�N�̓��͒l���擾
        // Vector2�^��Ԃ�
        m_leftStickValue = m_gamepad.leftStick.ReadValue();
    }

    /// <summary>
    ///���X�e�B�b�N�̓��͂̒l��Ԃ� 
    /// </summary>
    public Vector2 GetGamepadValue()
    {
        return m_leftStickValue;
    }
}