using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------
//制作者　平島　迅
//----------------------------------------------

public class Handle : MonoBehaviour
{
    enum HandleDirection
    {
        right,
        left
    }
    [SerializeField] private HandleDirection m_direction;

    private Vector2 m_grabberPos;

    private Block m_parentBlockScript;      //親オブジェクト(Block)のスクリプト保存用

    private Transform m_grabberTransform;   //取っ手を掴んでいるオブジェクトのTransform

    [SerializeField] private GameObject m_backLightPrefab;

    [SerializeField] private Vector2 m_uiPosition;
    [SerializeField] private GameObject m_uiPrefab;
    private GameObject m_backLight;
    private GameObject m_ui;

    // Start is called before the first frame update
    void Start()
    {
        m_grabberPos = new Vector2(0.45f, 0.0f);
        if (m_direction == HandleDirection.right) m_grabberPos.x = -m_grabberPos.x;

        m_uiPosition = new Vector2(0.0f, 1.0f);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    //イベント関数

    //置くときに呼んでもらう関数
    public void OnPlaced()
    {
        //親オブジェクト(Block)に伝達
        m_parentBlockScript.OnPlaced();

        SetIsShine(false);
    }

    //掴むときに呼んでもらう関数
    public void OnGrabbed(Transform _transform)
    {
        if (_transform.GetComponent<PlayerBlockInteraction>().GetIsGrasping()) return;

        Vector3 grabberPosition = new Vector3(transform.position.x + m_grabberPos.x,
                                                transform.position.y + m_grabberPos.y, 0.0f);

        _transform.position = grabberPosition;

        //もし親オブジェクト(Block)のコンポーネントがnullなら取得
        if (m_parentBlockScript == null)
        {
            //親オブジェクトのスクリプトを取得
            m_parentBlockScript = GetComponentInParent<Block>();
        }

        //親オブジェクト(Block)に伝達
        m_parentBlockScript.OnGrabbed(_transform);
        m_parentBlockScript.SetGrabbedDirection((int)m_direction);

        SetIsShine(false);
    }

    //投げるときに呼んでもらう関数
    public void OnThrown()
    {
        m_parentBlockScript.OnThrown();

        SetIsShine(false);
    }

    public void SetIsShine(bool _isShine)
    {
        if (m_backLightPrefab == null) m_backLightPrefab = (GameObject)Resources.Load("BackLight");
        if (m_ui == null) m_uiPrefab = (GameObject)Resources.Load("Button_X");

        if(_isShine && m_backLight == null)
        {
            m_backLight = Instantiate(m_backLightPrefab, transform.position, Quaternion.identity);
            m_backLight.transform.parent = this.transform;
            m_ui = Instantiate(m_uiPrefab, transform.position + (Vector3)m_uiPosition, Quaternion.identity);
            m_ui.transform.parent = this.transform;
        }
        else if(!_isShine && m_backLight != null)
        {
            m_backLight.GetComponent<BackLight>().Erase();
            m_backLight = null;
            Destroy(m_ui);
            m_ui = null;
        }
    }

    //---------------------------
    //衝突関数
    //---------------------------

    private void OnTriggerEnter2D(Collider2D collision)
    {
        //親オブジェクトが無くてかつtag がBlockだったら
        if (this.transform.parent == null && collision.tag == "Block")
        {
            //親オブジェクトに登録
            this.transform.parent = collision.transform;
            //親オブジェクトのスクリプトを取得
            m_parentBlockScript = GetComponentInParent<Block>();
        }
    }
    
}
