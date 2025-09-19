#include "SetUpAttack.h"
#include <DxLib.h>
#include "../../../../../../../Manager/SceneManager.h"
#include "../../../../../../../Manager/ResourceManager.h"
#include "../../../EnemyBoss.h"
#include "../../../Animation/EnemyBossAnimation.h"


SetUpAttack::SetUpAttack(EnemyBoss& boss) 
    :
    boss_(boss)
{
}

SetUpAttack::~SetUpAttack(void)
{
}

void SetUpAttack::Enter(EnemyBoss& boss)
{
    isFinished_ = false;
    timer_ = 0.0f;
    buffApplied_ = false;
    chargeFinished_ = false;
    boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::SETUP, false);
}

void SetUpAttack::Update(EnemyBoss& boss)
{
    timer_ += SceneManager::GetInstance().GetDeltaTime();

    // �`���[�W�t�F�[�Y
    if (timer_ < CHARGE_DURATION)
    {
        // �o�t�K�p�^�C�~���O�̃`�F�b�N
        if (!buffApplied_ && timer_ >= BUFF_APPLY_TIME)
        {
            // �{�X���g�̃o�t��K�p
            boss.SetAttackBuff(BUFF_DURATION, ATTACK_MULTIPLIER);
            buffApplied_ = true; // �o�t�͈�x�����K�p
        }
    }
    else 
    {
        // �`���[�W������A�c��̍U���A�j���[�V�����Ȃ�
        if (!chargeFinished_) 
        {
            chargeFinished_ = true;
        }
    }

    if (timer_ >= CHARGE_DURATION + ATTACK_DURATION) // �S�Ẵt�F�[�Y���I��������U������
    {
        if (!IsFinished())
        {
            isFinished_ = true;
        }
    }
}

void SetUpAttack::Exit(EnemyBoss& boss)
{
}

bool SetUpAttack::IsFinished() const
{
    return isFinished_;
}

void SetUpAttack::StopEffect(void)
{
}
