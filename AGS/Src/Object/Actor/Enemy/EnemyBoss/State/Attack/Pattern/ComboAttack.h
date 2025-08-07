#pragma once
#include "IEnemyAttackState.h"

class ComboAttack 
    : public IEnemyAttackState
{

public:

    // �U�����莞��
    static constexpr float ATTACK_DIRECTION_TIME = 0.73f;
    
    // �_���[�W�l
    static constexpr int DAMEGE_VALUE = 20;
    
    // ����
    static constexpr int SOUND_VALUE = 255;


    ComboAttack(EnemyBoss& boss);
    ~ComboAttack(void);

    void Enter(EnemyBoss& boss)override;
    void Update(EnemyBoss& boss)override;
    void Exit(EnemyBoss& boss)override;
    bool IsFinished()const override;
    void StopEffect(void) override;

private:
    EnemyBoss& boss_;
    
    // �T�E���h
    int noHitSound_;
    bool isSoundPlayed_;
    void InitSound(void);
};

