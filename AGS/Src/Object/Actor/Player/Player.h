#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
class AnimationController;
class Collider;
class Capsule;
class EnemyBoss;
class EnemyBossDummy;
class EnemyScarecrow;
class EnemyScarecrowDummy;
class PlayerWeapon;
class HpPotion;
class UltPotion;
class Book;
class Mist;

class Player : public ActorBase
{

public:
	// 音
	static constexpr int ATTACK_SE_VOL = 230;
	static constexpr int HIT_SE_VOL = 220;
	static constexpr int ROLLING_SE_VOL = 200;
	static constexpr int ULT_SE_VOL = 255;
	static constexpr int DAMAGE_SE_VOL = 240;
	static constexpr int ATTACK_SE_TIMING = 50;
	static constexpr int ATTACK2_SE_TIMING = 40;
	static constexpr int ATTACK3_SE_TIMING = 80;
	static constexpr int STRONG_ATTACK_SE_TIMING = 70;
	static constexpr int ROLLING_SE_TIMING = 65;

	// プレイヤー位置
	static constexpr VECTOR INIT_PLAYER_POS = { 0.0f, -100.0f, -400.0f };
	static constexpr VECTOR INIT_TOP_POS = { 0.0f, 190.0f, 0.0f };
	static constexpr VECTOR INIT_DOWN_POS = { 0.0f, 20.0f, 0.0f };
	static constexpr float PLAYER_RADIUS = 30.0f;

	// プレイヤー角度
	static constexpr VECTOR INIT_QUA_ROT_LOCAL = { 0.0f, 180.0f, 0.0f };

	// プレイヤーが死亡と見なされるまでの時間
	float PLAYER_DEAD_TIME = 150.0f;

	// スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.3f;

	// ジャンプ力
	static constexpr float POW_JUMP = 35.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	// HP関連
	static constexpr int MAX_HP = 100;
	static constexpr int HP_POS_X = 150;
	static constexpr int HP_POS_Y = 60;
	static constexpr int HP_SIZE_X = 450;
	static constexpr int HP_SIZE_Y = 15;
	static constexpr float HP_DECREASE_SPEED = 0.5f;
	static constexpr float HP_POTION_HEAL_AMOUNT = 15.0f;


	// 色
	static constexpr int HP_COLOR_TOP_R = 60;
	static constexpr int HP_COLOR_TOP_G = 200;
	static constexpr int HP_COLOR_TOP_B = 60;
	static constexpr int HP_COLOR_BOTTOM_R = 30;
	static constexpr int HP_COLOR_BOTTOM_G = 150;
	static constexpr int HP_COLOR_BOTTOM_B = 30;
	static constexpr int DAMAGE_COLOR_R = 180;
	static constexpr int DAMAGE_COLOR_G = 80;
	static constexpr int DAMAGE_COLOR_B = 80;
	static constexpr int HEAL_COLOR_R = 100;
	static constexpr int HEAL_COLOR_G = 200;
	static constexpr int HEAL_COLOR_B = 200;
	static constexpr int HP_OUTER_BORDER_COLOR_R = 80;
	static constexpr int HP_OUTER_BORDER_COLOR_G = 80;
	static constexpr int HP_OUTER_BORDER_COLOR_B = 80;
	static constexpr int HP_INNER_BORDER_COLOR_R = 160;
	static constexpr int HP_INNER_BORDER_COLOR_G = 160;
	static constexpr int HP_INNER_BORDER_COLOR_B = 160;
	static constexpr int HP_BG_COLOR_R = 30;
	static constexpr int HP_BG_COLOR_G = 30;
	static constexpr int HP_BG_COLOR_B = 30;
	static constexpr int OUTER_BORDER_OFFSET = 2;
	static constexpr int INNER_BORDER_OFFSET = 1;

	static constexpr int STAMINA_COLOR_TOP_R = 220;
	static constexpr int STAMINA_COLOR_TOP_G = 220;
	static constexpr int STAMINA_COLOR_TOP_B = 80;
	static constexpr int STAMINA_COLOR_BOTTOM_R = 180;
	static constexpr int STAMINA_COLOR_BOTTOM_G = 180;
	static constexpr int STAMINA_COLOR_BOTTOM_B = 50;
	static constexpr int STAMINA_OUTER_BORDER_COLOR_R = 60;
	static constexpr int STAMINA_OUTER_BORDER_COLOR_G = 60;
	static constexpr int STAMINA_OUTER_BORDER_COLOR_B = 60;
	static constexpr int STAMINA_INNER_BORDER_COLOR_R = 180;
	static constexpr int STAMINA_INNER_BORDER_COLOR_G = 180;
	static constexpr int STAMINA_INNER_BORDER_COLOR_B = 180;
	static constexpr int STAMINA_BG_COLOR_R = 30;
	static constexpr int STAMINA_BG_COLOR_G = 30;
	static constexpr int STAMINA_BG_COLOR_B = 30;

