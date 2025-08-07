#pragma once
#include <memory>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include "AttackPatternStateMachine.h"
#include "Attack/Pattern/IEnemyAttackState.h"
class EnemyBoss;

// �U����ԑJ�ڊǗ�
class EnemyBossCombatController
{

public:

    const float COMBO_THRUST_VIEW_ANGLE_DEG = 30.0f; // ���ʂ��獶�E30�x�i���v60�x�j
    const float COMBO_THRUST_MAX_DISTANCE = 600.0f; // ���̋������ł̂ݎ��씻����s��
    static constexpr float HP_RATIO_HALF = 0.5f;
    static constexpr float HP_RATIO_0_4 = 0.4f;

    EnemyBossCombatController(EnemyBoss& boss);

    // �v���C���[�Ƃ̋����ƃN�[���_�E�����l�����āA���s�\�ȍU���̒�����œK�Ȃ��̂�Ԃ�
    EnemyBoss::ATK_STATE SelectAttackCandidate();

    // �U�������s���ꂽ��ɃN�[���_�E����ݒ�
    void SetAttackCooldown(EnemyBoss::ATK_STATE attackType);

    // ���t���[���N�[���_�E�����X�V
    void Update(float dt);

    // ����̍U�����N�[���_�E������
    bool IsAttackOnCooldown(EnemyBoss::ATK_STATE attackType) const;
    
    // �S�Ă̍U�����N�[���_�E������
    bool AreAllAttacksOnCooldown() const;

    // �A���U�����J�n����ۂɁA���̍U���V���[�Y�𐶐�����
    void GenerateNewAttackSeries(float dist);

    // �V���[�Y���̎��̍U�����擾
    EnemyBoss::ATK_STATE GetNextAttackInSeries() const;

    // ���݂̘A���U���V���[�Y�����Z�b�g
    void ResetContinuousAttackSeries();

    // �A���U���V���[�Y�̃C���f�b�N�X��i�߂�
    void AdvanceContinuousAttackIndex();
    
    // ���݂̘A���U��������ڂ� (1-indexed)
    int GetCurrentContinuousAttackCount() const;
    
    // ���݂̘A���U���V���[�Y�̑���
    int GetTotalContinuousAttackCount() const;

private:
    EnemyBoss& boss_;

    // �e�U�����Ƃ̃N�[���_�E��
    std::map<EnemyBoss::ATK_STATE, float> attackCooldowns_;

    // �e�U���̃N�[���_�E������ (�ݒ�l)
    const std::map<EnemyBoss::ATK_STATE, float> ATTACK_COOLDOWN_TIMES = {
        {EnemyBoss::ATK_STATE::COMBO,  3.5f},
        {EnemyBoss::ATK_STATE::THRUST, 4.5f},
        {EnemyBoss::ATK_STATE::ROAR, 15.0f},
        {EnemyBoss::ATK_STATE::EXPLOSION, 20.0f},
        {EnemyBoss::ATK_STATE::SETUP, 60.0f},
    };

    // �e�U���̐��������͈� (min_dist, max_dist)
    const std::map<EnemyBoss::ATK_STATE, std::pair<float, float>> ATTACK_RANGES = {
        {EnemyBoss::ATK_STATE::COMBO, {0.0f, 180.0f}},
        {EnemyBoss::ATK_STATE::THRUST, {100.0f, 300.0f}},
        {EnemyBoss::ATK_STATE::ROAR, {0.0f, 180.0f}},
        {EnemyBoss::ATK_STATE::EXPLOSION, {0.0f, 800.0f}},
        {EnemyBoss::ATK_STATE::SETUP, {0.0f, 1000.0f}},
    };

    // �A���U���̂��߂ɓ�������
    std::vector<EnemyBoss::ATK_STATE> currentAttackSeries_; // ���݂̘A���U���V���[�Y
    int currentSeriesIndex_ = 0;
};

