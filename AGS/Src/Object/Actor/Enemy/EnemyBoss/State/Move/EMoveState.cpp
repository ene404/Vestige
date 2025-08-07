#include "EMoveState.h"
#include "../../../../../../Manager/SceneManager.h"
#include "../../EnemyBoss.h"
#include "../../Animation/EnemyBossAnimation.h"
#include "../EnemyBossStateMachine.h"
#include "../EnemyBossCombatController.h"
#include "../../../../Player/Player.h"

EMoveState::EMoveState(void)
{
}

EMoveState::~EMoveState(void)
{
}

void EMoveState::Enter(EnemyBoss& boss)
{
    boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::WALK_F, true, EnemyBoss::ANIM_BLEND_RATE);
}

void EMoveState::Update(EnemyBoss& boss)
{
    // �{�X���X�^�����̏ꍇ�͉������Ȃ�
    if (boss.IsStunned()) 
    {
        return;
    }

    float dist = boss.GetPlayerDistanceXZ();

    // �S�Ă̍U���̃N�[���_�E�����񕜂������A�܂��͍U���ł�����̂���ł��Ȃ����`�F�b�N
    if (!boss.GetAttackStateController()->AreAllAttacksOnCooldown()) 
    {
        boss.ChangeState(EnemyBoss::STATE::CHASE);
        return;
    }

    // �v���C���[�Ƃ̋��������ɕۂ��߂̈ړ����W�b�N
    if (dist < TARGET_DISTANCE - DISTANCE_TOLERANCE) 
    {
        // �߂�������v���C���[���痣���
        boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::WALK_B, true);

        // �v���C���[���痣������
        VECTOR dirAway = VNorm(VSub(boss.GetTransform().pos, boss.GetPlayer().GetTransform().pos));
        boss.SetPosition(VAdd(boss.GetTransform().pos, VScale(dirAway, MOVE_SPEED)));
    }
    else if (dist > TARGET_DISTANCE + DISTANCE_TOLERANCE) 
    {
        // ����������v���C���[�ɋ߂Â�
        boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::WALK_F, true);
        
        // �v���C���[�Ɍ���������
        VECTOR dirToward = VNorm(VSub(boss.GetPlayer().GetTransform().pos, boss.GetTransform().pos));
        boss.SetPosition(VAdd(boss.GetTransform().pos, VScale(dirToward, MOVE_SPEED)));
    }
    else 
    {
        // ���傤�ǂ悢�����Ȃ��~
        boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::IDLE, true);
    }
}

void EMoveState::Exit(EnemyBoss& boss)
{
}
