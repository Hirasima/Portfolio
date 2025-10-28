using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//--------------------------------------
//制作者：神谷優輝
//--------------------------------------

//用途としては当たり判定を行うことができない
//オブジェクトに対して、からのオブジェクトを子オブジェクトとして設定し、
//このスクリプトをアタッチして、接触したオブジェクトを取得する
public class ObjectJudge : MonoBehaviour
{
    private GameObject UnderObject;     //足元のオブジェクトを保存する

    private void OnTriggerStay2D(Collider2D collision)
    {
        UnderObject = collision.gameObject;
    }
    /// <summary> 
    ///足元のオブジェクトを返す
    /// </summary>
    /// <returns></returns>
    public GameObject GetUnderObject()
    {
        return UnderObject;
    }
}
