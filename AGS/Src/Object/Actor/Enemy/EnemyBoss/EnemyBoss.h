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

	// �������W
	static constexpr VECTOR INIT_POS = { 0.0f, -30.0f, 900.0f };

	// �T�C�Y
	static constexpr float SIZE = 1.3f;

	// �������[�J����]

	static constexpr float INIT_QUAROT_LOCAL_X = 0.0f;
	static constexpr float INIT_QUAROT_LOCAL_Y = 180.0f;
	static constexpr float INIT_QUAROT_LOCAL_Z = 0.0f;

	// �ő�HP
	static constexpr int MAX_HP_VALUE = 2000;

	// �������A�j���[�V����
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

	// �d�͊֘A
	static constexpr float GRAVITY_CHECK_POW = 10.0f;
	static constexpr float FALLINT_DOT_THRESHOLD = 0.9f;

	// �J�v�Z���֘A
	static constexpr float CAPSULE_TOP_LOCAL_POS_Y = 150.0f;
	static constexpr float CAPSULE_DOWN_LOCAL_POS_Y = 30.0f;
	static constexpr float CAPSULE_RADIUS = 30.0f;
	static constexpr int CAPSULE_LIMIT_COUNT = 10;

	// �_���[�W�֘A
	static constexpr float DECREASE_SPEED = 2.0f;

	// ����֘A
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

	// �X�^���֘A
	static constexpr float STUN_TIME = 5.0f;
	static constexpr int LIGHTNING_DAMAGE = 300;

	// �G�t�F�N�g�֘A
	// �o�t
	static constexpr float BUFF_EFFECT_SIZE = 100.0f;
	static constexpr int BUFF_EFFECT_SOUND = 255;

	// �X�^��
	static constexpr float STUN_EFFECT_SIZE = 50.0f;

	// ���S�֘A
	static constexpr float DEAD_INTERVAL_TIME = 5.0f;

	// ����̃N�[���_�E���Ԋu
	static constexpr float STEP_COOLDOWN_DURATON = 5.0f;

	// HP�o�[�֘A
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

	// �����̐���
	static constexpr float LENGTH_LIMIT = 0.001f;

	// ���ʕ�Ԃ̑��x
	static constexpr float SLERP_ROTIO = 0.05f;

	// �ړ����x
	static constexpr float MOVE_SPEED = 5.0f;
	

	// ���
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


	// �e�N���X�̃Q�b�^�[
	//------------------
	std::shared_ptr<EnemyStateMachine> GetStateMachine() const;
	std::shared_ptr<EnemyBossAnimation> GetAnimation() const;
	std::shared_ptr<EnemyBossCombatSystem> GetAttackStateSystem() const;
	std::shared_ptr<EnemyBossCombatController> GetAttackStateController() const;
	std::shared_ptr<EnemyBossAttackStateMachine> GetAttackStateMachine() const;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	//-------------------------------------
	void SetPlayerDummy(std::shared_ptr<PlayerDummy> playerDummy);

	// ���Ƃ̏Փ˔���p�����擾����
	void SetMist(std::shared_ptr<Mist> mist);

	// �Փ˗p�J�v�Z���̎擾-----------------------
	const std::shared_ptr<Capsule> GetCapsule(void) const;

	int GetHp(void) const;

	// ----------------------------------------
	STATE GetCurrentStateId() const;

	// ���C���X�e�[�g�ύX�C���^�[�t�F�[�X
	void ChangeState(STATE newState);
	void ChangeAttack(ATK_STATE newAttack);

	// EChaseState����EAttackState�֓n�����߂́u���Ɏ��s����U���v
	void SetNextAttackType(ATK_STATE type);
	ATK_STATE GetNextAttackType() const;

	// �v���C���[�֘A
	Player& GetPlayer();
	float GetPlayerDistanceXZ() const;
	VECTOR GetToPlayer();


	// -----------------------------
	// �����͈������Ɏ��g�̍��W������ΕK�v�Ȃ����A���S���̊ϓ_����l���Ă�������
	void SetPosition(VECTOR pos);
	Transform& GetTransformMutable();

	// �{�X�̃X�^����Ԋ֘A
	// �{�X���X�^�����Ԃ�ݒ肷��
	void SetStunned(float duration);

	bool IsStunned(void) const;

	//--------------------
	float  GetStunTimer(void) const;

	// �{�X�̍U���̓A�b�v���
	void SetAttackBuff(float duration, float muktiplier);
	void RemoveAttackBuff(void);
	float GetCurrentAttackMultiplier() const;

	// �{�X�̎��S��Ԋ֘A
	bool IsDead() const;
	void SetIsDead(bool flag);
	void SetDead();
	float GetDeadTimer() const;
	void OnDeathAnimationFinished(void);

	// Stun�G�t�F�N�g�֘A
	void StartStunEffect(void);
	void StopStunEffect(void);
	bool IsStunEffectActive(void);

	// SetUp�G�t�F�N�g�֘A
	void StartSetupEffect(void);
	void StopSetupEffect(void);
	bool IsSetupEffectActive(void);

	// ����֘A
	VECTOR GetTopPos();
	VECTOR GetDownPos();

	// HP�֘A
	int GetMaxHp() const;
	float GetHpRatio() const;

	// HP�o�[�̕`��
	void DrawHpBar(void);

	void LookAt(VECTOR pos);

	void MoveToward(const VECTOR& target);

	void DrawCircleOnGround(VECTOR center, float radius, int segment, int color);

	// �v���C���[���{�X�̐��ʂɂ��邩�𔻒肷��
	bool IsPlayerInFront(float viewAngleDeg, float maxDistance);

	// �U�������t���O
	bool hasUsedRoarBelow50Percent_ = false;
	bool hasUsedExplosionBelow50Percent_ = false;

	// SETUP�U����HP4���ȉ��ň�x�g�p���ꂽ���ǂ����̃t���O
	bool hasUsedSetupBelow40Percent_ = false;

	void CollisionMist(void);

	float GetStepCooldown(void);

	void SetStepCooldown(float duration);

