using UnityEngine;

//----------------------------------------------
//����ҁ@��z�h�g
//----------------------------------------------

public class Key : MonoBehaviour
{
    [Header("�J�M�̏������"), SerializeField]
    private bool m_keyActiveFlag;       // �J�M���v���C���[�Ɏ擾���ꂽ�����肷��t���O
    private Vector3 m_stopPosition;     // �J�M�̒�~�ʒu
    [Header("�ǔ����x"), SerializeField]
    private float m_trackingSpeed;          // �ǔ����x
    private Transform m_playerTransform;    // �v���C���[�̈ʒu�����擾����

    //�e�R���|�[�l���g��ۑ�����ϐ�
    private CapsuleCollider2D m_col;

    // Start is called before the first frame update
    private void Start()
    {
        // �ϐ�������������
        m_keyActiveFlag = false;                            // �J�M�擾�t���O
        m_stopPosition = new Vector3(0.5f, 0.0f, 0.0f);     // �J�M�̒�~�ʒu

        //�K�v�ȃR���|�[�l���g���擾����
        m_col = gameObject.GetComponent<CapsuleCollider2D>();
    }

    // Update is called once per frame
    private void Update()
    {
        // �L�[�A�N�e�B�u�t���O���������Ă���ꍇ�͎��s���Ȃ�
        if (!m_keyActiveFlag) { return; }
       
        // �v���C���[���E�������Ă��鎞
        if (m_playerTransform.rotation == Quaternion.Euler(0.0f, 0.0f, 0.0f))
        {
            // �v���C���[��ǔ����A"����"�ɒ�~����
            transform.position = Vector3.Lerp(transform.position,
                m_playerTransform.transform.position - m_stopPosition, m_trackingSpeed * Time.deltaTime);
        }
        // �v���C���[�����������Ă��鎞
        else
        {
            // �v���C���[��ǔ����A"�E��"�ɒ�~����
            transform.position = Vector3.Lerp(transform.position,
                m_playerTransform.transform.position + m_stopPosition, m_trackingSpeed * Time.deltaTime);
        }
    }

    // �Q�[���}�l�[�W���[�ɃJ�M�擾�t���O��n�����߂̊֐�
    public bool GetKey()
    {
        return m_keyActiveFlag;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �v���C���[�ƐڐG������
        if (collision.gameObject.tag == "Player")
        {
            // ���������v���C���[�̈ʒu�����i�[����
            m_playerTransform = collision.transform;
            // �����蔻����Ȃ���
            m_col.enabled = false;
            // �L�[�A�N�e�B�u�t���O���グ��
            m_keyActiveFlag = true;
        }
    }
}
