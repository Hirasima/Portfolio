using UnityEngine;
using UnityEngine.InputSystem;

//-----------------------------------------
//制作者：神谷優輝
//-----------------------------------------
public class GamepadStick : MonoBehaviour
{
    private Vector2 m_leftStickValue;
    private Gamepad m_gamepad;

    private void Start()
    {
        // ゲームパッド（デバイス取得）
        m_gamepad = Gamepad.current;
    }

    private void Update()
    {
        if (m_gamepad == null) { return; }

        // ゲームパッドの左のスティックの入力値を取得
        // Vector2型を返す
        m_leftStickValue = m_gamepad.leftStick.ReadValue();
    }

    /// <summary>
    ///左スティックの入力の値を返す 
    /// </summary>
    public Vector2 GetGamepadValue()
    {
        return m_leftStickValue;
    }
}