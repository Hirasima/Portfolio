using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------
//制作者　平島　迅
//----------------------------------------------

public class Gate : MonoBehaviour
{
    //ゲートを開けるためのスイッチの配列
    [SerializeField] private GameObject[] m_switch_go;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //もし全てのスイッチがオンなら
        if(CheckSwitchTurnOn())
        {
            //開けるためのスイッチの数繰り返し
            for (int i = 0; i < m_switch_go.Length; ++i)
            {
                //全てのスイッチを非アクティブに
                m_switch_go[i].GetComponent<Switch>().SetSwitchStatus(false);
            }

            //ゲートを開ける
            OnOpen();
        }
    }

    //ゲートを開ける
    void OnOpen()
    {
        Destroy(gameObject);
    }

    //スイッチがオンかチェック
    bool CheckSwitchTurnOn()
    {
        bool result = false;    //結果保存用
        int count = 0;          //カウンター

        //開けるためのスイッチの数繰り返し
        for (int i = 0; i < m_switch_go.Length; ++i)
        {
            //もしスイッチオンなら
            if(m_switch_go[i].GetComponent<Switch>().GetIsTurnOn())
            {
                //カウンターに+1
                ++count;
            }
        }

        //全てのスイッチがオンなら結果をtrueに
        if(count == m_switch_go.Length) result = true;

        return result;
    }
}
