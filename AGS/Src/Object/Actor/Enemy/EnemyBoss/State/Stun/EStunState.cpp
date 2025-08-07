#include "EStunState.h"
#include "../../../../../../Manager/SceneManager.h"
#include "../../EnemyBoss.h"
#include "../../Animation/EnemyBossAnimation.h"
#include "../EnemyBossStateMachine.h"

EStunState::EStunState(void)
{
}

EStunState::~EStunState(void)
{
}

void EStunState::Enter(EnemyBoss& boss)
{
	boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::STUN_IDLE, true);
}

void EStunState::Update(EnemyBoss& boss)
{
    // �X�^���^�C�}�[��0�ɂȂ�܂ŉ������Ȃ��i�{�X���g���^�C�}�[���X�V�j
    if (!boss.IsStunned()) 
    {
        // �X�^�����������ꂽ��CHASE�ɖ߂�
        boss.ChangeState(EnemyBoss::STATE::MOVE);
    }
}

void EStunState::Exit(EnemyBoss& boss)
{
    boss.SetStunned(0.0f);
}