	// スタミナ関連
	static constexpr int STAMINA_POS_X = 150;
	static constexpr int STAMINA_POS_Y = 80;
	static constexpr int STAMINA_SIZE_X = 350;
	static constexpr int STAMINA_SIZE_Y = 15;
	static constexpr int MAX_STAMINA = 255;
	static constexpr int ROLLING_STAMINA = 50;
	static constexpr int STRONG_ATTACK_STAMINA = 30;
	static constexpr int STAMINA_RECOVERY_RATE = 1;
	static constexpr int STAMINA_DRAIN_RATE = 1;

	// 攻撃関連
	static constexpr int NORMAL_DAMAGE = 100;
	static constexpr int STRONG_DAMAGE = 130;
	static constexpr int SPECIAL_NORMAL_DAMAGE = 150;
	static constexpr int SPECIAL_STRONG_DAMAGE = 180;

	static constexpr float MAX_ATTACK_CNT = 60.0f;
	static constexpr float MAX_ATTACK_CNT_2 = 60.0f;
	static constexpr float MAX_ATTACK_CNT_3 = 110.0f;
	static constexpr float MAX_STRONG_ATTACK_CNT = 90.0f;

	static constexpr int ATTACK_DISABLE_TIME = 20;
	static constexpr int ATTACK2_DISABLE_TIME = 20;
	static constexpr int ATTACK3_DISABLE_TIME = 30;
	static constexpr int STRONG_ATTACK_DISABLE_TIME = 30;

	// ローリング関連
	static constexpr float MAX_ROLLING_CNT = 70.0f;
	static constexpr float ROLLING_MOVE = 10.0f;

	// ウルト関連
	static constexpr int ULT_CENTER_X = 80;
	static constexpr int ULT_CENTER_Y = 80;
	static constexpr int ULT_RADIUS = 50;
	static constexpr int FLASH_BASE_RADIUS = 30;
	static constexpr int FLASH_RING_SPACING = 10;
	static constexpr int FLASH_RADIUS_GROWTH = 2;
	static constexpr float ULT_MAX_VALUE = 1.0f;
	static constexpr float ULT_CHARGE_SPEED = 0.1f;
	static constexpr float ULT_DEPLETION_SPEED = 0.2f;
	static constexpr float MAGIC_CIRCLE_ROTATE_SPEED = 0.1f;
	static constexpr float FULL_CIRCLE_DEG = 360.0f;
	static constexpr float ULT_DRAW_SCALE = 0.15f;
	static constexpr int ULT_BRIGHTEN_ALPHA = 220;
	static constexpr int ULT_SOFT_PULSE_ALPHA = 40;
	static constexpr int ULT_SEGMENT_COUNT = 100;
	static constexpr float ULT_START_ANGLE = 90.0f;
	static constexpr int ULT_FLASH_DURATION = 40;
	static constexpr int ULT_RAY_COUNT = 12;
	static constexpr int ULT_RAY_THICKNESS = 3;
	static constexpr int FLASH_ALPHA_DECAY = 5;
	static constexpr int FLASH_ROTATE_SPEED = 10;
	static constexpr int RAY_START_ALPHA = 200;
	static constexpr int RAY_ALPHA_DECAY_PER_FRAME = 5;
	static constexpr int FLASH_COLOR_R = 250;
	static constexpr int FLASH_COLOR_G = 250;
	static constexpr int FLASH_COLOR_B = 255;
	static constexpr int RAY_COLOR_R = 180;
	static constexpr int RAY_COLOR_G = 240;
	static constexpr int RAY_COLOR_B = 255;
	static constexpr float ULT_EFFECT_SCALE = 100.0f;


	static constexpr int MAX_ALPHA = 255;


