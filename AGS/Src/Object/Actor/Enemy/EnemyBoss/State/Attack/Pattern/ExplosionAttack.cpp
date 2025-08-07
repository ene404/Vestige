#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "ExplosionAttack.h"
#include "../../../../../../../Utility/AsoUtility.h"
#include "../../../../../../../Manager/SceneManager.h"
#include "../../../../../../../Manager/ResourceManager.h"
#include "../../../EnemyBoss.h"
#include "../../../Animation/EnemyBossAnimation.h"
#include "../../../../../Player/Player.h"
#include "../../EnemyBossStateMachine.h"

ExplosionAttack::ExplosionAttack(EnemyBoss& boss) 
    :
    boss_(boss)
{
    selfStunDuration_ = SELF_STUN_DURATION;
    targetPosition_ = AsoUtility::VECTOR_ZERO;
    isAttack_ = false;
    targetPosionFixed_ = false;


    InitEffect();
	InitSound();
}

ExplosionAttack::~ExplosionAttack(void)
{
    DeleteEffekseerEffect(effectBuffResId_);
}

void ExplosionAttack::Enter(EnemyBoss& boss)
{
    isFinished_ = false;
    timer_ = 0.0f;
    isAttack_ = false;
    targetPosionFixed_ = false;
    reachedTargetPos_ = false;
    IsEffect_ = false;
	soundPlayed_ = false;
    hasSelfStunned_ = false;
}

void ExplosionAttack::Update(EnemyBoss& boss)
{
    timer_ += SceneManager::GetInstance().GetDeltaTime();

    // �t�F�[�Y1: �^�[�Q�b�g�Œ�Ɨ\���G�t�F�N�g�J�n
    if (!targetPosionFixed_)
    {
        // �^�[�Q�b�g�ʒu�𖈃t���[���X�V��������
        targetPosition_ = boss.GetPlayer().GetTransform().pos;

        if (timer_ >= TARGET_LOCK_TIME) // �\�����Ԃ��o�߂�����^�[�Q�b�g���Œ�
        {
            targetPosionFixed_ = true;
        }
    }
    // �t�F�[�Y2: �Œ肳�ꂽ�^�[�Q�b�g�ʒu�ւ̈ړ�
    else if (!reachedTargetPos_)
    {

        VECTOR dirVec = VSub(targetPosition_, boss.GetTransform().pos);
        if (AsoUtility::MagnitudeF(dirVec) > DIRECTION_VEC_LIMIT)
        {
            boss.LookAt(targetPosition_);
        }
        boss.GetAnimation()->Play(EnemyBossAnimation::ANIM_TYPE::EXPLOSION, false, EnemyBoss::ANIM_BLEND_RATE);
    }

    if (!IsEffect_ && effectExploPlayId_ != -1) // �\���G�t�F�N�g�͍ŏ��Ɉ�x�����J�n
    {
        UpdateEffect();
        IsEffect_ = true;
    }

    if (!soundPlayed_ && timer_ >= SOUND_START_TIME)
    {
        if (soundExplosion_ != -1)
        {
            PlaySoundMem(soundExplosion_, DX_PLAYTYPE_BACK, true);
            soundPlayed_ = true;
        }
    }

    if (timer_ >= ATTACK_DIRECTION_TIME && !isAttack_)
    {
        // �U������
        float pDist = AsoUtility::MagnitudeF(VSub(boss.GetPlayer().GetTransform().pos, targetPosition_));
        if (pDist <= ATTACK_RADIUS)  // �����͈�
        {
            boss.GetPlayer().Damage(DAMEGE_VALUE);
            isAttack_ = true;
        }
    }

    // �t�F�[�Y4: �U���I���Ǝ��ȃX�^��
    if (timer_ >= EXPLOSION_END_TIME) // �U���̑����Ԃ��o�߂�����I��
    {
        if (!hasSelfStunned_)
        {
            boss.SetStunned(selfStunDuration_); // �{�X���g���X�^����Ԃɂ���
            hasSelfStunned_ = true;
        }
        StopEffekseer3DEffect(effectExploPlayId_); // �G�t�F�N�g���~

        reachedTargetPos_ = true;
        soundPlayed_ = false;
        isFinished_ = true;
    }

}

void ExplosionAttack::Exit(EnemyBoss& boss)
{
}

bool ExplosionAttack::IsFinished() const
{
    return isFinished_;
}

void ExplosionAttack::StopEffect(void)
{
    StopEffekseer3DEffect(effectExploPlayId_);
}

void ExplosionAttack::InitEffect(void)
{
    // �G�t�F�N�g
    effectBuffResId_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::EXPLOSION_EFFECT).handleId_;

    // �傫��
    SetScalePlayingEffekseer3DEffect(effectExploPlayId_, EFFECT_SIZE_XZ, EFFECT_SIZE_Y, EFFECT_SIZE_XZ);

}

void ExplosionAttack::UpdateEffect(void)
{
    // �G�t�F�N�g�Đ����J�n���A�C���X�^���X�n���h�����擾
    effectExploPlayId_ = PlayEffekseer3DEffect(effectBuffResId_);

    SetScalePlayingEffekseer3DEffect(effectExploPlayId_, EFFECT_SIZE_XZ, EFFECT_SIZE_Y, EFFECT_SIZE_XZ);

    SetPosPlayingEffekseer3DEffect(effectExploPlayId_, targetPosition_.x, targetPosition_.y - EFFECT_POS_Y_ADJUSTMENT, targetPosition_.z);
}

void ExplosionAttack::InitSound(void)
{
	soundExplosion_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EXP_SE).handleId_;

    ChangeVolumeSoundMem(SOUND_VALUE, soundExplosion_);
}
