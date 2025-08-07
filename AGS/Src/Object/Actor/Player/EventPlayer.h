#pragma once
#include <vector>
#include "../ActorBase.h"
class AnimationController;
class Collider;
class Capsule;
class PlayerWeapon;

class EventPlayer : public ActorBase
{
public:

	// �������W
	static constexpr VECTOR INIT_POS = { 0.0f, -100.0f, 0.0f };

	// �������[�J����]
	static constexpr float INIT_QUAROT_LOCAL_X = 0.0f;
	static constexpr float INIT_QUAROT_LOCAL_Y = 180.0f;
	static constexpr float INIT_QUAROT_LOCAL_Z = 0.0f;

	// �J�v�Z���֘A
	static constexpr VECTOR CAPSULE_POS_TOP = { 0.0f, 190.0f, 0.0f };
	static constexpr VECTOR CAPSULE_POS_DOWN = { 0.0f, 20.0f, 0.0f };
	static constexpr float CAPUSULE_RADIUS = 20.0f;

	// ����֘A
	static constexpr int WEAPON_TOP_INDEX = 3;
	static constexpr int WEAPON_DOWN_INDEX = 6;
	static constexpr VECTOR WEAPON_TOP_POS = { 0.0f, 0.0f, -100.0f };
	static constexpr VECTOR WEAPON_DOWN_POS = { -20.0f, 0.0f, 0.0f };
	static constexpr float WEAPON_ROLL = 0.0f;
	static constexpr float WEAPON_FORWARD_OFFSET = -5.0f;
	static constexpr float WEAPON_UP_OFFSET = -20.0f;
	static constexpr float WEAPON_RIGHT_OFFSET = -5.0f;
	static constexpr VECTOR WEAPON_UPVEC = { 0.0f,1.0f,0.0f };
	static constexpr float WEAPON_CAPSULE_FORWARD_OFFSET = 20.0f;
	static constexpr float WEAPON_CAPSULE_UP_OFFSET = 0.0f;
	static constexpr float WEAPON_CAPSULE_RIGHT_OFFSET = 5.0f;
	static constexpr VECTOR WEAPON_CAPSULE_UPVEC = { 0.0f,1.0f,0.0f };

	// ���֘A
	static constexpr int SOUND_VALUE = 180;

	// �A�j���[�V�����֘A
	static constexpr float IDLE_ANIM_SPEED = 20.0f;
	static constexpr float WALK_ANIM_SPEED = 30.0f;
	static constexpr float LOOK_ANIM_SPEED = 34.0f;
	static constexpr float STANDBY_ANIM_SPEED = 20.0f;
	static constexpr float ANIM_BLEND_RATE = 5.0f;

	// �ړ����x
	static constexpr float MOVE_SPEED = 1.8f;

	// �G�t�F�N�g�֘A
	static constexpr float EFFECT_SIZE = 25.0f;
	static constexpr float POS_Y_ADJUSTMENT = 100.0f;
	static constexpr float TIME_LIMIT = 0.85f;

	// ��]��臒l
	static constexpr double THRESHOLD_ROT = 0.1;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// �����̐���
	static constexpr float LENGTH_LIMIT = 0.001f;

	// ���ʉ�]�䗦
	static constexpr float SLEARP_RATIO = 0.05f;

	// �d�͊֘A
	static constexpr float GRAVITY_CHECK_POW = 10.0f;
	static constexpr float FALLINT_DOT_THRESHOLD = 0.9f;

	enum class STATE {
		NONE,
		IDLE,
		WALK,
		LOOK,
		STANDBY
	};

	enum class ANIM_TYPE {
		NONE,
		IDLE,
		WALK,
		LOOK,
		STANDBY
	};

	EventPlayer();
	~EventPlayer();

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	void SetPosition(VECTOR pos);
	void SetGoalRotate(double rotRad);

	void ChangeState(STATE state);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const std::shared_ptr<Capsule> GetCapsule(void) const;

	ANIM_TYPE GetAnimType();

	// ���n���[�V�����I��
	bool IsEndLanding(ANIM_TYPE anim);

private:
	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	// ����
	std::shared_ptr<PlayerWeapon> weapon_;

	// ���
	STATE state_;
	ANIM_TYPE anim_;

	// �ړ��X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v����
	bool isJump_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

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

	// �A�j���[�V�����̏�����
	void InitAnimation(void);

	// ��ԑJ��
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateWalk(void);
	void ChangeStateLook(void);
	void ChangeStateStandBy(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateLook(void);
	void UpdateStandBy(void);

	// ��]
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	VECTOR WeaponTopPos(VECTOR pos);
	VECTOR WeaponDownPos(VECTOR pos);

	// ����̍X�V
	void UpdateRightWeapon(void);

	void UpdateWeapon();

	int sHandle_;
	bool isSound_;
};

