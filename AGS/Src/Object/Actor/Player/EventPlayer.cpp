#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../Common/AnimationController.h"
#include "../../Common/Capsule.h"
#include "../../Common/Collider.h"
#include "../Weapon/PlayerWeapon.h"
#include "../../Stage/Planet.h"
#include "EventPlayer.h"

EventPlayer::EventPlayer()
{
	animationController_ = nullptr;
	state_ = STATE::NONE;
	anim_ = ANIM_TYPE::NONE;

	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	capsule_ = nullptr;

}

EventPlayer::~EventPlayer()
{
}

void EventPlayer::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ INIT_QUAROT_LOCAL_X, 
							AsoUtility::Deg2RadF(INIT_QUAROT_LOCAL_Y),
							INIT_QUAROT_LOCAL_Z });
	transform_.Update();

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	sHandle_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::WALK_BGM).handleId_;
	ChangeVolumeSoundMem(SOUND_VALUE, sHandle_);

	isSound_ = false;

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_POS_DOWN);
	capsule_->SetRadius(CAPUSULE_RADIUS);

	weapon_ = std::make_shared<PlayerWeapon>();
	weapon_->Init();

	// �t���[���擾
	wristIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHand");
	handIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHandMiddle1");

	weaponTopIndex_ = WEAPON_TOP_INDEX;
	weaponDownIndex_ = WEAPON_DOWN_INDEX;

	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	weaponTopPos_ = WeaponTopPos(WEAPON_TOP_POS);

	weaponDownPos_ = WeaponDownPos(WEAPON_DOWN_POS);

	UpdateRightWeapon();
	UpdateWeapon();

	// �������
	ChangeState(STATE::IDLE);
	SceneManager::GetInstance().GetCamera()->ChangeGameCamera(Camera::GAME_CAMERA::MOUSE);

}

void EventPlayer::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case EventPlayer::STATE::NONE:
		UpdateNone();
		break;
	case EventPlayer::STATE::IDLE:
		UpdateIdle();
		break;
	case EventPlayer::STATE::WALK:
		UpdateWalk();
		break;
	case EventPlayer::STATE::LOOK:
		UpdateLook();
		break;
	case EventPlayer::STATE::STANDBY:
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

void EventPlayer::Draw(void)
{
	weapon_->Draw();

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
}

void EventPlayer::SetPosition(VECTOR pos)
{
	transform_.pos = pos;
}

void EventPlayer::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void EventPlayer::ClearCollider(void)
{
	colliders_.clear();
}

const std::shared_ptr<Capsule> EventPlayer::GetCapsule(void) const
{
	return std::shared_ptr<Capsule>();
}

EventPlayer::ANIM_TYPE EventPlayer::GetAnimType()
{
	return anim_;
}

void EventPlayer::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", IDLE_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::WALK, path + "Unarmed Walk Forward.mv1", WALK_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::LOOK, path + "Look_Around_Idle.mv1", LOOK_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::STANDBY, path + "Stable Sword Inward Slash.mv1", STANDBY_ANIM_SPEED);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void EventPlayer::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case EventPlayer::STATE::NONE:
		ChangeStateNone();
		break;
	case EventPlayer::STATE::IDLE:
		ChangeStateIdle();
		break;
	case EventPlayer::STATE::WALK:
		ChangeStateWalk();
		break;
	case EventPlayer::STATE::LOOK:
		ChangeStateLook();
		break;
	case EventPlayer::STATE::STANDBY:
		ChangeStateStandBy();
		break;
	default:
		break;
	}
}

void EventPlayer::ChangeStateNone(void)
{
}

void EventPlayer::ChangeStateIdle(void)
{
	animationController_->Play((int)ANIM_TYPE::IDLE, true, ANIM_BLEND_RATE);
	anim_ = ANIM_TYPE::IDLE;
}

void EventPlayer::ChangeStateWalk(void)
{
	animationController_->Play((int)ANIM_TYPE::WALK, true, ANIM_BLEND_RATE);
	anim_ = ANIM_TYPE::WALK;
}

void EventPlayer::ChangeStateLook(void)
{
	StopSoundMem(sHandle_);
	animationController_->Play((int)ANIM_TYPE::LOOK, true, ANIM_BLEND_RATE);
	anim_ = ANIM_TYPE::LOOK;
}

void EventPlayer::ChangeStateStandBy(void)
{
	animationController_->Play((int)ANIM_TYPE::STANDBY, false, ANIM_BLEND_RATE);
	anim_ = ANIM_TYPE::STANDBY;
}

void EventPlayer::UpdateNone(void)
{
	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventPlayer::UpdateIdle(void)
{
	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventPlayer::UpdateWalk(void)
{
	if (!isSound_)
	{
		PlaySoundMem(sHandle_, DX_PLAYTYPE_LOOP, true);
		isSound_ = true;
	}

	// �����Ɉړ�����
	transform_.pos = VAdd(transform_.pos, VScale(AsoUtility::DIR_F, MOVE_SPEED));

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventPlayer::UpdateLook(void)
{
	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventPlayer::UpdateStandBy(void)
{
	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EventPlayer::SetGoalRotate(double rotRad)
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

void EventPlayer::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void EventPlayer::Collision(void)
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

void EventPlayer::CollisionGravity(void)
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

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		//if (hit.HitFlag > 0)
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

void EventPlayer::CollisionCapsule(void)
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

void EventPlayer::CalcGravityPow(void)
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

bool EventPlayer::IsEndLanding(ANIM_TYPE anim)
{
	if (state_ == STATE::WALK)return false;
	if (animationController_->GetPlayType() != (int)anim)return false;
	return animationController_->IsEnd();
}

VECTOR EventPlayer::WeaponTopPos(VECTOR pos)
{
	return VAdd(weaponTopPos_, pos);
}

VECTOR EventPlayer::WeaponDownPos(VECTOR pos)
{
	return VAdd(weaponDownPos_, pos);
}

void EventPlayer::UpdateRightWeapon(void)
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

void EventPlayer::UpdateWeapon()
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
