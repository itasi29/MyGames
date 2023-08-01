using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMoveLongDistance : MonoBehaviour
{
    // �G�̃X�e�[�^�X
    public int hp = 25;
    public int attack = 4;
    public float speedX = 0.05f;

    // �X�s�[�h�{��
    public int speedMultiplier = 1;

    // �����ʒu�ʒu
    float posX;
    float posY;

    // �~�܂�ʒu
    float stopPosX = 2.0f;

    // �U���Ԋu
    int waitFrameAttack;

    // �v���C���[�̏��
    PlayerControl player;

    void Start()
    {
        // ���x�̔{������
        speedX *= speedMultiplier;

        // �ʒu�̏�����
        posY = Random.Range(-1, 2) * 3.0f;
        posX = 9.5f;
        this.transform.position = new Vector2(posX, posY);

        // �U���Ԋu�̏�����
        waitFrameAttack = 0;
    }

    void FixedUpdate()
    {
        // hp���c���Ă��邩�̔���
        if (this.hp <= 0)
        {
            // �Q�[�W�̑���
            player.GaugeUp();
            // ���̃L����������
            Destroy(this.gameObject);
        }

        // �U���ʒu�ɂ��Ȃ��Ȃ�ړ�����
        if (this.transform.position.x > stopPosX)
        {
            this.gameObject.transform.Translate(speedX, 0.0f, 0.0f);
        }
        // �U���Ɉʒu�ɂ���Ȃ���Ԋu�ōU������
        else
        {
            // �ǂ���̃t���[���𑝂₵�Ă���������
            waitFrameAttack++;

            if (waitFrameAttack > 40)
            {
                // �v���C���[��HP�����炷
                player.HpDown(this.attack);

                // �ҋ@���Ԃ����߂ɖ߂�
                waitFrameAttack = 0;
            }
        }
    }

    // ���̓G��HP�����炷
    public void HpDown(int attack)
    {
        hp -= attack;
    }
}
