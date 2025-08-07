#pragma once
#include "../../ActorBase.h"
#include <map>
#include <unordered_set>
#include <functional>
#include <vector>
class Collider;
class Capsule;
class Player;
class PlayerDummy;
class EnemyWeapon;
class Mist;

class EnemyStateMachine;
class EnemyBossAnimation;
class EnemyBossCombatSystem;
class EnemyBossCombatController;
class EnemyBossAttackStateMachine;

class EnemyBoss : public ActorBase
{
public:

	// 初期座標
	static constexpr VECTOR INIT_POS = { 0.0f, -30.0f, 900.0f };

	// サイズ
	static constexpr float SIZE = 1.3f;

	// 初期ローカル回転

	static constexpr float INIT_QUAROT_LOCAL_X = 0.0f;
	static constexpr float INIT_QUAROT_LOCAL_Y = 180.0f;
	static constexpr float INIT_QUAROT_LOCAL_Z = 0.0f;

	// 最大HP
	static constexpr int MAX_HP_VALUE = 2000;

	// 初期化アニメーション
	static constexpr float IDLE_ANIM_SPEED = 20.0f;
	static constexpr float WALK_F_ANIM_SPEED = 10.0f;
	static constexpr float WALK_B_ANIM_SPEED = 20.0f;
	static constexpr float RUN_ANIM_SPEED = 20.0f;
	static constexpr float STEP_ANIM_SPEED = 40.0f;
	static constexpr float COMBO_ANIM_SPEED = 55.0f;
	static constexpr float THRUST_ANIM_SPEED = 40.0f;
	static constexpr float ROAR_ANIM_SPEED = 20.0f;
	static constexpr float EXPLOSION_ANIM_SPEED = 15.0f;
	static constexpr float STUN_IDLE_ANIM_SPEED = 20.0f;
	static constexpr float SET_UP_ANIM_SPEED = 20.0f;
	static constexpr float DEATH_ANIM_SPEED = 30.0f;
	static constexpr float ANIM_BLEND_RATE = 0.5f;

	// 重力関連
	static constexpr float GRAVITY_CHECK_POW = 10.0f;
	static constexpr float FALLINT_DOT_THRESHOLD = 0.9f;

	// カプセル関連
	static constexpr float CAPSULE_TOP_LOCAL_POS_Y = 150.0f;
	static constexpr float CAPSULE_DOWN_LOCAL_POS_Y = 30.0f;
	static constexpr float CAPSULE_RADIUS = 30.0f;
	static constexpr int CAPSULE_LIMIT_COUNT = 10;

	// ダメージ関連
	static constexpr float DECREASE_SPEED = 2.0f;

	// 武器関連
	static constexpr int WEAPON_TOP_INDEX = 3;
	static constexpr int WEAPON_DOWN_INDEX = 6;
	static constexpr float WEAPON_ROLL = 0.0f;
	static constexpr float WEAPON_FORWARD_OFFSET = -5.0f;
	static constexpr float WEAPON_UP_OFFSET = -20.0f;
	static constexpr float WEAPON_RIGHT_OFFSET = -5.0f;
	static constexpr VECTOR WEAPON_UPVEC = { 0.0f,1.0f,0.0f };
	static constexpr float WEAPON_CAPSULE_FORWARD_OFFSET = 20.0f;
	static constexpr float WEAPON_CAPSULE_UP_OFFSET = 0.0f;
	static constexpr float WEAPON_CAPSULE_RIGHT_OFFSET = 5.0f;
	static constexpr VECTOR WEAPON_CAPSULE_UPVEC = { 0.0f,1.0f,0.0f };

	// スタン関連
	static constexpr float STUN_TIME = 5.0f;
	static constexpr int LIGHTNING_DAMAGE = 300;

	// エフェクト関連
	// バフ
	static constexpr float BUFF_EFFECT_SIZE = 100.0f;
	static constexpr int BUFF_EFFECT_SOUND = 255;

	// スタン
	static constexpr float STUN_EFFECT_SIZE = 50.0f;

	// 死亡関連
	static constexpr float DEAD_INTERVAL_TIME = 5.0f;

	// 回避のクールダウン間隔
	static constexpr float STEP_COOLDOWN_DURATON = 5.0f;

