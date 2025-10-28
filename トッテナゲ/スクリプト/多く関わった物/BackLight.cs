using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------
//êßçÏé“Å@ïΩìáÅ@êv
//----------------------------------------------

public class BackLight : MonoBehaviour
{
    private float m_time;

    private SpriteRenderer _sr;

    // Start is called before the first frame update
    void Start()
    {
        m_time = 0.0f;
        _sr = GetComponent<SpriteRenderer>();
    }

    // Update is called once per frame
    void Update()
    {
        m_time += Time.deltaTime;

        _sr.color = new Color( 255.0f, 255.0f, 0.0f, Mathf.Abs( Mathf.Sin(m_time * 2.0f) ) );
    }

    public void Erase()
    {
        Destroy(gameObject);
    }
}
