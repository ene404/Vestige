#include "EChaseState.h"
#include "../../../../../../Manager/SceneManager.h"
#include "../../EnemyBoss.h"
#include "../../Animation/EnemyBossAnimation.h"
#include "../EnemyBossCombatController.h"
#include "../EnemyBossStateMachine.h"
#include "../../../../Player/Player.h"

EChaseState::EChaseState(void)
{
	dist_ = 0.0f;
}

EChaseState::~EChaseState(void)
{
}

void EChaseState::Enter(EnemyBoss& boss)
{
	boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::RUN, true, EnemyBoss::ANIM_BLEND_RATE);
}

void EChaseState::Update(EnemyBoss& boss)
{
	// �v���C���[�ւ̕����i�����ʂ̂݁j
	dist_ = boss.GetPlayerDistanceXZ();
	if (dist_ < ATTACK_RANGE)
	{
		if (boss.GetStepCooldown() <= 0.0f)
		{
			boss.SetStepCooldown(EnemyBoss::STEP_COOLDOWN_DURATON);
			boss.ChangeState(EnemyBoss::STATE::STEP);
			return;
		}
	}

	// �v���C���[���U���J�n�͈͓��ɓ�������A�U���\���`�F�b�N
	if (dist_ < ATTACK_INITIATE_RANGE)
	{
		// ���̍U������ CombatController ����擾
		EnemyBoss::ATK_STATE nextAttack = boss.GetAttackStateController()->SelectAttackCandidate();
		if (nextAttack != EnemyBoss::ATK_STATE::NONE)
		{
			// EAttackState�Ŏg������,���̍U���� EnemyBoss ���L������
			boss.SetNextAttackType(nextAttack);
			boss.ChangeState(EnemyBoss::STATE::ATTACK);
			return;
		}
	}

	// ���K�������x�N�g��
	VECTOR dir = VNorm(boss.GetToPlayer());

	// �������N�H�[�^�j�I���ŕ��
	Quaternion targetRot = Quaternion::LookRotation(dir);
	boss.GetTransformMutable().quaRot = Quaternion::Slerp(boss.GetTransform().quaRot, targetRot, 0.1f);

	// �O�i�ړ�
	boss.GetTransformMutable().pos = VAdd(boss.GetTransform().pos, VScale(dir, CHASE_SPEED));
}

void EChaseState::Exit(EnemyBoss& boss)
{
}
