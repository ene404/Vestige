#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../Common/AnimationController.h"
#include "../../Common/Capsule.h"
#include "../../Common/Collider.h"
#include "../Weapon/EnemyWeapon.h"
#include "../../Stage/Planet.h"
#include "../Player/EventPlayer.h"
#include "EventEnemy.h"

EventEnemy::EventEnemy(EventPlayer& player) 
	:
	player_(player),
	animationController_(nullptr),
	weapon_(nullptr),
	state_(STATE::NONE),
	speed_(0.0f),
	moveDir_({}),
	movePow_({}),
	movedPos_({}),
	playerRotY_({}),
	goalQuaRot_({}),
	stepRotTime_(0.0f),
	jumpPow_({}),
	isJump_(false),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	weaponBoneIndex_(0),
	wristIndex_(0),
	wristPos({}),
	handIndex_(0),
	handPos({}),
	weaponTopIndex_(0),
	weaponTopPos_({}),
	weaponDownIndex_(0),
	weaponDownPos_({}),
	sHandle_(-1),
	isSound_(false),
	timer_(0.0f),
	effectRoarFPlayId_(-1),
	effectRoarFResId_(-1),
	IsEffect_(false)
{
}

EventEnemy::~EventEnemy()
{
}

void EventEnemy::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_BOSS));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	
	LookAt(player_.GetTransform().pos);

	transform_.Update();

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_POS_DOWN);
	capsule_->SetRadius(CAPUSULE_RADIUS);

	weapon_ = std::make_shared<EnemyWeapon>();
	weapon_->Init();

	// �t���[���擾
	wristIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHand");
	handIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHandMiddle1");

	weaponTopIndex_ = WEAPON_TOP_INDEX;
	weaponDownIndex_ = WEAPON_DOWN_INDEX;

	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	weaponTopPos_ = WeaponTopPos(WEAPON_TOP_POS);

	UpdateRightWeapon();
	UpdateWeapon();

	//eSHandle
	sHandle_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::ATTACK_SE).handleId_;
	ChangeVolumeSoundMem(SOUND_VALUE, sHandle_);

	isSound_ = false;
	timer_ = false;

	IsEffect_ = false;

	// �������
	ChangeState(STATE::IDLE);
	SceneManager::GetInstance().GetCamera()->ChangeGameCamera(Camera::GAME_CAMERA::MOUSE);
}

void EventEnemy::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case EventEnemy::STATE::NONE:
		UpdateNone();
		break;
	case EventEnemy::STATE::IDLE:
		UpdateIdle();
		break;
	case EventEnemy::STATE::WALK:
		UpdateWalk();
		break;
	case EventEnemy::STATE::LOOK:
		UpdateLook();
		break;
	case EventEnemy::STATE::STANDBY:
		UpdateStandBy();
		break;
	default:
		break;
	}

	// ���f������X�V
	transform_.Update();

	// �A�j���[�V�����Đ�
	animationController_->Update();

	UpdateRightWeapon();
	UpdateWeapon();
}

void EventEnemy::Draw(void)
{
	weapon_->Draw();

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
}

void EventEnemy::SetPosition(VECTOR pos)
{
	transform_.pos = pos;
}

void EventEnemy::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void EventEnemy::ClearCollider(void)
{
	colliders_.clear();
}

const std::shared_ptr<Capsule> EventEnemy::GetCapsule(void) const
{
	return std::shared_ptr<Capsule>();
}

int EventEnemy::GetEffectRoarFPlayId(void)
{
	return effectRoarFPlayId_;
}

void EventEnemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Enemy/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", IDLE_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::WALK, path + "Walk Forward.mv1", WALK_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::LOOK, path + "Idle.mv1", LOOK_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::STANDBY, path + "Stable Sword Inward Slash.mv1", STANDBY_ANIM_SPEED);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void EventEnemy::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case EventEnemy::STATE::NONE:
		ChangeStateNone();
		break;
	case EventEnemy::STATE::IDLE:
		ChangeStateIdle();
		break;
	case EventEnemy::STATE::WALK:
		ChangeStateWalk();
		break;
	case EventEnemy::STATE::LOOK:
		ChangeStateLook();
		break;
	case EventEnemy::STATE::STANDBY:
		ChangeStateStandBy();
		break;
	default:
		break;
	}
}

