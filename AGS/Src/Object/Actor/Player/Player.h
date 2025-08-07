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
	// ��
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

	// �v���C���[�ʒu
	static constexpr VECTOR INIT_PLAYER_POS = { 0.0f, -100.0f, -400.0f };
	static constexpr VECTOR INIT_TOP_POS = { 0.0f, 190.0f, 0.0f };
	static constexpr VECTOR INIT_DOWN_POS = { 0.0f, 20.0f, 0.0f };
	static constexpr float PLAYER_RADIUS = 30.0f;

	// �v���C���[�p�x
	static constexpr VECTOR INIT_QUA_ROT_LOCAL = { 0.0f, 180.0f, 0.0f };

	// �v���C���[�����S�ƌ��Ȃ����܂ł̎���
	float PLAYER_DEAD_TIME = 150.0f;

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.3f;

	// �W�����v��
	static constexpr float POW_JUMP = 35.0f;

	// �W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	// HP�֘A
	static constexpr int MAX_HP = 100;
	static constexpr int HP_POS_X = 150;
	static constexpr int HP_POS_Y = 60;
	static constexpr int HP_SIZE_X = 450;
	static constexpr int HP_SIZE_Y = 15;
	static constexpr float HP_DECREASE_SPEED = 0.5f;
	static constexpr float HP_POTION_HEAL_AMOUNT = 15.0f;


	// �F
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

	// �X�^�~�i�֘A
	static constexpr int STAMINA_POS_X = 150;
	static constexpr int STAMINA_POS_Y = 80;
	static constexpr int STAMINA_SIZE_X = 350;
	static constexpr int STAMINA_SIZE_Y = 15;
	static constexpr int MAX_STAMINA = 255;
	static constexpr int ROLLING_STAMINA = 50;
	static constexpr int STRONG_ATTACK_STAMINA = 30;
	static constexpr int STAMINA_RECOVERY_RATE = 1;
	static constexpr int STAMINA_DRAIN_RATE = 1;

	// �U���֘A
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

	// ���[�����O�֘A
	static constexpr float MAX_ROLLING_CNT = 70.0f;
	static constexpr float ROLLING_MOVE = 10.0f;

	// �E���g�֘A
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


	// ����
	static constexpr float WEAPON_RADIUS = 30.0f;
	static constexpr int WEAPON_TOP_INDEX = 3;
	static constexpr int WEAPON_DOWN_INDEX = 6;
	static constexpr VECTOR INIT_WEAPON_TOP_POS = { 0.0f, 0.0f, -100.0f };
	static constexpr float FORWARD_OFFSET = 20.0f;
	static constexpr float UP_OFFSET = 0.0f;
	static constexpr float RIGHT_OFFSET = 5.0f;
	static constexpr float EFFECT_HIT_SCALE = 25.0f;

	// �G�������蔻�苅�̔��a
	static constexpr float ENEMY_HIT_SPHERE_RADIUS = 1.0f;

	// �A�j���[�V����
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

	// �p�b�h
	static constexpr int PAD_INPUT_THRESHOLD_XY = 500;
	static constexpr int PAD_INPUT_THRESHOLD_Z = -800;
	static constexpr int ATTACK_PAD_BUTTON_NUM = 5;
	static constexpr int POV_RIGHT = 9000;
	static constexpr int POV_LEFT = 27000;
	static constexpr int INVALID_POV = -1;
	static constexpr int PAD_VIBRATION_POWER = 40000;
	static constexpr int PAD_VIBRATION_DURATION_MS = 500;

	// �}�E�X
	static constexpr int WHEEL_THRESHOLD = 3;

	// ���C�g�j���O
	static constexpr int LIGHTNING_UPDATE_FRAME = 60;
	static constexpr int LIGHTNING_START_FRAME = 40;
	static constexpr int LIGHTNING_DURATION_FRAME = 120;

	// ��]�𔭐�������p�x�i���W�A���j�������l
	static constexpr double ROTATION_THRESHOLD_RAD = 0.1;

	// �㉺�̓����蔻����̒��������p
	static constexpr float GROUND_CHECK_DISTANCE = 10.0f;
	static constexpr float GROUND_CHECK_UP_OFFSET = GROUND_CHECK_DISTANCE * 2.0f;

	// ������������̂������l
	static constexpr float GRAVITY_COLLISION_DOT_THRESHOLD = 0.9f;

	// ���n��̕������␳
	static constexpr float LANDING_ADJUST_HEIGHT = 2.0f;

	// �A�j���[�V�����̒��n�����̊J�n�E�I���t���[��
	static constexpr float LANDING_ANIM_START_FRAME = 29.0f;
	static constexpr float LANDING_ANIM_END_FRAME = 45.0f;

	// �J�v�Z�����|���S���ɖ��܂����ۂɁA�@�������ɉ����߂������s����ő��
	static constexpr int MAX_COLLISION_ADJUST_COUNT = 10;

	// �����߂�����
	static constexpr float COLLISION_PUSHBACK_AMOUNT = 1.0f;

	// ����܂ŏՓˉ�������݂邩
	static constexpr int MAX_COLLISION_RESOLVE_ATTEMPTS = 12;

	// ���
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

	// �A�j���[�V�������
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

	// �A�C�e��
	enum class ITEM_TYPE
	{
		HP_POTION,
		ULT_POTION,
		BOOK,
		NUM
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
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


	// �X�^��
	void StunUpdate(void);

	// �Փ˔���
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

	// ��
	int attackSe_;
	int hitSe_;
	int rollingSe_;
	int ultSe_;
	int damageSe_;

	// �A�j���[�V����
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

	// ��ԊǗ�
	STATE state_;

	// �A�C�e�����
	ITEM_TYPE itemType_;

	// �ړ��X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// RUN��Ԃ��ǂ���
	bool isRun_;

	// ��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v����
	bool isJump_;

	// �W�����v�̓��͎�t����
	float stepJump_;

	int attack_;

	// �U���̃A�j���[�V�����J�E���g
	float attackCnt_;
	float attackCnt2_;
	bool isAttack2_;
	float attackCnt3_;
	bool isAttack3_;

	float strongAttackCnt_;

	float stunCnt_;

	float deadCnt_;

	float rollingCnt_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	bool canAttack_;

	// �ۉe
	int imgShadow_;

	// ���摜
	int imgBlack_;

	//����̃{�[���C���f�b�N�X
	int weaponBoneIndex_;

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

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//�A�C�e���ύX
	void ChangeItem(ITEM_TYPE itemType);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);

	// ����
	void ProcessMove(void);

	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// ���n���[�V�����I��
	bool IsEndLanding(void);

	// ����̍X�V
	void UpdateRightWeapon(void);


	// �A�C�e��
	void UpdateItem(void);
	// Hp�|�[�V����
	void UseHpPosion(void);
	// Ult�|�[�V����
	void UseUltPosion(void);
	// ���@��
	void UseBook(void);

	VECTOR WeaponTopPos(VECTOR pos);
	VECTOR WeaponDownPos(VECTOR pos);

	void UpdateWeaponCapsule();

	void UpdateUlt(void);

	// �G�t�F�N�g
	int effectHitPlayId_;
	int effectHitResId_;

	int effectUltPlayId_;
	int effectUltResId_;

	void InitEffect(void);
};
