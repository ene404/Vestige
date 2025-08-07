#include "EnemyBossCombatController.h"
#include <memory>
#include <DxLib.h>
#include "../../../../../Manager/SceneManager.h"
#include "Attack/EAttackState.h"
#include "Attack/Pattern/ComboAttack.h"
#include "Attack/Pattern/ThrustAttack.h"
#include "Attack/Pattern/RoarAttack.h"
#include "Attack/Pattern/ExplosionAttack.h"
#include "Attack/Pattern/SetUpAttack.h"

EnemyBossCombatController::EnemyBossCombatController(EnemyBoss& boss)
    : 
    boss_(boss)
{
    // �e�U���̏����N�[���_�E�����[���ɐݒ�
    for (const auto& pair : ATTACK_COOLDOWN_TIMES) 
    {
        attackCooldowns_[pair.first] = 0.0f;
    }
}

EnemyBoss::ATK_STATE EnemyBossCombatController::SelectAttackCandidate()
{
    // �v���C���[�Ƃ̋������擾
    float dist = boss_.GetPlayerDistanceXZ(); 

    // �A���U������������Ă��Ȃ��ꍇ�A�V���ɐ�������
    if (currentAttackSeries_.empty()) 
    {
        GenerateNewAttackSeries(dist);
        currentSeriesIndex_ = 0;
    }

    // ���݂̘A���U������A���Ɏ��s���ׂ��U����I��
    if (currentSeriesIndex_ < currentAttackSeries_.size()) 
    {
        EnemyBoss::ATK_STATE candidate = currentAttackSeries_[currentSeriesIndex_];

        // �I�����ꂽ�U�����N�[���_�E�����ł͂Ȃ����ŏI�`�F�b�N
        if (!IsAttackOnCooldown(candidate)) 
        {
            return candidate;
        }
        else 
        {
            // �N�[���_�E�����̂��ߘA���U���𒆒f
            ResetContinuousAttackSeries(); 
            return EnemyBoss::ATK_STATE::NONE;
        }
    }

    // �A���U�����I���������A�I���ł���U�����Ȃ��ꍇ
    ResetContinuousAttackSeries(); // �V���[�Y�����Z�b�g
    return EnemyBoss::ATK_STATE::NONE;
}

// �U�������s���ꂽ��ɃN�[���_�E����ݒ�
void EnemyBossCombatController::SetAttackCooldown(EnemyBoss::ATK_STATE attackType)
{
    if (ATTACK_COOLDOWN_TIMES.count(attackType)) 
    {
        attackCooldowns_[attackType] = ATTACK_COOLDOWN_TIMES.at(attackType);
    }
}

void EnemyBossCombatController::Update(float dt)
{
    // �S�Ă̍U���N�[���_�E�����X�V
    for (auto& pair : attackCooldowns_) 
    {
        if (pair.second > 0.0f) 
        {
            pair.second -= dt;
            if (pair.second < 0.0f) 
            {
                pair.second = 0.0f;
            }
        }
    }
}

bool EnemyBossCombatController::IsAttackOnCooldown(EnemyBoss::ATK_STATE attackType) const
{
    if (attackCooldowns_.count(attackType)) 
    {
        return attackCooldowns_.at(attackType) > 0.0f;
    }
    return false;
}

bool EnemyBossCombatController::AreAllAttacksOnCooldown() const
{
    for (const auto& pair : ATTACK_COOLDOWN_TIMES) 
    {
        if (attackCooldowns_.count(pair.first) == 0 || attackCooldowns_.at(pair.first) <= 0.0f) 
        {
            return false;
        }
    }
    return true;
}

void EnemyBossCombatController::ResetContinuousAttackSeries()
{
    currentAttackSeries_.clear();
    currentSeriesIndex_ = 0;
}

void EnemyBossCombatController::AdvanceContinuousAttackIndex()
{
    currentSeriesIndex_++;
}

int EnemyBossCombatController::GetCurrentContinuousAttackCount() const
{
    return currentSeriesIndex_ + 1;
}

int EnemyBossCombatController::GetTotalContinuousAttackCount() const
{
    return static_cast<int>(currentAttackSeries_.size());
}

