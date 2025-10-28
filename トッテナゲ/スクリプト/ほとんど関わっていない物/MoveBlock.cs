using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveBlock : MonoBehaviour
{
    [SerializeField] private float m_speed;
    [SerializeField] private Vector2 m_postionA;
    [SerializeField] private Vector2 m_postionB;

    enum Target
    {
        a,
        b,
    }
    Target m_target;

    private Vector2[] m_targetPos;

    private Rigidbody2D _rb;

    private GameObject m_player_ob;

    // Start is called before the first frame update
    void Start()
    {
        m_targetPos = new Vector2[2];
        m_targetPos[0] = m_postionA;
        m_targetPos[1] = m_postionB;

        m_target = Target.a;

        _rb = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        Move();
    }

    private void Move()
    {
        Vector2 dire = m_targetPos[(int)m_target] - (Vector2)transform.position;

        if(dire.magnitude < m_speed)
        {
            _rb.position += dire;

            if(m_player_ob != null)
            {
                m_player_ob.GetComponent<Rigidbody2D>().position += dire;
            }
        }
        else
        {
            _rb.position += dire.normalized * m_speed;

            if(m_player_ob != null)
            {
                m_player_ob.GetComponent<Rigidbody2D>().position += dire.normalized * m_speed;
            }
        }

        if(_rb.position == m_targetPos[(int)m_target])
        {
            ChangeTarget();
        }
    }

    private void ChangeTarget()
    {
        if (m_target == Target.a)
        {
            m_target = Target.b;
        }
        else
        {
            m_target = Target.a;
        }
    }

    public void SetSpeed(float _speed)
    {
        m_speed = _speed;
    }

    private void OnCollisionStay2D(Collision2D collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            m_player_ob = collision.gameObject;
        }
    }

    private void OnCollisionExit2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            m_player_ob = null;
        }
    }
}
