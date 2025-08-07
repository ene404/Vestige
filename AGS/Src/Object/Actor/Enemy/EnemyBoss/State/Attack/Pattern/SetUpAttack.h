#pragma once
#include "IEnemyAttackState.h"
class SetUpAttack 
    : public IEnemyAttackState
{

public:

    // チャージ時間
    static constexpr float CHARGE_DURATION = 1.5f;
    
    // バフが適用される時間
    static constexpr float BUFF_APPLY_TIME = 1.0f;

    // 攻撃の時間
    static constexpr float ATTACK_DURATION = 0.5f;
    
    // バフが適用されている時間
    static constexpr float BUFF_DURATION = 12.0f;
    
    // バフの倍率
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

    bool buffApplied_ = false;      //バフが適応されたかどうかのフラグ
    bool chargeFinished_ = false;   // チャージフェーズが狩猟したかどうかのフラグ

};

