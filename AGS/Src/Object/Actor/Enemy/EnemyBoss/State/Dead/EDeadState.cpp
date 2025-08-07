#include "EDeadState.h"
#include "../../../../../../Manager/SceneManager.h"
#include"../../EnemyBoss.h"
#include "../../Animation/EnemyBossAnimation.h"
#include "../EnemyBossStateMachine.h"

EDeadState::EDeadState(void)
{
    time_ = 0.0f;
    deathAnimationDuration_ = 0.0f;
}

EDeadState::~EDeadState(void)
{
}

void EDeadState::Enter(EnemyBoss& boss)
{
	deathAnimationStarted_ = false;
	deathAnimationFinishedReported_ = false;
}

void EDeadState::Update(EnemyBoss& boss)
{
	float dt = SceneManager::GetInstance().GetDeltaTime();
    
    boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::DEATH, false);

    if (!deathAnimationStarted_)
    {
        deathAnimationStarted_ = true;
    }

    // ���S�A�j���[�V�����̐i�s��ǐ�
    if (!deathAnimationFinishedReported_) 
    {
        timer_ += dt;
        if (timer_ >= deathAnimationDuration_) 
        {
            // �A�j���[�V�������I�������Ɣ��f
            boss.OnDeathAnimationFinished();
            deathAnimationFinishedReported_ = true;
        }
    }
}

void EDeadState::Exit(EnemyBoss& boss)
{
}