void EventEnemy::ChangeStateNone(void)
{
}

void EventEnemy::ChangeStateIdle(void)
{
	animationController_->Play((int)ANIM_TYPE::IDLE, true, ANIM_BLEND_RATE);
}

void EventEnemy::ChangeStateWalk(void)
{
	animationController_->Play((int)ANIM_TYPE::WALK, true, ANIM_BLEND_RATE);
}

void EventEnemy::ChangeStateLook(void)
{
	animationController_->Play((int)ANIM_TYPE::LOOK, true, ANIM_BLEND_RATE);
}

void EventEnemy::ChangeStateStandBy(void)
{
	animationController_->Play((int)ANIM_TYPE::STANDBY, false, ANIM_BLEND_RATE);
}

void EventEnemy::UpdateNone(void)
{
	// ����

	// �ړ������ɉ�������]
	Rotate();
	LookAt(player_.GetTransform().pos);

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventEnemy::UpdateIdle(void)
{
	// ����

	// �ړ������ɉ�������]
	Rotate();
	LookAt(player_.GetTransform().pos);

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventEnemy::UpdateWalk(void)
{
	// ����

	// �ړ������ɉ�������]
	Rotate();
	LookAt(player_.GetTransform().pos);

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventEnemy::UpdateLook(void)
{
	// ����

	// �ړ������ɉ�������]
	Rotate();
	LookAt(player_.GetTransform().pos);

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventEnemy::UpdateStandBy(void)
{
	float dt = SceneManager::GetInstance().GetDeltaTime();
	timer_ += dt;

	if (!IsEffect_)
	{
		// �G�t�F�N�g�Đ����J�n���A�C���X�^���X�n���h�����擾
		effectRoarFPlayId_ = PlayEffekseer3DEffect(effectRoarFResId_);
		SetPosPlayingEffekseer3DEffect(effectRoarFPlayId_, transform_.pos.x, transform_.pos.y + POS_Y_ADJUSTMENT, transform_.pos.z);
		IsEffect_ = true;
	}

	if (timer_ >= TIME_LIMIT)
	{
		if (!isSound_)
		{
			PlaySoundMem(sHandle_, DX_PLAYTYPE_BACK, true);
			isSound_ = true;
		}
	}
}

void EventEnemy::SetGoalRotate(double rotRad)
{
	VECTOR angles = SceneManager::GetInstance().GetCamera()->GetAngles();
	double cameraRadY = AsoUtility::Deg2RadD(angles.y);


	Quaternion axis = Quaternion::AngleAxis(cameraRadY + rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > THRESHOLD_ROT)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void EventEnemy::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void EventEnemy::LookAt(VECTOR pos)
{
	VECTOR BossPos = transform_.pos;
	VECTOR targetPosXZ = pos;
	targetPosXZ.y = BossPos.y;//y������(������]�̂�)

	VECTOR dir = VSub(targetPosXZ, BossPos);
	float length = AsoUtility::MagnitudeF(dir);
	if (length < LENGTH_LIMIT)return;

	dir = VNorm(dir);
	Quaternion lookRot = Quaternion::LookRotation(dir);

	const float slerpRotio = SLEARP_RATIO;
	// �X���[�Y�ɉ�](���ʐ��`���)
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, lookRot, slerpRotio);
}

void EventEnemy::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;
}

void EventEnemy::CollisionGravity(void)
{
	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = GRAVITY_CHECK_POW;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow * -1.0f));
	for (const auto c : colliders_)
	{

		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > FALLINT_DOT_THRESHOLD)
		{
			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			// �W�����v���Z�b�g
			jumpPow_ = AsoUtility::VECTOR_ZERO;

			isJump_ = false;

		}

	}
}

void EventEnemy::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
	for (const auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					// �J�v�Z�����ړ�������
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
	}
}

void EventEnemy::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}
}

bool EventEnemy::IsEndLanding(ANIM_TYPE anim)
{
	if (state_ == STATE::WALK)return false;
	if (animationController_->GetPlayType() != (int)anim)return false;
	return animationController_->IsEnd();
}

