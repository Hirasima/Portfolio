using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//----------------------------------------------
//制作者　平島　迅
//----------------------------------------------

public class Switch : MonoBehaviour
{
    //スイッチの状態
    enum SwitchStatus
    {
        sleep,
        wake,
    }
    SwitchStatus m_status;

    //スイッチがオンかどうか
    private bool m_isTurnOn;

    //スイッチオン時の画像
    [SerializeField] private Sprite m_onSprite;

    //スイッチオフ時の画像
    private Sprite m_offSprite;

    //スプライトレンダラー
    private SpriteRenderer _sr;

    // Start is called before the first frame update
    void Start()
    {
        m_isTurnOn = false;
        m_status = SwitchStatus.wake;
        _sr = this.GetComponent<SpriteRenderer>();
        m_offSprite = _sr.sprite;
    }

    // Update is called once per frame
    void Update()
    {
        //起動時なら
        if(m_status == SwitchStatus.wake)
        {
            if(m_isTurnOn)
            {
                //画像を切り替える
                _sr.sprite = m_onSprite;
            }
            else
            {
                //画像を切り替える
                _sr.sprite = m_offSprite;
            }
        }
        //非起動時なら
        else
        {
            //コリジョンを無効
            this.GetComponent<Collider2D>().enabled = false;
            //画像を切り替える
            _sr.sprite = m_onSprite;
            //画像を暗くする
            _sr.color = Color.gray;
        }
    }

    //スイッチの状態を設定
    public void SetSwitchStatus(bool _status)
    {
        if(_status)
        {
            m_status = SwitchStatus.wake;
        }
        else
        {
            m_status = SwitchStatus.sleep;
        }
    }

    //スイッチがオンならtrue、オフならfalseを取得
    public bool GetIsTurnOn()
    {
        return m_isTurnOn;
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        //触れたのがプレイヤーまたはブロックの時
        if(collision.gameObject.tag == "Player" || collision.gameObject.tag == "Block")
        {
            //スイッチオン
            m_isTurnOn = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        //触れたのがプレイヤーまたはブロックの時
        if (collision.gameObject.tag == "Player" || collision.gameObject.tag == "Block")
        {
            //スイッチオフ
            m_isTurnOn = false;
        }
    }
}
