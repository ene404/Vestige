#include "EAttackState.h"
#include "../../EnemyBoss.h"
#include "../EnemyBossStateMachine.h"
#include "../EnemyBossCombatController.h"
#include "EnemyBossAttackStateMachine.h"
#include "../Attack/Pattern/IEnemyAttackState.h"
#include "../Attack/Pattern/ComboAttack.h"
#include "../Attack/Pattern/ThrustAttack.h"
#include "../Attack/Pattern/RoarAttack.h"
#include "../Attack/Pattern/ExplosionAttack.h"
#include "../Attack/Pattern/SetUpAttack.h"

EAttackState::EAttackState(void)
{
	
}

EAttackState::~EAttackState(void)
{
}

void EAttackState::Enter(EnemyBoss& boss)
{
    if (!isContinuous_)
    {
        boss.GetAttackStateController()->GenerateNewAttackSeries(boss.GetPlayerDistanceXZ());
        isContinuous_ = true;
    }

    // ���̍U���� EnemyBoss ���L�����Ă���̂�
    EnemyBoss::ATK_STATE attackToExecute = boss.GetAttackStateController()->GetNextAttackInSeries();

    if (attackToExecute != EnemyBoss::ATK_STATE::NONE)
    {
        boss.GetAttackStateMachine()->Change(boss, attackToExecute);
    }
    else {
        boss.ChangeState(EnemyBoss::STATE::CHASE);
    }
}

void EAttackState::Update(EnemyBoss& boss)
{
    if (boss.IsStunned())
    {
        return;
    }

    // �T�u�X�e�[�g�}�V���̍X�V
    if (boss.GetAttackStateMachine()) 
    {
        boss.GetAttackStateMachine()->Update(boss);
    }
    else
    {
        boss.ChangeState(EnemyBoss::STATE::CHASE);
        return;
    }

    // ���݂̍U�����I��������A���̍s��������
    if (boss.GetAttackStateMachine()->IsFinished())
    {
        // �U�����I�������̂ŃN�[���_�E����ݒ�
        boss.GetAttackStateController()->SetAttackCooldown(boss.GetAttackStateMachine()->GetCurrentStateType());

        // �A���U���̃C���f�b�N�X��i�߂�
        boss.GetAttackStateController()->AdvanceContinuousAttackIndex();

        // �A���U���̑��񐔂��m�F
        if (boss.GetAttackStateController()->GetCurrentContinuousAttackCount() > boss.GetAttackStateController()->GetTotalContinuousAttackCount())
        {
            // �S�Ă̘A���U�������������ꍇ
            // �A���U���I��
            isContinuous_ = false; 

            // ������ۂX�e�[�g��
            boss.ChangeState(EnemyBoss::STATE::MOVE);
            return;
        }

        // ���̍U���� CombatController �ɖ₢���킹��
        EnemyBoss::ATK_STATE nextAttackInSeries = boss.GetAttackStateController()->SelectAttackCandidate();

        if (nextAttackInSeries != EnemyBoss::ATK_STATE::NONE) 
        {
            // ���̍U��������Ύ��s
            boss.GetAttackStateMachine()->Change(boss, nextAttackInSeries);
            boss.GetAttackStateController()->SetAttackCooldown(nextAttackInSeries);
        }
        else {
            // �A���U���V���[�Y�̓r���ŁA���̍U�����N�[���_�E�����ȂǂőI�ׂȂ������ꍇ
            // �A���U���I��
            isContinuous_ = false; 
            // Chase�ɖ߂�
            boss.ChangeState(EnemyBoss::STATE::CHASE);
        }
    }
}

void EAttackState::Exit(EnemyBoss& boss)
{
    // �U���I�����ɃT�u�X�e�[�g�}�V�������Z�b�g
    if (boss.GetAttackStateMachine()) 
    {
        boss.GetAttackStateMachine()->Reset(boss);
    }

    // �A���U���V���[�Y�̏�Ԃ����Z�b�g
    if (boss.GetAttackStateController()) 
    {
        boss.GetAttackStateController()->ResetContinuousAttackSeries();
    }

    // ���̍U�������Z�b�g
    boss.SetNextAttackType(EnemyBoss::ATK_STATE::NONE);
    isContinuous_ = false;
 }