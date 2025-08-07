#pragma once
#include <DxLib.h>
#include "IEnemyAttackState.h"
class RoarAttack 
    : public IEnemyAttackState
{

public:

    // スタンさせ始める時間
    static constexpr float STUN_TRIGGER_TIME = 0.5f;
    
    // 攻撃範囲
    static constexpr float ATTACK_MAX_RANGE = 450.0f;

    // 音が再生され始める時間
    static constexpr float SOUND_START_TIME = 0.5f;
    
    // 音量
    static constexpr int SOUND_VALUE = 255;
    
    // 咆哮が終わる時間
    static constexpr float TIME_LIMIT = 3.0f;

    // エフェクトの大きさ
    static constexpr float EFFECT_SIZE = 25.0f;
    
    // ひとつめのエフェクトのy座標
    static constexpr float F_EFFECT_POS_Y = 100.0f;
    
    // ふたつめのエフェクトのz方向のサイズ
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
    float waveTimer_;       // 当たり判定用時間
    float effectRadius_;    //エフェクト用の半径
    bool waveAtMax_;        // 半径が最大になったらtrueを返す
    bool hasStunnedPlayer_; //プレイヤーがスタンしているかどうか


    // エフェクト
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

