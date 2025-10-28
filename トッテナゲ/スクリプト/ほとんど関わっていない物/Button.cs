using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Button : MonoBehaviour
{
    private bool m_IsPressed;   //ボタンが押されているかどうか

    // Start is called before the first frame update
    void Start()
    {
        //変数の初期化
        m_IsPressed = false;
    }

    //当たっている時だけ処理
    private void OnTriggerStay2D(Collider2D collision)
    {
        //プレイヤーかブロックが触れている時だけ処理
        if (collision.tag == "Player" || collision.tag == "Block")
        {
            m_IsPressed = true;
        }
        
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.tag == "Player" || collision.tag == "Block")
        {
            m_IsPressed = false;
        }
    }

    /// <summary>
    /// ボタンが押されているかどうかを返す
    /// </summary>
    /// <returns></returns>
    public bool GetIsPressing()
    {
        return m_IsPressed;
    }
}