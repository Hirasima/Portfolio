using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shutter : MonoBehaviour
{
    [SerializeField]
    GameObject[] m_button;      //ボタンを登録する配列
    [SerializeField]
    bool[] m_isPressedButton;   //ture,falseを保存する配列
    
    int m_PressedButtonCount;   //押されているボタンをカウントする

    // Start is called before the first frame update
    void Start()
    {
        m_PressedButtonCount = 0;
    }

    // Update is called once per frame
    void Update()
    {
        for (int i = 0; i < m_button.Length; ++i)
        {
            if (m_button[i].GetComponent<Button>().GetIsPressing())
            {
                m_isPressedButton[i] = true;
                IsCheck();
            }
        }

        if (m_button.Length == m_PressedButtonCount)
        {
            Destroy(gameObject);
        }
    }

    void IsCheck()
    {
        for (int i = 0; i < m_isPressedButton.Length; ++i)
        {
            if (m_isPressedButton[i])
            {
                ++m_PressedButtonCount;
            }
            else
            {
                m_PressedButtonCount = 0;
            }
        }
    }
}