private:

	// �v���C���[
	Player& player_;

	std::shared_ptr<PlayerDummy> playerDummy_;
	std::shared_ptr<Mist> mist_;

	// �A�j���[�V����
	std::shared_ptr<EnemyWeapon> weapon_;

	// ��ԑJ��
	std::shared_ptr<EnemyStateMachine> stateMachine_;

	// �����ڂƓ���
	std::shared_ptr<EnemyBossAnimation> animation_;

	// �퓬����
	std::shared_ptr<EnemyBossCombatSystem> combat_;
	std::shared_ptr<EnemyBossCombatController> combatController_;
	std::shared_ptr<EnemyBossAttackStateMachine> attackFSM_;

	// �U�����Ɉ��ł��������v���C���[ID���L�^����
	std::unordered_set<int> hitPlayers_;	

	int hp_;				// �{�X��HP
	int maxHp_;				//�@HP�̍ő�l
	
	// EChaseState����EAttackState�֓n�����̍U���^�C�v
	ATK_STATE nextAttackType_;

	// �ړ��p
	VECTOR moveDir_;
	VECTOR movedPos_;
	VECTOR movePow_;

	VECTOR jumpPow_;
	bool isJump_;

	// �v���C���[�Ƃ̋���
	VECTOR toPlayer_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �󂯂��_���[�W�̕`��p
	int damageHp_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// ���̃t���[��
	int wristIndex_;
	VECTOR wristPos;

	// ��̃t���[��
	int handIndex_;
	VECTOR handPos;

	// ����̃g�b�v
	int weaponTopIndex_;
	VECTOR weaponTopPos_;

	// ����̉�
	int weaponDownIndex_;
	VECTOR weaponDownPos_;

	// ��̃J�v�Z��
	VECTOR capsuleDir_;
	VECTOR capsuleCenter_;

	// ����p�����蔼�a
	float radius_;

	// �{�X�̎��ȃX�^���Ǘ�
	bool isStunned_;
	float stunTimer_;

	// �X�^���G�t�F�N�g�֘A
	int effectStunPlayId_;
	int effectStunResId_;
	bool isStunEffectActive_;

	// SetUp�G�t�F�N�g�֘A
	int effectBuffPlayId_;
	int effectBuffResId_;
	bool isSetUpEffectActive_; // SetUp�G�t�F�N�g���A�N�e�B�u���ǂ���

	// �U���̓A�b�v�Ǘ�(SETUP�p)
	float currentAttackMultiplier_;	// ��{�U����
	float attackBuffTimer_;			// �U���o�t�c�莞��

	bool isDead_;			// ���S���Ftrue��Ԃ�
	float deadTimer_;		// ���S���̃^�C�}�[

	float stepCooldown_;	// Step�N�[���_�E��

	// ���f�����̏�����
	void InitModel(void);

	//�p�����[�^�̏�����
	void InitParam(void);

	// ��ԑJ�ڂ̏�����
	void InitStateMachine(void);

	// �A�j���[�V�����̏�����
	void InitAnimation(void);

	// �U���p�^�[���̏�����
	void InitAttackPattern(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// �_���[�W����
	void Damage(void);

	// �f�o�b�N�p�`��
	void DrawDebug(void);

	// ����p
	VECTOR GetWeaponCenter(void);
	void UpdateWeapon(void);	// ����̍X�V
	void UpdateWeaponCapsule();	// ����J�v�Z���̍X�V

	void InitEffect(void);

	// �T�E���h
	int sHandle_;
	bool isSound_;
	void InitSound(void);
};