	// 武器
	static constexpr float WEAPON_RADIUS = 30.0f;
	static constexpr int WEAPON_TOP_INDEX = 3;
	static constexpr int WEAPON_DOWN_INDEX = 6;
	static constexpr VECTOR INIT_WEAPON_TOP_POS = { 0.0f, 0.0f, -100.0f };
	static constexpr float FORWARD_OFFSET = 20.0f;
	static constexpr float UP_OFFSET = 0.0f;
	static constexpr float RIGHT_OFFSET = 5.0f;
	static constexpr float EFFECT_HIT_SCALE = 25.0f;

	// 敵側当たり判定球の半径
	static constexpr float ENEMY_HIT_SPHERE_RADIUS = 1.0f;

	// アニメーション
	static constexpr float IDLE_ANIM_SPEED = 20.0f;
	static constexpr float RUN_ANIM_SPEED = 20.0f;
	static constexpr float FAST_RUN_ANIM_SPEED = 20.0f;
	static constexpr float JUMP_ANIM_SPEED = 60.0f;
	static constexpr float WARP_PAUSE_ANIM_SPEED = 60.0f;
	static constexpr float FLY_ANIM_SPEED = 60.0f;
	static constexpr float FALLING_ANIM_SPEED = 80.0f;
	static constexpr float VICTORY_ANIM_SPEED = 60.0f;
	static constexpr float ATTACK_ANIM_SPEED = 40.0f;
	static constexpr float ATTACK2_ANIM_SPEED = 40.0f;
	static constexpr float ATTACK3_ANIM_SPEED = 40.0f;
	static constexpr float STRONG_ATTACK_ANIM_SPEED = 40.0f;
	static constexpr float STUN_ANIM_SPEED = 40.0f;
	static constexpr float ROLLING_ANIM_SPEED = 30.0f;
	static constexpr float DEAD_ANIM_SPEED = 30.0f;

	static constexpr float STUN_RECHARGE_THRESHOLD = -90.0f;
	static constexpr float STUN_DURATION = 180.0f;

	// パッド
	static constexpr int PAD_INPUT_THRESHOLD_XY = 500;
	static constexpr int PAD_INPUT_THRESHOLD_Z = -800;
	static constexpr int ATTACK_PAD_BUTTON_NUM = 5;
	static constexpr int POV_RIGHT = 9000;
	static constexpr int POV_LEFT = 27000;
	static constexpr int INVALID_POV = -1;
	static constexpr int PAD_VIBRATION_POWER = 40000;
	static constexpr int PAD_VIBRATION_DURATION_MS = 500;

	// マウス
	static constexpr int WHEEL_THRESHOLD = 3;

	// ライトニング
	static constexpr int LIGHTNING_UPDATE_FRAME = 60;
	static constexpr int LIGHTNING_START_FRAME = 40;
	static constexpr int LIGHTNING_DURATION_FRAME = 120;

	// 回転を発生させる角度（ラジアン）しきい値
	static constexpr double ROTATION_THRESHOLD_RAD = 0.1;

	// 上下の当たり判定線の長さ調整用
	static constexpr float GROUND_CHECK_DISTANCE = 10.0f;
	static constexpr float GROUND_CHECK_UP_OFFSET = GROUND_CHECK_DISTANCE * 2.0f;

	// 落下方向判定のしきい値
	static constexpr float GRAVITY_COLLISION_DOT_THRESHOLD = 0.9f;

	// 着地後の浮かせ補正
	static constexpr float LANDING_ADJUST_HEIGHT = 2.0f;

	// アニメーションの着地部分の開始・終了フレーム
	static constexpr float LANDING_ANIM_START_FRAME = 29.0f;
	static constexpr float LANDING_ANIM_END_FRAME = 45.0f;

	// カプセルがポリゴンに埋まった際に、法線方向に押し戻しを試行する最大回数
	static constexpr int MAX_COLLISION_ADJUST_COUNT = 10;

	// 押し戻す距離
	static constexpr float COLLISION_PUSHBACK_AMOUNT = 1.0f;

