#include "ThrustAttack.h"
#include <DxLib.h>
#include "../../../../../../../Utility/AsoUtility.h"
#include "../../../../../../../Manager/SceneManager.h"
#include "../../../../../../../Manager/ResourceManager.h"
#include "../../../EnemyBoss.h"
#include "../../../Animation/EnemyBossAnimation.h"
#include "../../EnemyBossCombatSystem.h"
#include "../../../../../Player/Player.h"


ThrustAttack::ThrustAttack(EnemyBoss& boss) 
    :
    boss_(boss),
    initialPos_({ 0.0f,0.0f,0.0f }),
    chargeDirection_({ 0.0f,0.0f ,0.0f })
{
    isFinished_ = false;
    timer_ = 0.0f;
    initialPos_ = AsoUtility::VECTOR_ZERO;
    chargeDirection_ = AsoUtility::VECTOR_ZERO;
}

ThrustAttack::~ThrustAttack(void)
{
}

void ThrustAttack::Enter(EnemyBoss& boss)
{
    InitSound();
    isFinished_ = false;
    timer_ = 0.0f;
    isSoundPlayed_ = false;

    // �ːi�J�n�ʒu��ۑ�
    initialPos_ = boss.GetPlayer().GetTransform().pos;

    // �v���C���[������ːi�����Ƃ���
    chargeDirection_ = VNorm(boss.GetToPlayer());

    // ���̃A�j���[�V�����Đ�
    boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::THRUST, false); 
}

void ThrustAttack::Update(EnemyBoss& boss)
{
    float dt = SceneManager::GetInstance().GetDeltaTime();
    timer_ += dt;

    float t = timer_ / DURATION; // 0.0����1.0�ɐ��K�����ꂽ����

    // ���Ԃ��o�߂�����U���I��
    if (t > 1.0f) 
    {
        t = 1.0f; // �N�����v
    }

    float currentSpeed = GetCurrentSpeed(t);

    VECTOR diff = VSub(initialPos_, boss.GetTransform().pos);
    float dist = std::sqrt(diff.x * diff.x + diff.z * diff.z);

    if (dist < DIST_LIMIT)
    {
        float targetPos = VSize(VSub(boss.GetPlayer().GetTransform().pos, initialPos_));
        if (targetPos > TARGETPOS_LIMIT)
        {
            VECTOR pos = boss.GetTransformMutable().pos;
            pos.x += chargeDirection_.x * currentSpeed;
            pos.z += chargeDirection_.z * currentSpeed;
        }
    }

    if (!boss.GetAnimation()->IsEnd())
    {
        if (!isSoundPlayed_)
        {
            PlaySoundMem(noHitSound_, DX_PLAYTYPE_BACK, true);
            isSoundPlayed_ = true;
        }
        if (timer_ < ATTACK_TIME_LIMIT)
        {
            if (boss.GetAttackStateSystem()->IsHit())
            {
                boss.GetPlayer().Damage(DAMAGE_VALUE);
            }
        }
        else 
        {
            if (!isSoundPlayed_)
            {
                PlaySoundMem(noHitSound_, DX_PLAYTYPE_BACK, true);
                isSoundPlayed_ = true;
            }
        }
    }

    if (boss.GetAnimation()->IsEnd())
    {
        boss.GetAttackStateSystem()->ClearHitRecord();
        isFinished_ = true;
    }
}

void ThrustAttack::Exit(EnemyBoss& boss)
{
}

bool ThrustAttack::IsFinished() const
{
    return isFinished_;
}

void ThrustAttack::StopEffect(void)
{
}

// �ŏ��͒x���A�������A����
float ThrustAttack::GetCurrentSpeed(float t)
{
    if (t < DECELERATION_RATE)
    {
        // �����t�F�[�Y (�ŏ��͒x���A�s�[�N��)
        // ease-in-out �̂悤�ȃJ�[�u��z��
        float progress = t / DECELERATION_RATE; // 0.0����1.0
        return INITIAL_SPEED + (PEAK_SPEED - INITIAL_SPEED) * sinf(progress * DX_PI_F / 2.0f); // �Ȑ��I�ȉ���
    }
    else 
    {
        // �����t�F�[�Y (�s�[�N����0��)
        float progress = (t - DECELERATION_RATE) / (1.0f - DECELERATION_RATE); // 0.0����1.0
        return PEAK_SPEED * (1.0f - progress); // ���`���� (�K�v�ɉ����ċȐ���)
    }
}

void ThrustAttack::InitSound(void)
{
    noHitSound_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::ATTACK_SE).handleId_;
    ChangeVolumeSoundMem(SOUND_VALUE, noHitSound_);
}