	// HPバー関連
	static constexpr int WIDTH = 650;
	static constexpr int HEIGHT = 20;
	static constexpr int X_ADJUSTMENT = 60;
	static constexpr int Y_ADJUSTMENT = 80;
	static constexpr int FONT_SIZE = 20;
	static constexpr int NAME_HEIGHT = 24;
	static constexpr int PADDING_LEFT = 8;
	static constexpr int PADDING = 8;
	static constexpr int NAME_BLEND_PARAM = 160;
	static constexpr int MAX_COL = 255;
	static constexpr int TEXCOL_R = 255;
	static constexpr int TEXCOL_G = 255;
	static constexpr int TEXCOL_B = 255;
	static constexpr int A_QUARTER_COL = 60;
	static constexpr int DAMEGE_COL_LIGHT_R = 255;
	static constexpr int DAMEGE_COL_LIGHT_GB = 200;
	static constexpr int DAMEGE_COL_TOP_R = 255;
	static constexpr int DAMEGE_COL_TOP_GB = 100;
	static constexpr int DAMEGE_COL_BOTTOM_R = 200;
	static constexpr int DAMEGE_COL_BOTTOM_GB = 50;
	static constexpr int HP_COL_TOP_R = 255;
	static constexpr int HP_COL_TOP_GB = 80;
	static constexpr int HP_COL_BOTTOM_R = 180;
	static constexpr int HP_COL_BOTTOM_GB = 40;
	static constexpr int BORDER_DARK_COL = 40;
	static constexpr int BORDER_LIGHT_COL = 200;
	static constexpr int DAMAGE_BLEND_PARAM = 180;
	static constexpr int BORDER_DARK_ADJUSTMENT = 2;
	static constexpr int BORDER_LIGHT_ADJUSTMENT = 1;

	// 長さの制限
	static constexpr float LENGTH_LIMIT = 0.001f;

	// 球面補間の速度
	static constexpr float SLERP_ROTIO = 0.05f;

	// 移動速度
	static constexpr float MOVE_SPEED = 5.0f;
	

	// 状態
	enum class STATE
	{
		IDLE,
		OBSERVE,
		MOVE,
		CHASE,
		ATTACK,
		STEP,
		STUN,
		RECOVER,
		DEAD
	};

	enum class ATK_STATE
	{
		NONE,
		COMBO,
		THRUST,
		ROAR,
		EXPLOSION,
		SWEEP,
		FOLLOW_SLASH,
		SETUP
	};