	// 何回まで衝突回避を試みるか
	static constexpr int MAX_COLLISION_RESOLVE_ATTEMPTS = 12;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		ATTACK,
		ATTACK2,
		ATTACK3,
		STRONG_ATTACK,
		STUN,
		ROLLING,
		DEAD,
	};

	// アイテム
	enum class ITEM_TYPE
	{
		HP_POTION,
		ULT_POTION,
		BOOK,
		NUM
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const std::shared_ptr<Capsule> GetCapsule(void) const;

	void SetEnemy(std::shared_ptr<EnemyBoss> enemy);
	void SetEnemyDummy(std::shared_ptr<EnemyBossDummy> enemyDummy);
	void SetEnemyScarecrow(std::shared_ptr<EnemyScarecrow> enemyScarecrow);
	void SetEnemyScarecrowDummy(std::shared_ptr<EnemyScarecrowDummy> enemyScarecrowDummy);
	void SetMist(std::shared_ptr<Mist> mist);

	bool IsHitAttackEnemyBoss(void);
	bool IsHitAttackEnemyScarecrow(void);
	int GetHp(void);
	int GetStamina(void);


	void Damage(int damage);

	bool IsDead(void);

	VECTOR GetMovePow(void);

	int GetID() const;


	// スタン
	void StunUpdate(void);

	// 衝突判定
	void CollisionEnemyBoss(void);
	void CollisionEnemyScarecrow(void);
	void CollisionMist(void);

	bool IsRun(void);
	float GetAttackCnt(void);
	float GetAttackCnt2(void);
	float GetAttackCnt3(void);
	float GetStrongAttackCnt(void);
	float GetRollingCnt(void);

	int GetAttack(void);

	void SetUltValue(float ultValue);

	bool IsLightning(void);
	void StopEffect(void);

	void DrawHp(float hp, int maxHp);
	void DrawStamina(float stamina, int staminaMax);
	void DrawItem(void);
	void DrawUlt();

	void TutorialUlt(void);

private:

	// 音
	int attackSe_;
	int hitSe_;
	int rollingSe_;
	int ultSe_;
	int damageSe_;

	// アニメーション
	AnimationController* animationController_;

	std::shared_ptr<EnemyBoss> enemy_;
	std::shared_ptr<EnemyBossDummy> enemyDummy_;
	std::shared_ptr<EnemyScarecrow> enemyScarecrow_;
	std::shared_ptr<EnemyScarecrowDummy> enemyScarecrowDummy_;
	std::shared_ptr<PlayerWeapon> weapon_;
	std::unique_ptr<HpPotion> hpPotion_;
	std::unique_ptr<UltPotion> ultPotion_;
	std::unique_ptr<Book> book_;
	std::shared_ptr<Mist> mist_;

	// 状態管理
	STATE state_;

	// アイテム状態
	ITEM_TYPE itemType_;

	// 移動スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// RUN状態かどうか
	bool isRun_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// ジャンプ量
	VECTOR jumpPow_;

	// ジャンプ判定
	bool isJump_;

	// ジャンプの入力受付時間
	float stepJump_;

	int attack_;

	// 攻撃のアニメーションカウント
	float attackCnt_;
	float attackCnt2_;
	bool isAttack2_;
	float attackCnt3_;
	bool isAttack3_;

	float strongAttackCnt_;

	float stunCnt_;

	float deadCnt_;

	float rollingCnt_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	bool canAttack_;

	// 丸影
	int imgShadow_;

	// 黒画像
	int imgBlack_;

	//武器のボーンインデックス
	int weaponBoneIndex_;

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

	float currentHp_;
	float displayHp_;
	float damageHp_;
	float healHp_;

	int stamina_;

	int id_;


	int ultFrameHandle_;
	int magicCircleHandle_;
	int runeHandle_;
	float ultValue_;
	bool isUltActive_;
	bool wasUltMaxed_;
	int flashTimer_;

	int lightningTimer_;
	bool isLightning_;

	void InitAnimation(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//アイテム変更
	void ChangeItem(ITEM_TYPE itemType);

	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);

	// 操作
	void ProcessMove(void);

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

	// 着地モーション終了
	bool IsEndLanding(void);

	// 武器の更新
	void UpdateRightWeapon(void);


	// アイテム
	void UpdateItem(void);
	// Hpポーション
	void UseHpPosion(void);
	// Ultポーション
	void UseUltPosion(void);
	// 魔法書
	void UseBook(void);

	VECTOR WeaponTopPos(VECTOR pos);
	VECTOR WeaponDownPos(VECTOR pos);

	void UpdateWeaponCapsule();

	void UpdateUlt(void);

	// エフェクト
	int effectHitPlayId_;
	int effectHitResId_;

	int effectUltPlayId_;
	int effectUltResId_;

	void InitEffect(void);
};
