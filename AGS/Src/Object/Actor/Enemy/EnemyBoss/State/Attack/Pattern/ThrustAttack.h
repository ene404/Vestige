#pragma once
#include <DxLib.h>
#include "IEnemyAttackState.h"
class ThrustAttack :
    public IEnemyAttackState
{

public:

    // �ːi�p������
    static constexpr float DURATION = 1.0f;

    // �ːi���鋗��
    static constexpr float DISTANCE = 250.0f;
    
    // �����x
    static constexpr float INITIAL_SPEED = 5.0f;
    
    // �ō����x
    static constexpr float PEAK_SPEED = 20.0f;
    
    // �����J�n�܂ł̎��Ԋ���
    static constexpr float DECELERATION_RATE = 0.5f;
    
    // �U���Ώۍ��W�Ǝ��g�̍��W�Ƃ̋����̐���
    static constexpr float DIST_LIMIT = 500.0f;

    // �^�[�Q�b�g���W�̐���
    static constexpr float TARGETPOS_LIMIT = 200.0f;

    // �U�����Ԃ̐���
    static constexpr float ATTACK_TIME_LIMIT = 0.95f;

    // �_���[�W�l
    static constexpr int DAMAGE_VALUE = 20;

    // ����
    static constexpr int SOUND_VALUE = 255;

    ThrustAttack(EnemyBoss& boss);
    ~ThrustAttack(void);

    void Enter(EnemyBoss& boss)override;
    void Update(EnemyBoss& boss)override;
    void Exit(EnemyBoss& boss)override;
    bool IsFinished()const override;
    void StopEffect(void) override;

private:

    EnemyBoss& boss_;
    
    VECTOR initialPos_;         // �ːi�J�n�ʒu
    VECTOR chargeDirection_;    // �ːi����

    // ���x�J�[�u�̌v�Z�p
    float GetCurrentSpeed(float t);

    // �T�E���h
    int noHitSound_;
    bool isSoundPlayed_;
    void InitSound(void);
};

