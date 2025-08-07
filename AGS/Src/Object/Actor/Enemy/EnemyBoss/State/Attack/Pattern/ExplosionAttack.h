#pragma once
#include <DxLib.h>
#include "IEnemyAttackState.h"

class ExplosionAttack 
    : public IEnemyAttackState
{

public:

    // �X�^�����鎞��
    static constexpr float SELF_STUN_DURATION = 3.0f;
    
    // �U���ΏۂɍU�����W���Œ肷��܂ł̎���
    static constexpr float TARGET_LOCK_TIME = 2.0f;

    // ��]�̐���
    static constexpr float DIRECTION_VEC_LIMIT = 0.001f;
    
    // �����Đ�����鎞��
    static constexpr float SOUND_START_TIME = 5.0f;
    
    // �U�����莞��
    static constexpr float ATTACK_DIRECTION_TIME = 6.0f;
    
    // �U������̔��a
    static constexpr float ATTACK_RADIUS = 80.0f;
    
    // �_���[�W�l
    static constexpr int DAMEGE_VALUE = 100;
    
    // �U�����I�����鎞��
    static constexpr float EXPLOSION_END_TIME = 8.0f;

    // �G�t�F�N�g��XZ�������̑傫��
    static constexpr float EFFECT_SIZE_XZ = 200.0f;
    
    // �G�t�F�N�g��Y�������̑傫��
    static constexpr float EFFECT_SIZE_Y = 150.0f;
    
    // �G�t�F�N�g��Y���W�̒���
    static constexpr float EFFECT_POS_Y_ADJUSTMENT = 100.0f;

    // ����
    static constexpr int SOUND_VALUE = 255;


    ExplosionAttack(EnemyBoss& boss);
    ~ExplosionAttack(void);

    void Enter(EnemyBoss& boss)override;
    void Update(EnemyBoss& boss)override;
    void Exit(EnemyBoss& boss)override;
    bool IsFinished()const override;
    void StopEffect(void) override;

private:
    EnemyBoss& boss_;
    float selfStunDuration_; // ���g���X�^�������鎞��
    VECTOR targetPosition_;         // �Ώۍ��W
    bool isAttack_;                 // �U���������ǂ���
    bool targetPosionFixed_;        // �Ώۍ��W���Œ肷�邩�ǂ���
    bool reachedTargetPos_;         // �U�����W�����܂������ǂ���

    bool hasSelfStunned_;           // ���g���X�^����Ԃł��邩�ǂ���

    // �G�t�F�N�g
    int effectExploPlayId_;
    int effectBuffResId_;
    bool IsEffect_;

    void InitEffect(void);
    void UpdateEffect(void);

	int soundExplosion_;
	bool soundPlayed_;
    void InitSound(void);

};

