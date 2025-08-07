#pragma once
#include <DxLib.h>
#include "IEnemyAttackState.h"
class RoarAttack 
    : public IEnemyAttackState
{

public:

    // �X�^�������n�߂鎞��
    static constexpr float STUN_TRIGGER_TIME = 0.5f;
    
    // �U���͈�
    static constexpr float ATTACK_MAX_RANGE = 450.0f;

    // �����Đ�����n�߂鎞��
    static constexpr float SOUND_START_TIME = 0.5f;
    
    // ����
    static constexpr int SOUND_VALUE = 255;
    
    // ���K���I��鎞��
    static constexpr float TIME_LIMIT = 3.0f;

    // �G�t�F�N�g�̑傫��
    static constexpr float EFFECT_SIZE = 25.0f;
    
    // �ЂƂ߂̃G�t�F�N�g��y���W
    static constexpr float F_EFFECT_POS_Y = 100.0f;
    
    // �ӂ��߂̃G�t�F�N�g��z�����̃T�C�Y
    static constexpr float S_EFFECT_SIZE_Z = 15.0f;

    RoarAttack(EnemyBoss& boss);
    ~RoarAttack(void);

    void Enter(EnemyBoss& boss)override;
    void Update(EnemyBoss& boss)override;
    void Exit(EnemyBoss& boss)override;
    bool IsFinished()const override;
    void StopEffect(void) override;

private:
    EnemyBoss& boss_;
    float waveTimer_;       // �����蔻��p����
    float effectRadius_;    //�G�t�F�N�g�p�̔��a
    bool waveAtMax_;        // ���a���ő�ɂȂ�����true��Ԃ�
    bool hasStunnedPlayer_; //�v���C���[���X�^�����Ă��邩�ǂ���


    // �G�t�F�N�g
    int effectRoarFPlayId_;
    int effectRoarFResId_;

    int effectRoarSPlayId_;
    int effectRoarSResId_;
    bool IsEffect_;

    void InitEffect(void);
    void UpdateEffect(EnemyBoss& boss);

	void InitSound(void);
	int soundRoar_;
	bool soundPlayed_;
};