void EnemyBossCombatController::GenerateNewAttackSeries(float dist)
{
    currentAttackSeries_.clear();
    std::vector<EnemyBoss::ATK_STATE> availableAttacks;

    // HP�̊������擾
	float hpRatio = boss_.GetHpRatio(); 

    // HP5���ȉ��ň�x�����ARoar , Explosion �̂ǂ��炩�𔭓�������
    if (hpRatio <= HP_RATIO_HALF)
    {
        if (!boss_.hasUsedRoarBelow50Percent_ && !IsAttackOnCooldown(EnemyBoss::ATK_STATE::ROAR)) 
        {
            currentAttackSeries_.push_back(EnemyBoss::ATK_STATE::ROAR);
            boss_.hasUsedRoarBelow50Percent_ = true;
            return;
        }
        if (!boss_.hasUsedExplosionBelow50Percent_ && !IsAttackOnCooldown(EnemyBoss::ATK_STATE::EXPLOSION)) 
        {
            currentAttackSeries_.push_back(EnemyBoss::ATK_STATE::EXPLOSION);
            boss_.hasUsedExplosionBelow50Percent_ = true;
            return;
        }
    }
    
    // Roar, Explosion ���������Ȃ������ꍇ�Ƀ`�F�b�N�����
    if (hpRatio <= HP_RATIO_0_4 && !boss_.hasUsedSetupBelow40Percent_ && !IsAttackOnCooldown(EnemyBoss::ATK_STATE::SETUP))
    {
        currentAttackSeries_.push_back(EnemyBoss::ATK_STATE::SETUP);
        boss_.hasUsedSetupBelow40Percent_ = true;
        return;
    }

    for (const auto& pair : ATTACK_RANGES) 
    {
        EnemyBoss::ATK_STATE attackType = pair.first;
        float minDist = pair.second.first;
        float maxDist = pair.second.second;

        if ((attackType == EnemyBoss::ATK_STATE::ROAR && (hpRatio > HP_RATIO_HALF || boss_.hasUsedRoarBelow50Percent_)) ||
            (attackType == EnemyBoss::ATK_STATE::EXPLOSION && (hpRatio > HP_RATIO_HALF || boss_.hasUsedExplosionBelow50Percent_)))
        {
            continue;
        }

        if ((attackType == EnemyBoss::ATK_STATE::SETUP))
        {
            continue;
        }

        bool meetsDistance = (dist >= minDist && dist <= maxDist);

        if (attackType == EnemyBoss::ATK_STATE::COMBO || attackType == EnemyBoss::ATK_STATE::THRUST) 
        {
            // ���������Ǝ���p�����̗����𖞂����ꍇ
            if (meetsDistance && boss_.IsPlayerInFront(COMBO_THRUST_VIEW_ANGLE_DEG, maxDist)) 
            {
                availableAttacks.push_back(attackType);
            }
        }
        // ���̑��̒ʏ�U�� (����p�����Ȃ��A���������̂�)
        else 
        {
            if (meetsDistance) 
            {
                availableAttacks.push_back(attackType);
            }
        }
    }

    if (availableAttacks.empty()) 
    {
        currentAttackSeries_.clear();
        return;
    }

    // �A���U���񐔂����� (1�񂩂�ő�2��܂�)
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    // 1����3��A���U��
    std::uniform_int_distribution<int> numAttacksDist(1, 2); 
    int numAttacksInSeries = numAttacksDist(engine);

    // �A���U���V���[�Y�𐶐�
    for (int i = 0; i < numAttacksInSeries; ++i)
    {
        // ���p�\�ȍU���̒����烉���_���ɑI�����ĘA���U���ɒǉ�
        std::uniform_int_distribution<int> attackTypeDist(0, static_cast<int>(availableAttacks.size() - 1));
        currentAttackSeries_.push_back(availableAttacks[attackTypeDist(engine)]);
    }
}

EnemyBoss::ATK_STATE EnemyBossCombatController::GetNextAttackInSeries() const
{
    if (currentSeriesIndex_ < currentAttackSeries_.size()) 
    {
        return currentAttackSeries_[currentSeriesIndex_];
    }
    return EnemyBoss::ATK_STATE::NONE;
}
