#pragma once
#include<map>
#include<functional>
#include "../EnemyBoss.h"
#include "../State/Attack/Pattern/IEnemyAttackState.h"

class AttackPatternStateMachine
{

public:

    explicit AttackPatternStateMachine(EnemyBoss& boss);

    void Update(EnemyBoss& boss);
    void SetUpdateFunction(EnemyBoss::ATK_STATE aStateId, std::shared_ptr<IEnemyAttackState> stateInstance);
    void ChangeState(EnemyBoss::ATK_STATE newState);

    EnemyBoss::ATK_STATE GetCurrentAStateId() const;

    void DrawDebugInfo(const VECTOR& screenPos);

private:
    EnemyBoss& boss_;
    std::map<EnemyBoss::ATK_STATE, std::shared_ptr<IEnemyAttackState>> states_;
    std::shared_ptr<IEnemyAttackState> curAState_;
    EnemyBoss::ATK_STATE curAStateId_;
};

