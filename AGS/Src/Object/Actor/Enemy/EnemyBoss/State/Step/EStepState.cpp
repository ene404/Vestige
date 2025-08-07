#include "EStepState.h"
#include "../../../../../../Manager/SceneManager.h"
#include "../../EnemyBoss.h"
#include "../../Animation/EnemyBossAnimation.h"
#include "../EnemyBossStateMachine.h"
#include "../../../../Player/Player.h"

EStepState::EStepState(void)
{
    stepCount_ = 0;
    backStepTimer_ = 0.0f;
    height_ = 0.0f;
}

EStepState::~EStepState(void)
{
}

void EStepState::Enter(EnemyBoss& boss)
{
    stepCount_ = 0;
    backStepTimer_ = 0.0f;
    boss.LookAt(boss.GetPlayer().GetTransform().pos);
    boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::STEP, true, EnemyBoss::ANIM_BLEND_RATE);
}

void EStepState::Update(EnemyBoss& boss)
{
    boss.LookAt(boss.GetPlayer().GetTransform().pos);
    
    // 2��ڂ͉����ɉ�����
    float distance = (stepCount_ == 0) ? FIRST_DIS : SECOND_DIS;  
    VECTOR pos = boss.GetTransform().pos;
    VECTOR toPlayer = VSub(pos, boss.GetPlayer().GetTransform().pos);
    
    // �v���C���[�̋t�����i��ޕ����j
    VECTOR dir = VNorm(VScale(toPlayer, 1.0f));

    if (backStepTimer_ < DURATION)
    {
        backStepTimer_ += SceneManager::GetInstance().GetDeltaTime();

        boss.GetTransformMutable().pos = CalculateBackstepPosition(pos, dir, backStepTimer_, DURATION, distance, height_);
    }
    else
    {
        backStepTimer_ = 0.0f;
        stepCount_++;
        if (stepCount_ < STEP_MAX_COUNT)
        {
            // 2��ڂ̃X�e�b�v�J�n�B
            boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::STEP, true, EnemyBoss::ANIM_BLEND_RATE);
        }
        else
        {
            // 2��X�e�b�v����������Recover�X�e�[�g�֑J��
            boss.ChangeState(EnemyBoss::STATE::RECOVER);
        }
    }
}

void EStepState::Exit(EnemyBoss& boss)
{
}

VECTOR EStepState::CalculateBackstepPosition(const VECTOR& startPos, const VECTOR& dir, float elapsedTime, float duration, float distance, float height)
{
    float t = elapsedTime / duration;
    if (t > 1.0f) t = 1.0f;

    // ���_�炩�������ƌ���
    float easedT = 1.0f - (1.0f - t) * (1.0f - t);

    // �����̕�Ԃ��኱�����u��������v�悤�ɒ���
    float yOffset = height * sinf(t * PI); // �T�C���g�łӂ���Ə㉺

    // �X�e�b�v�����ɋ�������
    VECTOR moveOffset = VScale(dir, distance * easedT);

    VECTOR result = 
    {
        startPos.x + moveOffset.x,
        startPos.y + yOffset,
        startPos.z + moveOffset.z
    };

    return result;
}