VECTOR EventEnemy::WeaponTopPos(VECTOR pos)
{
	return VAdd(weaponTopPos_, pos);
}

void EventEnemy::UpdateRightWeapon(void)
{
	// ��� �� �� �̌����x�N�g��
	wristPos = MV1GetFramePosition(transform_.modelId, wristIndex_);
	handPos = MV1GetFramePosition(transform_.modelId, handIndex_);

	VECTOR dir = VSub(handPos, wristPos); // �����x�N�g��
	dir = VNorm(dir); // ���K�����ĒP�ʃx�N�g���ɂ���

	// ��������p�x���Z�o
	float yaw = atan2f(dir.x, dir.z);
	float pitch = atan2f(-dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z));
	float roll = WEAPON_ROLL;

	// ���̉�]��ݒ�
	MV1SetRotationXYZ(weapon_->GetModelHandle(), VGet(pitch, yaw, roll));

	// �ʒu������
	float forwardOffset = WEAPON_FORWARD_OFFSET;
	float upOffset = WEAPON_UP_OFFSET;
	float rightOffset = WEAPON_RIGHT_OFFSET;

	// �E�x�N�g���Ə�x�N�g�����ȈՓI�ɋ��߂�i���S�ȉ�]�s�񂪂Ȃ��O��j
	VECTOR upVec = WEAPON_UPVEC; // �����
	VECTOR rightVec = VCross(upVec, dir);  // ��̌����Əォ��E���������
	rightVec = VNorm(rightVec); // ���K��

	// ������Ē����i��̂Ђ˂�͍l�����Ȃ��j
	upVec = VCross(dir, rightVec);
	upVec = VNorm(upVec);

	// �I�t�Z�b�g����
	VECTOR offsetPos = handPos;
	offsetPos = VAdd(offsetPos, VScale(dir, forwardOffset));   // �O��
	offsetPos = VAdd(offsetPos, VScale(upVec, upOffset));      // ��
	offsetPos = VAdd(offsetPos, VScale(rightVec, rightOffset));// �E

	// ���̈ʒu��ݒ�
	MV1SetPosition(weapon_->GetModelHandle(), offsetPos);
}

void EventEnemy::UpdateWeapon()
{
	// ��� �� �� �̌����x�N�g��
	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	VECTOR dir = VSub(weaponTopPos_, weaponDownPos_); // �����x�N�g��
	dir = VNorm(dir); // ���K�����ĒP�ʃx�N�g���ɂ���

	// �ʒu������
	float forwardOffset = WEAPON_CAPSULE_FORWARD_OFFSET;
	float upOffset = WEAPON_CAPSULE_UP_OFFSET;
	float rightOffset = WEAPON_CAPSULE_RIGHT_OFFSET;

	// �E�x�N�g���Ə�x�N�g�����ȈՓI�ɋ��߂�i���S�ȉ�]�s�񂪂Ȃ��O��j
	VECTOR upVec = WEAPON_CAPSULE_UPVEC; // �����
	VECTOR rightVec = VCross(upVec, dir);  // ��̌����Əォ��E���������
	rightVec = VNorm(rightVec); // ���K��

	// ������Ē����i��̂Ђ˂�͍l�����Ȃ��j
	upVec = VCross(dir, rightVec);
	upVec = VNorm(upVec);

	// �I�t�Z�b�g����
	VECTOR offsetPos = handPos;
	offsetPos = VAdd(offsetPos, VScale(dir, forwardOffset));   // �O��
	offsetPos = VAdd(offsetPos, VScale(upVec, upOffset));      // ��
	offsetPos = VAdd(offsetPos, VScale(rightVec, rightOffset));// �E

	// ���̈ʒu��ݒ�
	MV1SetPosition(weapon_->GetModelHandle(), offsetPos);
}

void EventEnemy::InitEffect(void)
{
	// �G�t�F�N�g
	effectRoarFResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::ROAR_01).handleId_;

	// �傫��
	float SCALE = EFFECT_SIZE;
	SetScalePlayingEffekseer3DEffect(effectRoarFPlayId_, SCALE , SCALE, SCALE);

}
