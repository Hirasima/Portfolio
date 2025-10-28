using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//制作者　平島　迅
//----------------------------------------------

public class PlayerBlockInteraction : MonoBehaviour
{
    [SerializeField] private bool m_bGrasping;   //掴んでいるか
    [SerializeField] private bool m_bCanGrab;    //掴めるか

    //コンポーネント保存用
    [SerializeField] private Collider2D m_canGrabHandle;     //現在掴める取っ手を保存

    // Start is called before the first frame update
    void Start()
    {
        //変数の初期化
        m_bGrasping = false;
        m_bCanGrab = false;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //置くときに呼んでもらう関数
    public void PlaceHandle()
    {
        //掴んでいないときは処理を抜ける
        if(m_bGrasping == false) { return; }

        //掴んでいる取っ手のスクリプトを取得して取っ手に「置く」ことを伝える
        Handle handleScript = m_canGrabHandle.gameObject.GetComponent<Handle>();
        handleScript.OnPlaced();

        m_canGrabHandle = null;

        //「掴んでいない」状態にする
        m_bGrasping = false;
    }

    //掴むときに呼んでもらう関数
    public void GrabHandle()
    {
        //掴む取っ手が無い場合処理抜ける
        if(m_bCanGrab == false || m_bGrasping == true || m_canGrabHandle == null) { return; }

        ////掴んでいる取っ手のスクリプトを取得して取っ手に「掴む」ことを伝える
        Handle handleScript = m_canGrabHandle.gameObject.GetComponent<Handle>();
        handleScript.OnGrabbed(this.transform);

        //「掴んでいる」状態にする
        m_bGrasping = true;
        m_bCanGrab = false;
    }

    //投げるときに呼んでもらう関数
    public void ThrowHandle()
    {
        //掴んでいないときは処理を抜ける
        if (m_bGrasping == false) { return; }

        //掴んでいる取っ手のスクリプトを取得して取っ手に「投げる」ことを伝える
        Handle handleScript = m_canGrabHandle.gameObject.GetComponent<Handle>();
        handleScript.OnThrown();

        m_canGrabHandle = null;

        //「掴んでいない」状態にする
        m_bGrasping = false;
    }

    //-----------------------------------
    //ゲッター(Get関数)
    //-----------------------------------

    //現在掴んでいるかを返す関数
    public bool GetIsGrasping()
    {
        return m_bGrasping;
    }

    //現在掴めるかを返す関数
    public bool GetCanGrab()
    {
        return m_bCanGrab;
    }

    //-----------------------------------
    //衝突関数
    //-----------------------------------

    //触れたときによばれる関数
    private void OnTriggerStay2D(Collider2D _collision)
    {
        if(_collision.gameObject.tag == "Handle" && m_canGrabHandle == null)
        {
            m_bCanGrab = true;
            m_canGrabHandle = _collision;
            _collision.GetComponent<Handle>().SetIsShine(true);
        }
    }

    //触れなくなったときによばれる関数
    private void OnTriggerExit2D(Collider2D _collision)
    {
        if (_collision.gameObject.tag == "Handle" && m_bGrasping == false)
        {
            _collision.GetComponent<Handle>().SetIsShine(false);
            m_bCanGrab = false;
            m_canGrabHandle = null;
        }
    }
}
