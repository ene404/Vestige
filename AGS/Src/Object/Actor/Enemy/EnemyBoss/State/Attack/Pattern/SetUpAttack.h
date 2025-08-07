#pragma once
#include "IEnemyAttackState.h"
class SetUpAttack 
    : public IEnemyAttackState
{

public:

    // �`���[�W����
    static constexpr float CHARGE_DURATION = 1.5f;
    
    // �o�t���K�p����鎞��
    static constexpr float BUFF_APPLY_TIME = 1.0f;

    // �U���̎���
    static constexpr float ATTACK_DURATION = 0.5f;
    
    // �o�t���K�p����Ă��鎞��
    static constexpr float BUFF_DURATION = 12.0f;
    
    // �o�t�̔{��
    static constexpr float ATTACK_MULTIPLIER = 1.5f;

    SetUpAttack(EnemyBoss& boss);
    ~SetUpAttack(void);

    void Enter(EnemyBoss& boss)override;
    void Update(EnemyBoss& boss)override;
    void Exit(EnemyBoss& boss)override;
    bool IsFinished()const override;
    void StopEffect(void) override;

private:
    EnemyBoss& boss_;

    bool buffApplied_ = false;      //�o�t���K�����ꂽ���ǂ����̃t���O
    bool chargeFinished_ = false;   // �`���[�W�t�F�[�Y����������ǂ����̃t���O

};