	EnemyBoss(Player& player);
	~EnemyBoss(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;


	// 各クラスのゲッター
	//------------------
	std::shared_ptr<EnemyStateMachine> GetStateMachine() const;
	std::shared_ptr<EnemyBossAnimation> GetAnimation() const;
	std::shared_ptr<EnemyBossCombatSystem> GetAttackStateSystem() const;
	std::shared_ptr<EnemyBossCombatController> GetAttackStateController() const;
	std::shared_ptr<EnemyBossAttackStateMachine> GetAttackStateMachine() const;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	//-------------------------------------
	void SetPlayerDummy(std::shared_ptr<PlayerDummy> playerDummy);

	// 霧との衝突判定用情報を取得する
	void SetMist(std::shared_ptr<Mist> mist);

	// 衝突用カプセルの取得-----------------------
	const std::shared_ptr<Capsule> GetCapsule(void) const;

	int GetHp(void) const;

	// ----------------------------------------
	STATE GetCurrentStateId() const;

	// メインステート変更インターフェース
	void ChangeState(STATE newState);
	void ChangeAttack(ATK_STATE newAttack);

	// EChaseStateからEAttackStateへ渡すための「次に実行する攻撃」
	void SetNextAttackType(ATK_STATE type);
	ATK_STATE GetNextAttackType() const;

	// プレイヤー関連
	Player& GetPlayer();
	float GetPlayerDistanceXZ() const;
	VECTOR GetToPlayer();


	// -----------------------------
	// これらは引数内に自身の座標を入れれば必要ないし、安全性の観点から考えておかしい
	void SetPosition(VECTOR pos);
	Transform& GetTransformMutable();

	// ボスのスタン状態関連
	// ボスをスタン時間を設定する
	void SetStunned(float duration);

	bool IsStunned(void) const;

	//--------------------
	float  GetStunTimer(void) const;

	// ボスの攻撃力アップ状態
	void SetAttackBuff(float duration, float muktiplier);
	void RemoveAttackBuff(void);
	float GetCurrentAttackMultiplier() const;

	// ボスの死亡状態関連
	bool IsDead() const;
	void SetIsDead(bool flag);
	void SetDead();
	float GetDeadTimer() const;
	void OnDeathAnimationFinished(void);

	// Stunエフェクト関連
	void StartStunEffect(void);
	void StopStunEffect(void);
	bool IsStunEffectActive(void);

	// SetUpエフェクト関連
	void StartSetupEffect(void);
	void StopSetupEffect(void);
	bool IsSetupEffectActive(void);

	// 武器関連
	VECTOR GetTopPos();
	VECTOR GetDownPos();

	// HP関連
	int GetMaxHp() const;
	float GetHpRatio() const;

	// HPバーの描画
	void DrawHpBar(void);

	void LookAt(VECTOR pos);

	void MoveToward(const VECTOR& target);

	void DrawCircleOnGround(VECTOR center, float radius, int segment, int color);

	// プレイヤーがボスの正面にいるかを判定する
	bool IsPlayerInFront(float viewAngleDeg, float maxDistance);

	// 攻撃制限フラグ
	bool hasUsedRoarBelow50Percent_ = false;
	bool hasUsedExplosionBelow50Percent_ = false;

	// SETUP攻撃がHP4割以下で一度使用されたかどうかのフラグ
	bool hasUsedSetupBelow40Percent_ = false;

	void CollisionMist(void);

	float GetStepCooldown(void);

	void SetStepCooldown(float duration);

private:

	// プレイヤー
	Player& player_;

	std::shared_ptr<PlayerDummy> playerDummy_;
	std::shared_ptr<Mist> mist_;

	// アニメーション
	std::shared_ptr<EnemyWeapon> weapon_;

	// 状態遷移
	std::shared_ptr<EnemyStateMachine> stateMachine_;

	// 見た目と動作
	std::shared_ptr<EnemyBossAnimation> animation_;

	// 戦闘処理
	std::shared_ptr<EnemyBossCombatSystem> combat_;
	std::shared_ptr<EnemyBossCombatController> combatController_;
	std::shared_ptr<EnemyBossAttackStateMachine> attackFSM_;

	// 攻撃中に一回でもあったプレイヤーIDを記録する
	std::unordered_set<int> hitPlayers_;	

	int hp_;				// ボスのHP
	int maxHp_;				//　HPの最大値
	
	// EChaseStateからEAttackStateへ渡す次の攻撃タイプ
	ATK_STATE nextAttackType_;

	// 移動用
	VECTOR moveDir_;
	VECTOR movedPos_;
	VECTOR movePow_;

	VECTOR jumpPow_;
	bool isJump_;

	// プレイヤーとの距離
	VECTOR toPlayer_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 受けたダメージの描画用
	int damageHp_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 手首のフレーム
	int wristIndex_;
	VECTOR wristPos;

	// 手のフレーム
	int handIndex_;
	VECTOR handPos;

	// 武器のトップ
	int weaponTopIndex_;
	VECTOR weaponTopPos_;

	// 武器の下
	int weaponDownIndex_;
	VECTOR weaponDownPos_;

	// 手のカプセル
	VECTOR capsuleDir_;
	VECTOR capsuleCenter_;

	// 武器用当たり半径
	float radius_;

	// ボスの自己スタン管理
	bool isStunned_;
	float stunTimer_;

	// スタンエフェクト関連
	int effectStunPlayId_;
	int effectStunResId_;
	bool isStunEffectActive_;

	// SetUpエフェクト関連
	int effectBuffPlayId_;
	int effectBuffResId_;
	bool isSetUpEffectActive_; // SetUpエフェクトがアクティブかどうか

	// 攻撃力アップ管理(SETUP用)
	float currentAttackMultiplier_;	// 基本攻撃力
	float attackBuffTimer_;			// 攻撃バフ残り時間

	bool isDead_;			// 死亡時：trueを返す
	float deadTimer_;		// 死亡時のタイマー

	float stepCooldown_;	// Stepクールダウン

	// モデル情報の初期化
	void InitModel(void);

	//パラメータの初期化
	void InitParam(void);

	// 状態遷移の初期化
	void InitStateMachine(void);

	// アニメーションの初期化
	void InitAnimation(void);

	// 攻撃パターンの初期化
	void InitAttackPattern(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

	// ダメージ処理
	void Damage(void);

	// デバック用描画
	void DrawDebug(void);

	// 武器用
	VECTOR GetWeaponCenter(void);
	void UpdateWeapon(void);	// 武器の更新
	void UpdateWeaponCapsule();	// 武器カプセルの更新

	void InitEffect(void);

	// サウンド
	int sHandle_;
	bool isSound_;
	void InitSound(void);
};

