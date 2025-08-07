#include <string>
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
#include "../Enemy/EnemyBoss/EnemyBoss.h"
#include "../Enemy/EnemyBoss/EnemyBossDummy.h"
#include "../Enemy/EnemyScarecrow.h"
#include "../Enemy/EnemyScarecrowDummy.h"
#include "../Weapon/PlayerWeapon.h"
#include "../../Item/HpPotion.h"
#include "../../Item/UltPotion.h"
#include "../../Item/Book.h"
#include "../../Stage/Planet.h"
#include "../../Stage/Mist.h"
#include "Player.h"

Player::Player(void)
{
	animationController_ = nullptr;
	state_ = STATE::NONE;
	itemType_ = ITEM_TYPE::HP_POTION;

	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;
	isRun_ = false;

	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	attack_ = 0;

	attackCnt_ = 0.0f;
	attackCnt2_ = 0.0f;
	isAttack2_ = false;
	attackCnt3_ = 0.0f;
	isAttack3_ = false;

	strongAttackCnt_ = 0.0f;

	stunCnt_ = 0.0f;

	rollingCnt_ = 0.0f;

	deadCnt_ = 0.0f;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	capsule_ = nullptr;

	canAttack_ = false;

	currentHp_ = MAX_HP;
	displayHp_ = currentHp_;
	damageHp_ = currentHp_;
	healHp_ = 0;
	stamina_ = MAX_STAMINA;
	ultFrameHandle_ = resMng_.Load(ResourceManager::SRC::GAUGE_FRAME).handleId_;
	magicCircleHandle_ = resMng_.Load(ResourceManager::SRC::MAGIC_CIRCLE).handleId_;
	runeHandle_ = resMng_.Load(ResourceManager::SRC::RUNE).handleId_;
	ultValue_ = 0.0f;
	isUltActive_ = false;
	wasUltMaxed_ = false;
	flashTimer_ = 0;
	id_ = 0;
	lightningTimer_ = 0;
	isLightning_ = false;
}

Player::~Player(void)
{
	delete animationController_;
}

void Player::Init(void)
{
	// ��
	attackSe_ = resMng_.Load(ResourceManager::SRC::ATTACK_SE).handleId_;
	ChangeVolumeSoundMem(ATTACK_SE_VOL, attackSe_);

	hitSe_ = resMng_.Load(ResourceManager::SRC::HIT_SE).handleId_;
	ChangeVolumeSoundMem(HIT_SE_VOL, hitSe_);

	rollingSe_ = resMng_.Load(ResourceManager::SRC::ROLLING_SE).handleId_;
	ChangeVolumeSoundMem(ROLLING_SE_VOL, rollingSe_);

	ultSe_ = resMng_.Load(ResourceManager::SRC::ULT_SE).handleId_;
	ChangeVolumeSoundMem(ULT_SE_VOL, ultSe_);

	damageSe_ = resMng_.Load(ResourceManager::SRC::DAMAGE_SE).handleId_;
	ChangeVolumeSoundMem(DAMAGE_SE_VOL, damageSe_);

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = INIT_PLAYER_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ INIT_QUA_ROT_LOCAL.x, AsoUtility::Deg2RadF(INIT_QUA_ROT_LOCAL.y), INIT_QUA_ROT_LOCAL.z });
	transform_.Update();

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(INIT_TOP_POS);
	capsule_->SetLocalPosDown(INIT_DOWN_POS);
	capsule_->SetRadius(PLAYER_RADIUS);

	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	weapon_ = std::make_shared<PlayerWeapon>();
	weapon_->Init();

	hpPotion_ = std::make_unique<HpPotion>();
	hpPotion_->Init();

	ultPotion_ = std::make_unique<UltPotion>();
	ultPotion_->Init();

	book_ = std::make_unique<Book>();
	book_->Init();

	// �t���[���擾
	wristIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHand");
	handIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHandMiddle1");

	weaponTopIndex_ = WEAPON_TOP_INDEX;
	weaponDownIndex_ = WEAPON_DOWN_INDEX;

	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	weaponTopPos_ = WeaponTopPos(INIT_WEAPON_TOP_POS);

	UpdateRightWeapon();
	UpdateWeaponCapsule();

	// �������
	ChangeState(STATE::PLAY);
	SceneManager::GetInstance().GetCamera()->ChangeGameCamera(Camera::GAME_CAMERA::MOUSE);

	InitEffect();
}

void Player::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case Player::STATE::NONE:
		UpdateNone();
		break;
	case Player::STATE::PLAY:
		if (currentHp_ > 0)
		{
			UpdatePlay();
		}
		else
		{
			SceneManager::GetInstance().GetCamera()->ChangeGameCamera(Camera::GAME_CAMERA::NONE);
			deadCnt_++;
			animationController_->Play((int)ANIM_TYPE::DEAD, false);
		}

		// �_���[�W���̔����Q�[�W��������茸�炷
		if (damageHp_ > currentHp_) {
			damageHp_ -= HP_DECREASE_SPEED;
			if (damageHp_ < currentHp_) damageHp_ = currentHp_;
		}
		// ���C���Q�[�W�͉񕜎��ɂ�����葝����
		if (currentHp_ < healHp_) {
			currentHp_ += HP_DECREASE_SPEED;
			if (currentHp_ >= healHp_)
			{
				currentHp_ = healHp_;
				healHp_ = currentHp_;
				damageHp_ = currentHp_;
			}
		}
		break;
	}

	// ���f������X�V
	transform_.Update();

	attackCnt_--;
	attackCnt2_--;
	attackCnt3_--;
	strongAttackCnt_--;
	stunCnt_--;
	rollingCnt_--;
	lightningTimer_--;

	// �A�j���[�V�����Đ�
	animationController_->Update();

	UpdateRightWeapon();
	UpdateWeaponCapsule();
	UpdateUlt();
}

void Player::Draw(void)
{
	weapon_->Draw();

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

}

void Player::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Player::ClearCollider(void)
{
	colliders_.clear();
}

const std::shared_ptr<Capsule> Player::GetCapsule(void) const
{
	return capsule_;
}

void Player::SetEnemy(std::shared_ptr<EnemyBoss> enemy)
{
	enemy_ = enemy;
}

void Player::SetEnemyDummy(std::shared_ptr<EnemyBossDummy> enemyDummy)
{
	enemyDummy_ = enemyDummy;
}

void Player::SetEnemyScarecrow(std::shared_ptr<EnemyScarecrow> enemyScarecrow)
{
	enemyScarecrow_ = enemyScarecrow;
}

void Player::SetEnemyScarecrowDummy(std::shared_ptr<EnemyScarecrowDummy> enemyScarecrowDummy)
{
	enemyScarecrowDummy_ = enemyScarecrowDummy;
}

void Player::SetMist(std::shared_ptr<Mist> mist)
{
	mist_ = mist;
}

void Player::InitAnimation(void)
{

	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", IDLE_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", RUN_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", FAST_RUN_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", JUMP_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", WARP_PAUSE_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", FLY_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", FALLING_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", VICTORY_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::ATTACK, path + "Attack.mv1", ATTACK_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::ATTACK2, path + "Attack2.mv1", ATTACK2_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::ATTACK3, path + "Attack3.mv1", ATTACK3_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::STRONG_ATTACK, path + "StrongAttack.mv1", STRONG_ATTACK_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::STUN, path + "Stun.mv1", STUN_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::ROLLING, path + "Rolling.mv1", ROLLING_ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::DEAD, path + "Dead.mv1", DEAD_ANIM_SPEED);

	animationController_->Play((int)ANIM_TYPE::IDLE);

}

void Player::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case Player::STATE::NONE:
		ChangeStateNone();
		break;
	case Player::STATE::PLAY:
		ChangeStatePlay();
		break;
	}

}

void Player::ChangeStateNone(void)
{
}

void Player::ChangeStatePlay(void)
{
}

void Player::ChangeItem(ITEM_TYPE itemType)
{
	// ��ԕύX
	itemType_ = itemType;

	// �e��ԑJ�ڂ̏�������
	switch (itemType_)
	{
	case Player::ITEM_TYPE::HP_POTION:
		break;
	case Player::ITEM_TYPE::ULT_POTION:
		break;
	}
}

void Player::UpdateNone(void)
{
}

void Player::UpdatePlay(void)
{
	isRun_ = false;
	// �ړ�����
	ProcessMove();

	if (attackCnt_ > 0)
	{
		animationController_->Play((int)ANIM_TYPE::ATTACK);
		if (!isUltActive_)
		{
			attack_ = NORMAL_DAMAGE;
		}
		else
		{
			attack_ = SPECIAL_NORMAL_DAMAGE;
		}
		if (attackCnt_ == ATTACK_SE_TIMING)
		{
			PlaySoundMem(attackSe_, DX_PLAYTYPE_BACK, true);
		}
	}


	if (attackCnt_ <= 0 && attackCnt2_ > 0)
	{
		animationController_->Play((int)ANIM_TYPE::ATTACK2);
		if (!isUltActive_)
		{
			attack_ = NORMAL_DAMAGE;
		}
		else
		{
			attack_ = SPECIAL_NORMAL_DAMAGE;
		}
		if (attackCnt2_ == ATTACK2_SE_TIMING)
		{
			PlaySoundMem(attackSe_, DX_PLAYTYPE_BACK, true);
		}
	}


	if (attackCnt_ <= 0 && attackCnt2_ <= 0 && attackCnt3_ > 0)
	{
		animationController_->Play((int)ANIM_TYPE::ATTACK3);
		if (!isUltActive_)
		{
			attack_ = NORMAL_DAMAGE;
		}
		else
		{
			attack_ = SPECIAL_NORMAL_DAMAGE;
		}
		if (attackCnt3_ == ATTACK3_SE_TIMING)
		{
			PlaySoundMem(attackSe_, DX_PLAYTYPE_BACK, true);
		}
	}


	if (strongAttackCnt_ > 0)
	{
		animationController_->Play((int)ANIM_TYPE::STRONG_ATTACK);
		if (!isUltActive_)
		{
			attack_ = STRONG_DAMAGE;
		}
		else
		{
			attack_ = SPECIAL_STRONG_DAMAGE;
		}
		if (strongAttackCnt_ == STRONG_ATTACK_SE_TIMING)
		{
			PlaySoundMem(attackSe_, DX_PLAYTYPE_BACK, true);
		}
	}

	if ((attackCnt_ <= ATTACK_DISABLE_TIME && attackCnt2_ <= ATTACK2_DISABLE_TIME && attackCnt3_ <= ATTACK3_DISABLE_TIME && strongAttackCnt_ <= STRONG_ATTACK_DISABLE_TIME) || stunCnt_ > 0.0f)
	{
		canAttack_ = false;
	}

	if (stunCnt_ > 0.0f)
	{
		animationController_->Play((int)ANIM_TYPE::STUN);
	}
	if (rollingCnt_ > 0)
	{
		animationController_->Play((int)ANIM_TYPE::ROLLING);
		movePow_ = VScale(transform_.GetForward(), ROLLING_MOVE);
		if (rollingCnt_ == ROLLING_SE_TIMING)
		{
			PlaySoundMem(rollingSe_, DX_PLAYTYPE_BACK, true);
		}
	}


	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;

	// �A�C�e��
	UpdateItem();
}


void Player::ProcessMove(void)
{

	auto& ins = InputManager::GetInstance();

	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// �ړ��ʂ��[��
	movePow_ = AsoUtility::VECTOR_ZERO;

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	double rotRad = 0;

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	if (attackCnt_ <= 0 && attackCnt2_ <= 0 && attackCnt3_ <= 0 && strongAttackCnt_ <= 0 && stunCnt_ <= 0 && rollingCnt_ <= 0)
	{
		dir = AsoUtility::VECTOR_ZERO;

		if (ins.IsNew(KEY_INPUT_W) || input.Y < -PAD_INPUT_THRESHOLD_XY)
		{
			dir = VAdd(dir, cameraRot.GetForward());
		}
		if (ins.IsNew(KEY_INPUT_S) || input.Y > PAD_INPUT_THRESHOLD_XY)
		{
			dir = VAdd(dir, cameraRot.GetBack());
		}
		if (ins.IsNew(KEY_INPUT_D) || input.X > PAD_INPUT_THRESHOLD_XY)
		{
			dir = VAdd(dir, cameraRot.GetRight());
		}
		if (ins.IsNew(KEY_INPUT_A) || input.X < -PAD_INPUT_THRESHOLD_XY)
		{
			dir = VAdd(dir, cameraRot.GetLeft());
		}

		if (!AsoUtility::EqualsVZero(dir))
		{
			dir = VNorm(dir);
			rotRad = std::atan2(dir.x, dir.z);
		}
	}

	VECTOR angles = SceneManager::GetInstance().GetCamera()->GetAngles();

	Quaternion rots = Quaternion::Euler(AsoUtility::Deg2RadF(0.0f), AsoUtility::Deg2RadF(angles.y), AsoUtility::Deg2RadF(0.0f));

	VECTOR direction = rots.PosAxis(moveDir_);

	if (!AsoUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding())) {

		// �ړ�����
		speed_ = SPEED_MOVE;
		if ((ins.IsNew(KEY_INPUT_LSHIFT) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)) && stamina_ > 0)
		{
			speed_ = SPEED_RUN;
			isRun_ = true;
		}

		moveDir_ = dir;
		movePow_ = VScale(direction, speed_);

		// ��]����
		SetGoalRotate(rotRad);


		if (!isJump_ && IsEndLanding())
		{
			if (attackCnt_ <= 0 && attackCnt2_ <= 0 && attackCnt3_ <= 0 && strongAttackCnt_ <= 0 && stunCnt_ <= 0 && rollingCnt_ <= 0)
			{
				if ((ins.IsTrgMouseRight() || input.Z < PAD_INPUT_THRESHOLD_Z) && stamina_ > STRONG_ATTACK_STAMINA)
				{
					strongAttackCnt_ = MAX_STRONG_ATTACK_CNT;
					canAttack_ = true;
					stamina_ -= STRONG_ATTACK_STAMINA;
				}

				if ((ins.IsTrgMouseLeft() || input.Buttons[ATTACK_PAD_BUTTON_NUM]) && strongAttackCnt_ < 0)
				{
					attackCnt_ = MAX_ATTACK_CNT;
					canAttack_ = true;
				}

				// �A�j���[�V����
				if (ins.IsNew(KEY_INPUT_LSHIFT) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
				{
					if (stamina_ > 0)
					{
						animationController_->Play((int)ANIM_TYPE::FAST_RUN);
						stamina_ -= STAMINA_DRAIN_RATE;
					}
					else
					{
						animationController_->Play((int)ANIM_TYPE::RUN);
					}
				}
				else
				{
					animationController_->Play((int)ANIM_TYPE::RUN);
					if (stamina_ < MAX_STAMINA)
					{

						stamina_ += STAMINA_RECOVERY_RATE;
					}
				}
				if ((ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) && stamina_ > ROLLING_STAMINA)
				{

					rollingCnt_ = MAX_ROLLING_CNT;
					stamina_ -= ROLLING_STAMINA;

				}
			}
		}

	}
	else
	{
		if (attackCnt_ <= 0 && attackCnt2_ <= 0 && attackCnt3_ <= 0 && strongAttackCnt_ <= 0 && stunCnt_ <= 0 && rollingCnt_ <= 0)
		{
			if ((ins.IsTrgMouseRight() || input.Z < PAD_INPUT_THRESHOLD_Z) && stamina_ > STRONG_ATTACK_STAMINA)
			{
				strongAttackCnt_ = MAX_STRONG_ATTACK_CNT;
				canAttack_ = true;
				stamina_ -= STRONG_ATTACK_STAMINA;
			}

			if ((ins.IsTrgMouseLeft() || input.Buttons[ATTACK_PAD_BUTTON_NUM]) && strongAttackCnt_ < 0)
			{
				attackCnt_ = MAX_ATTACK_CNT;
				canAttack_ = true;
			}

			if (!isJump_ && IsEndLanding())
			{
				animationController_->Play((int)ANIM_TYPE::IDLE);
			}

			if ((ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) && stamina_ > ROLLING_STAMINA)
			{
				rollingCnt_ = MAX_ROLLING_CNT;
				stamina_ -= ROLLING_STAMINA;
			}

			if (stamina_ < MAX_STAMINA)
			{
				stamina_ += STAMINA_RECOVERY_RATE;
			}
		}
	}
	if (attackCnt_ < ATTACK_DISABLE_TIME && attackCnt_ > 0.0f)
	{
		if (ins.IsTrgMouseLeft() || input.Buttons[ATTACK_PAD_BUTTON_NUM])
		{
			attackCnt2_ = MAX_ATTACK_CNT_2;
		}
	}
	if (attackCnt_ == 0.0f && attackCnt2_ > 0.0f)
	{
		isAttack2_ = true;
	}
	if (isAttack2_)
	{
		attackCnt2_ = MAX_ATTACK_CNT_2;
		isAttack2_ = false;
		canAttack_ = true;
	}


	if (attackCnt2_ < ATTACK2_DISABLE_TIME && attackCnt2_ > 0.0f)
	{
		if (ins.IsTrgMouseLeft() || input.Buttons[ATTACK_PAD_BUTTON_NUM])
		{
			attackCnt3_ = MAX_ATTACK_CNT_3;
		}

	}
	if (attackCnt2_ == 0.0f && attackCnt3_ > 0.0f)
	{
		isAttack3_ = true;
	}
	if (isAttack3_)
	{
		attackCnt3_ = MAX_ATTACK_CNT_3;
		isAttack3_ = false;
		canAttack_ = true;
	}
}


void Player::SetGoalRotate(double rotRad)
{

	VECTOR angles = SceneManager::GetInstance().GetCamera()->GetAngles();
	double cameraRadY = AsoUtility::Deg2RadD(angles.y);


	Quaternion axis = Quaternion::AngleAxis(cameraRadY + rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > ROTATION_THRESHOLD_RAD)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Player::Rotate(void)
{

	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void Player::Collision(void)
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

void Player::CollisionGravity(void)
{

	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, GROUND_CHECK_UP_OFFSET));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, -GROUND_CHECK_DISTANCE));
	for (const auto c : colliders_)
	{

		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > GRAVITY_COLLISION_DOT_THRESHOLD)
		{

			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, LANDING_ADJUST_HEIGHT));

			// �W�����v���Z�b�g
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				// ���n���[�V����
				animationController_->Play(
					(int)ANIM_TYPE::JUMP, false, LANDING_ANIM_START_FRAME, LANDING_ANIM_END_FRAME, false, true);
			}

			isJump_ = false;

		}

	}

}

void Player::CollisionCapsule(void)
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

			for (int tryCnt = 0; tryCnt < MAX_COLLISION_ADJUST_COUNT; tryCnt++)
			{

				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, COLLISION_PUSHBACK_AMOUNT));
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

void Player::CollisionEnemyBoss(void)
{
	MV1RefreshCollInfo(enemyDummy_->GetTransform().modelId);

	// �J�v�Z���Ƃ̏Փ˔���
	auto hits = MV1CollCheck_Capsule(
		enemyDummy_->GetTransform().modelId, -1,
		capsule_->GetPosTop(), capsule_->GetPosDown(), PLAYER_RADIUS);

	// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
	// �v���C���[�̈ʒu���ړ�������
	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hit = hits.Dim[i];
		// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
		// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
		// �Փ˂����|���S���̖@�������������肷��
		for (int tryCnt = 0; tryCnt < MAX_COLLISION_RESOLVE_ATTEMPTS; tryCnt++)
		{
			// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
			// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
			int pHit = HitCheck_Capsule_Triangle(
				capsule_->GetPosTop(), capsule_->GetPosDown(), PLAYER_RADIUS,
				hit.Position[0], hit.Position[1], hit.Position[2]);
			if (pHit)
			{
				// �@���̕����ɂ�����Ƃ����ړ�������
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, COLLISION_PUSHBACK_AMOUNT));
				//// �J�v�Z�����ꏏ�Ɉړ�������
				//transform_.topPos.x = movedPos_.x;
				//transform_.topPos.z = movedPos_.z;
				//transform_.downPos.x = movedPos_.x;
				//transform_.downPos.z = movedPos_.z;
				transform_.pos = movedPos_;
				transform_.Update();
				continue;
			}
			break;
		}
	}
	// ���o�����|���S�����̌�n��
	MV1CollResultPolyDimTerminate(hits);
}

void Player::CalcGravityPow(void)
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

bool Player::IsEndLanding(void)
{

	bool ret = true;

	// �A�j���[�V�������W�����v�ł͂Ȃ�
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}

	// �A�j���[�V�������I�����Ă��邩
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;

}

void Player::UpdateRightWeapon(void)
{

	// ��� �� �� �̌����x�N�g��
	wristPos = MV1GetFramePosition(transform_.modelId, wristIndex_);
	handPos = MV1GetFramePosition(transform_.modelId, handIndex_);

	VECTOR dir = VSub(handPos, wristPos); // �����x�N�g��
	dir = VNorm(dir); // ���K�����ĒP�ʃx�N�g���ɂ���

	// ��������p�x���Z�o
	float yaw = atan2f(dir.x, dir.z);
	float pitch = atan2f(-dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z));
	float roll = 0.0f;

	// ���̉�]��ݒ�
	MV1SetRotationXYZ(weapon_->GetModelHandle(), VGet(pitch, yaw, roll));

	// �ʒu������
	float forwardOffset = FORWARD_OFFSET;
	float upOffset = UP_OFFSET;
	float rightOffset = RIGHT_OFFSET;

	// �E�x�N�g���Ə�x�N�g�����ȈՓI�ɋ��߂�i���S�ȉ�]�s�񂪂Ȃ��O��j
	VECTOR upVec = AsoUtility::DIR_U; // �����
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

void Player::DrawHp(float hp, int maxHp)
{
	// �F�ݒ�F���F�ł͂Ȃ����ԐF���g�p���ė�����������ۂ�
	int hpColorTop = GetColor(HP_COLOR_TOP_R, HP_COLOR_TOP_G, HP_COLOR_TOP_B);  // ����HP�̏㑤�i��������߁j
	int hpColorBottom = GetColor(HP_COLOR_BOTTOM_R, HP_COLOR_BOTTOM_G, HP_COLOR_BOTTOM_B);  // ����HP�̉����i�����������΁j
	int damageColor = GetColor(DAMAGE_COLOR_R, DAMAGE_COLOR_G, DAMAGE_COLOR_B);  // �_���[�W���i���Z���ԁj
	int healColor = GetColor(HEAL_COLOR_R, HEAL_COLOR_G, HEAL_COLOR_B); // �񕜕��i���ԗ΁j
	int borderColor = GetColor(HP_OUTER_BORDER_COLOR_R, HP_OUTER_BORDER_COLOR_G, HP_OUTER_BORDER_COLOR_B);   // �O�g�i�_�[�N�O���[�j

	int barWidth = HP_SIZE_X;
	int barHeight = HP_SIZE_Y;

	// �w�i�Ƙg�i2�d�ɕ`���Ă�◧�̓I�Ɍ�����j
	DrawBox(HP_POS_X - OUTER_BORDER_OFFSET, HP_POS_Y - OUTER_BORDER_OFFSET, HP_POS_X + barWidth + OUTER_BORDER_OFFSET, HP_POS_Y + barHeight + OUTER_BORDER_OFFSET, borderColor, TRUE);     // �O�g�i�Áj
	DrawBox(HP_POS_X - INNER_BORDER_OFFSET, HP_POS_Y - INNER_BORDER_OFFSET, HP_POS_X + barWidth + INNER_BORDER_OFFSET, HP_POS_Y + barHeight + INNER_BORDER_OFFSET, GetColor(HP_INNER_BORDER_COLOR_R, HP_INNER_BORDER_COLOR_G, HP_INNER_BORDER_COLOR_B), TRUE); // ���g�i���j

	// �w�i�iHP���[���̂Ƃ��̃x�[�X�j
	DrawBox(HP_POS_X, HP_POS_Y, HP_POS_X + barWidth, HP_POS_Y + barHeight, GetColor(HP_BG_COLOR_R, HP_BG_COLOR_G, HP_BG_COLOR_B), TRUE);

	// �_���[�W��
	int damageW = static_cast<int>(barWidth * damageHp_ / maxHp);
	if (damageW > 0)
		DrawBox(HP_POS_X, HP_POS_Y, HP_POS_X + damageW, HP_POS_Y + barHeight, damageColor, TRUE);

	// �񕜕�
	int healW = static_cast<int>(barWidth * healHp_ / maxHp);
	if (healW > 0)
		DrawBox(HP_POS_X, HP_POS_Y, HP_POS_X + healW, HP_POS_Y + barHeight, healColor, TRUE);

	// ����HP�i�O���f�[�V�������F��Ɖ��ŐF��ς���j
	int hpW = static_cast<int>(barWidth * currentHp_ / maxHp);
	if (hpW > 0)
	{
		int halfH = barHeight / 2;
		DrawBox(HP_POS_X, HP_POS_Y, HP_POS_X + hpW, HP_POS_Y + halfH, hpColorTop, TRUE);      // �㔼��
		DrawBox(HP_POS_X, HP_POS_Y + halfH, HP_POS_X + hpW, HP_POS_Y + barHeight, hpColorBottom, TRUE);  // ������
	}
}

void Player::DrawStamina(float stamina, int staminaMax)
{
	// �������������F�n�̐F��p�Ӂi�㉺�Ŕ����ɈႤ�F�ŃO���f���j
	int colorTop = GetColor(STAMINA_COLOR_TOP_R, STAMINA_COLOR_TOP_G, STAMINA_COLOR_TOP_B);    // ����߂̉��F
	int colorBottom = GetColor(STAMINA_COLOR_BOTTOM_R, STAMINA_COLOR_BOTTOM_G, STAMINA_COLOR_BOTTOM_B); // �����Â߂̉��F

	int borderDark = GetColor(STAMINA_OUTER_BORDER_COLOR_R, STAMINA_OUTER_BORDER_COLOR_G, STAMINA_OUTER_BORDER_COLOR_B);    // �g�̈Â��F
	int borderLight = GetColor(STAMINA_INNER_BORDER_COLOR_R, STAMINA_INNER_BORDER_COLOR_G, STAMINA_INNER_BORDER_COLOR_B); // �g�̖��邢�F

	int barWidth = STAMINA_SIZE_X;
	int barHeight = STAMINA_SIZE_Y;

	// �O�g�i�Z���߁j
	DrawBox(STAMINA_POS_X - OUTER_BORDER_OFFSET, STAMINA_POS_Y - OUTER_BORDER_OFFSET, STAMINA_POS_X + barWidth + OUTER_BORDER_OFFSET, STAMINA_POS_Y + barHeight + OUTER_BORDER_OFFSET, borderDark, TRUE);

	// ���g�i�������邢�j
	DrawBox(STAMINA_POS_X - INNER_BORDER_OFFSET, STAMINA_POS_Y - INNER_BORDER_OFFSET, STAMINA_POS_X + barWidth + INNER_BORDER_OFFSET, STAMINA_POS_Y + barHeight + INNER_BORDER_OFFSET, borderLight, TRUE);

	// �w�i�i�Â߁j
	DrawBox(STAMINA_POS_X, STAMINA_POS_Y, STAMINA_POS_X + barWidth, STAMINA_POS_Y + barHeight, GetColor(STAMINA_BG_COLOR_R, STAMINA_BG_COLOR_G, STAMINA_BG_COLOR_B), TRUE);

	// �X�^�~�i�Q�[�W�̕����v�Z
	int staminaW = static_cast<int>(barWidth * stamina / staminaMax);
	if (staminaW > 0)
	{
		int halfH = barHeight / 2;
		// �㔼���F����߉��F
		DrawBox(STAMINA_POS_X, STAMINA_POS_Y, STAMINA_POS_X + staminaW, STAMINA_POS_Y + halfH, colorTop, TRUE);
		// �������F�Â߉��F
		DrawBox(STAMINA_POS_X, STAMINA_POS_Y + halfH, STAMINA_POS_X + staminaW, STAMINA_POS_Y + barHeight, colorBottom, TRUE);
	}
}

void Player::UpdateItem(void)
{
	auto& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	static int prevPOV = INVALID_POV;
	int nowPOV = input.POV[0];

	bool isPOVRightTriggered = (nowPOV == POV_RIGHT && prevPOV != POV_RIGHT);
	bool isPOVLeftTriggered = (nowPOV == POV_LEFT && prevPOV != POV_LEFT);
	prevPOV = nowPOV;

	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	// ���̃A�C�e����
	if (wheelCounter >= WHEEL_THRESHOLD || isPOVRightTriggered)
	{
		itemType_ = static_cast<ITEM_TYPE>((static_cast<int>(itemType_) + 1) % static_cast<int>(ITEM_TYPE::NUM));
		wheelCounter = 0;
	}
	// �O�̃A�C�e����
	else if (wheelCounter <= -WHEEL_THRESHOLD || isPOVLeftTriggered)
	{
		itemType_ = static_cast<ITEM_TYPE>((static_cast<int>(itemType_) - 1 + static_cast<int>(ITEM_TYPE::NUM)) % static_cast<int>(ITEM_TYPE::NUM));
		wheelCounter = 0;
	}

	// �A�C�e���g�p
	switch (itemType_)
	{
	case ITEM_TYPE::HP_POTION:
		UseHpPosion();
		break;
	case ITEM_TYPE::ULT_POTION:
		UseUltPosion();
		break;
	case ITEM_TYPE::BOOK:
		UseBook();
		break;
	}

	isLightning_ = false;
	if (lightningTimer_ == LIGHTNING_UPDATE_FRAME)
	{
		if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::TUTORIAL)
		{
			book_->LightningUpdate(enemyScarecrow_->GetTransform().pos);
		}
		else if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::GAME)
		{
			book_->LightningUpdate(enemy_->GetTransform().pos);
		}
	}
	if (lightningTimer_ == LIGHTNING_START_FRAME)
	{
		isLightning_ = true;
	}
}

void Player::DrawItem(void)
{
	switch (itemType_)
	{
	case ITEM_TYPE::HP_POTION:
		hpPotion_->Draw();
		break;
	case ITEM_TYPE::ULT_POTION:
		ultPotion_->Draw();
		break;
	case ITEM_TYPE::BOOK:
		book_->Draw();
		break;
	}
}

void Player::UseHpPosion(void)
{
	auto& ins = InputManager::GetInstance();

	if ((ins.IsTrgDown(KEY_INPUT_R)
		|| ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
		&& hpPotion_->GetCount() > 0 && healHp_ < MAX_HP
		&& currentHp_ < MAX_HP
		&& stunCnt_ < 0.0f)
	{
		hpPotion_->Use();
		healHp_ = healHp_ + HP_POTION_HEAL_AMOUNT;
	}
	if (currentHp_ > MAX_HP)
	{
		currentHp_ = MAX_HP;
	}
	if (healHp_ > MAX_HP)
	{
		healHp_ = MAX_HP;
	}
}

void Player::UseUltPosion(void)
{
	auto& ins = InputManager::GetInstance();

	if ((ins.IsTrgDown(KEY_INPUT_R)
		|| ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
		&& ultPotion_->GetCount() > 0
		&& ultValue_ < ULT_MAX_VALUE
		&& isUltActive_ == false
		&& stunCnt_ < 0.0f)
	{
		ultPotion_->Use();
		ultValue_ += ULT_MAX_VALUE;
	}
}

void Player::UseBook(void)
{
	auto& ins = InputManager::GetInstance();

	if ((ins.IsTrgDown(KEY_INPUT_R)
		|| ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
		&& book_->GetCount() > 0
		&& stunCnt_ < 0.0f)
	{
		book_->Use();
		lightningTimer_ = LIGHTNING_DURATION_FRAME;
	}
}

VECTOR Player::WeaponTopPos(VECTOR pos)
{
	return VAdd(weaponTopPos_, pos);
}

VECTOR Player::WeaponDownPos(VECTOR pos)
{
	return VAdd(weaponDownPos_, pos);
}


void Player::UpdateWeaponCapsule()
{
	// ��� �� �� �̌����x�N�g��
	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	VECTOR dir = VSub(weaponTopPos_, weaponDownPos_); // �����x�N�g��
	dir = VNorm(dir); // ���K�����ĒP�ʃx�N�g���ɂ���

	// �E�x�N�g���Ə�x�N�g�����ȈՓI�ɋ��߂�i���S�ȉ�]�s�񂪂Ȃ��O��j
	VECTOR upVec = AsoUtility::DIR_U; // �����
	VECTOR rightVec = VCross(upVec, dir);  // ��̌����Əォ��E���������
	rightVec = VNorm(rightVec); // ���K��

	// ������Ē����i��̂Ђ˂�͍l�����Ȃ��j
	upVec = VCross(dir, rightVec);
	upVec = VNorm(upVec);

	// �I�t�Z�b�g����
	VECTOR offsetPos = handPos;
	offsetPos = VAdd(offsetPos, VScale(dir, FORWARD_OFFSET));   // �O��
	offsetPos = VAdd(offsetPos, VScale(upVec, UP_OFFSET));      // ��
	offsetPos = VAdd(offsetPos, VScale(rightVec, RIGHT_OFFSET));// �E

	// ���̈ʒu��ݒ�
	MV1SetPosition(weapon_->GetModelHandle(), offsetPos);
}

void Player::InitEffect(void)
{
	// �q�b�g�G�t�F�N�g
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HIT_EFFECT).handleId_;

	// ULT�G�t�F�N�g
	effectUltResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::ULT_EFFECT).handleId_;
}

void Player::UpdateUlt(void)
{
	auto& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (!isUltActive_) {
		// �ʏ펞�̓Q�[�W����
		ultValue_ += SceneManager::GetInstance().GetDeltaTime() * ULT_CHARGE_SPEED;

		if (ultValue_ > ULT_MAX_VALUE) ultValue_ = ULT_MAX_VALUE;

		// �Q�[�W�����^���ɂȂ�����t���O�𗧂Ă�
		if (ultValue_ >= ULT_MAX_VALUE) {
			wasUltMaxed_ = true;
		}

		// �����J�n
		if (ultValue_ >= ULT_MAX_VALUE && currentHp_ > 0 && (ins.IsTrgDown(KEY_INPUT_Q) || input.Buttons[4])) {
			if (input.Buttons[4]) {
				// �p�b�h�U���i��F���߂�0.5�b�U���j
				StartJoypadVibration(DX_INPUT_PAD1, PAD_VIBRATION_POWER, PAD_VIBRATION_DURATION_MS);
			}
			isUltActive_ = true;
			flashTimer_ = 0;
			PlaySoundMem(ultSe_, DX_PLAYTYPE_LOOP, true);
			// �G�t�F�N�g
			effectUltPlayId_ = PlayEffekseer3DEffect(effectUltResId_);
			// �傫��
			SetScalePlayingEffekseer3DEffect(effectUltPlayId_, ULT_EFFECT_SCALE, ULT_EFFECT_SCALE, ULT_EFFECT_SCALE);
			SetPosPlayingEffekseer3DEffect(effectUltPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);

		}
	}
	else {
		// �������̓Q�[�W�����炷
		ultValue_ -= SceneManager::GetInstance().GetDeltaTime() * ULT_DEPLETION_SPEED;
		SetPosPlayingEffekseer3DEffect(effectUltPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
		if (ultValue_ < 0.0f) {
			ultValue_ = 0.0f;
			isUltActive_ = false;
			wasUltMaxed_ = false;  // �Q�[�W��0�ɂȂ����̂Ń��Z�b�g
			StopSoundMem(ultSe_);
		}
	}
}

void Player::DrawUlt()
{
	auto& ins = InputManager::GetInstance();

	// 0. ���@�~�i��ɂ�������]�j
	float angle = fmod(GetNowCount() * MAGIC_CIRCLE_ROTATE_SPEED, FULL_CIRCLE_DEG);
	float rotationRad = angle * AsoUtility::DEG2RAD;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MAX_ALPHA);
	DrawRotaGraph(ULT_CENTER_X, ULT_CENTER_Y, ULT_DRAW_SCALE, rotationRad, magicCircleHandle_, TRUE);

	// 1. ���@�~�̖��邳�����i�_�ł���߂ď������邭�j
	if (wasUltMaxed_) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, ULT_BRIGHTEN_ALPHA);
		DrawRotaGraph(ULT_CENTER_X, ULT_CENTER_Y, ULT_DRAW_SCALE, rotationRad, magicCircleHandle_, TRUE);
	}
	else if (ultValue_ > 0.0f) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ULT_SOFT_PULSE_ALPHA);
		DrawRotaGraph(ULT_CENTER_X, ULT_CENTER_Y, ULT_DRAW_SCALE, rotationRad, magicCircleHandle_, TRUE);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 2. �����}�X�N�iultValue_��������ɂ�Ĕ����v���ɏ�����A�J�n�ʒu�͏�(90�x)�ɕύX�j
	float angleMax = FULL_CIRCLE_DEG * (ULT_MAX_VALUE - ultValue_);

	int maskColor = GetColor(0, 0, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MAX_ALPHA);
	for (int i = 0; i < ULT_SEGMENT_COUNT; ++i) {
		float t1 = (float)i / ULT_SEGMENT_COUNT;
		float t2 = (float)(i + 1) / ULT_SEGMENT_COUNT;

		float angle1 = fmod(FULL_CIRCLE_DEG - angleMax * t1 + ULT_START_ANGLE, FULL_CIRCLE_DEG);
		float angle2 = fmod(FULL_CIRCLE_DEG - angleMax * t2 + ULT_START_ANGLE, FULL_CIRCLE_DEG);

		// �`�悷�ׂ��p�x�͈͂��`�F�b�N

		float radius1 = angle1 * AsoUtility::DEG2RAD;
		float radius2 = angle2 * AsoUtility::DEG2RAD;

		int x1 = ULT_CENTER_X + (int)(cos(radius1) * ULT_RADIUS);
		int y1 = ULT_CENTER_Y - (int)(sin(radius1) * ULT_RADIUS);
		int x2 = ULT_CENTER_X + (int)(cos(radius2) * ULT_RADIUS);
		int y2 = ULT_CENTER_Y - (int)(sin(radius2) * ULT_RADIUS);

		DrawTriangle(ULT_CENTER_X, ULT_CENTER_Y, x1, y1, x2, y2, maskColor, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 3. Q�L�[���������Ƃ��̉��o�i40�t���[���p���j
	if (isUltActive_ && flashTimer_ < ULT_FLASH_DURATION) {
		for (int i = 0; i < FLASH_ALPHA_DECAY; ++i) {
			int alpha = (int)(MAX_ALPHA * (ULT_MAX_VALUE - flashTimer_ / static_cast<float>(ULT_FLASH_DURATION))) - i * ULT_FLASH_DURATION;
			if (alpha < 0) alpha = 0;
			int radius = ULT_RADIUS + FLASH_BASE_RADIUS + i * FLASH_RING_SPACING + flashTimer_ * FLASH_RADIUS_GROWTH;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawCircle(ULT_CENTER_X, ULT_CENTER_Y, radius, GetColor(FLASH_COLOR_R, FLASH_COLOR_G, FLASH_COLOR_B), TRUE);
		}

		for (int i = 0; i < ULT_RAY_COUNT; ++i) {
			float rayAngle = (flashTimer_ * FLASH_ROTATE_SPEED + i * (FULL_CIRCLE_DEG / ULT_RAY_COUNT)) * AsoUtility::DEG2RAD;
			int rayLength = ULT_RADIUS + ULT_FLASH_DURATION + (flashTimer_ % FLASH_ROTATE_SPEED) * FLASH_ALPHA_DECAY;
			int x = ULT_CENTER_X + (int)(cos(rayAngle) * rayLength);
			int y = ULT_CENTER_Y - (int)(sin(rayAngle) * rayLength);
			int alpha = RAY_START_ALPHA - (flashTimer_ * RAY_ALPHA_DECAY_PER_FRAME);
			if (alpha < 0) alpha = 0;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawLine(ULT_CENTER_X, ULT_CENTER_Y, x, y, GetColor(RAY_COLOR_R, RAY_COLOR_G, RAY_COLOR_B), ULT_RAY_THICKNESS);
		}

		flashTimer_++;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	// 4. �g
	DrawRotaGraph(ULT_CENTER_X, ULT_CENTER_Y, ULT_DRAW_SCALE, 0.0f, ultFrameHandle_, TRUE);
}

void Player::TutorialUlt(void)
{
	ultValue_ = 0.0f;
	isUltActive_ = false;
	wasUltMaxed_ = false;  // �Q�[�W��0�ɂȂ����̂Ń��Z�b�g
	StopSoundMem(ultSe_);
	StopEffekseer3DEffect(effectUltPlayId_);
}

bool Player::IsHitAttackEnemyBoss(void)
{
	MV1RefreshCollInfo(enemy_->GetTransform().modelId);

	// �J�v�Z���Ƃ̏Փ˔���
	auto hits = MV1CollCheck_Capsule(
		enemy_->GetTransform().modelId, -1,
		weaponTopPos_, weaponDownPos_, WEAPON_RADIUS);

	bool isUseEffect = false;

	if (hits.HitNum > 0 && canAttack_)
	{
		canAttack_ = false;
		PlaySoundMem(hitSe_, DX_PLAYTYPE_BACK, true);

		for (int i = 0; i < hits.HitNum; i++)
		{
			auto ePos = hits.Dim[i].HitPosition;
			VECTOR wCenter = VScale(VAdd(weaponTopPos_, weaponDownPos_), 0.5f);
			if (AsoUtility::IsHitSpheres(wCenter, WEAPON_RADIUS * 2, ePos, ENEMY_HIT_SPHERE_RADIUS))
			{

				// �G�t�F�N�g
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				// �傫��
				SetScalePlayingEffekseer3DEffect(effectHitPlayId_, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE);
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, hits.Dim->HitPosition.x, hits.Dim->HitPosition.y, hits.Dim->HitPosition.z);
				isUseEffect = true;
			}
		}
		if (isUseEffect == false)
		{
			// �G�t�F�N�g
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
			// �傫��
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE);
			SetPosPlayingEffekseer3DEffect(effectHitPlayId_, weaponTopPos_.x, weaponTopPos_.y, weaponTopPos_.z);
		}
		// ���o�����|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
		return true;
	}
	else
	{
		// ���o�����|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
		return false;
	}
}

bool Player::IsHitAttackEnemyScarecrow(void)
{
	MV1RefreshCollInfo(enemyScarecrow_->GetTransform().modelId);

	// �J�v�Z���Ƃ̏Փ˔���
	auto hits = MV1CollCheck_Capsule(
		enemyScarecrow_->GetTransform().modelId, -1,
		weaponTopPos_, weaponDownPos_, WEAPON_RADIUS);

	bool isUseEffect = false;

	if (hits.HitNum > 0 && canAttack_)
	{
		canAttack_ = false;
		PlaySoundMem(hitSe_, DX_PLAYTYPE_BACK, true);

		for (int i = 0; i < hits.HitNum; i++)
		{
			auto ePos = hits.Dim[i].HitPosition;
			VECTOR wCenter = VScale(VAdd(weaponTopPos_, weaponDownPos_), 0.5f);
			if (AsoUtility::IsHitSpheres(wCenter, WEAPON_RADIUS * 2, ePos, ENEMY_HIT_SPHERE_RADIUS))
			{
				// �G�t�F�N�g
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				// �傫��
				SetScalePlayingEffekseer3DEffect(effectHitPlayId_, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE);
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, hits.Dim->HitPosition.x, hits.Dim->HitPosition.y, hits.Dim->HitPosition.z);
				isUseEffect = true;
			}
		}
		if (isUseEffect == false)
		{
			// �G�t�F�N�g
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
			// �傫��
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE, EFFECT_HIT_SCALE);
			SetPosPlayingEffekseer3DEffect(effectHitPlayId_, weaponTopPos_.x, weaponTopPos_.y, weaponTopPos_.z);
		}

		// ���o�����|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
		return true;
	}
	else
	{
		// ���o�����|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
		return false;
	}
}

int Player::GetHp(void)
{
	return static_cast<int>(currentHp_);
}

int Player::GetStamina(void)
{
	return stamina_;
}

void Player::Damage(int damage)
{
	if (currentHp_ > 0)
	{
		// ���[�����O�̏���
		if (rollingCnt_ <= 0)
		{
			currentHp_ -= damage;
			healHp_ = currentHp_;
			PlaySoundMem(damageSe_, DX_PLAYTYPE_BACK, true);
		}
		else
		{
			if (!isUltActive_)
			{
				ultValue_ += ULT_CHARGE_SPEED;
			}
		}

		if (currentHp_ < 0)
		{
			currentHp_ = 0;
		}
		if (healHp_ < 0)
		{
			healHp_ = 0;
		}

		if (ultValue_ > ULT_MAX_VALUE)
		{
			ultValue_ = ULT_MAX_VALUE;
		}
	}
}

bool Player::IsDead(void)
{
	if (deadCnt_ > PLAYER_DEAD_TIME)
	{
		return true;
	}
	else
	{
		return false;
	}
}

VECTOR Player::GetMovePow(void)
{
	return movePow_;
}

int Player::GetID() const
{
	return id_;
}

void Player::StunUpdate(void)
{
	if (stunCnt_ <= STUN_RECHARGE_THRESHOLD && rollingCnt_ <= 0)
	{
		stunCnt_ = STUN_DURATION;
	}
}

void Player::CollisionEnemyScarecrow(void)
{
	MV1RefreshCollInfo(enemyScarecrowDummy_->GetTransform().modelId);

	// �J�v�Z���Ƃ̏Փ˔���
	auto hits = MV1CollCheck_Capsule(
		enemyScarecrowDummy_->GetTransform().modelId, -1,
		capsule_->GetPosTop(), capsule_->GetPosDown(), WEAPON_RADIUS);

	// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA�v���C���[�̈ʒu���ړ�������
	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hit = hits.Dim[i];
		// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
		// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
		// �Փ˂����|���S���̖@�������������肷��
		for (int tryCnt = 0; tryCnt < MAX_COLLISION_RESOLVE_ATTEMPTS; tryCnt++)
		{
			// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
			// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
			int pHit = HitCheck_Capsule_Triangle(
				capsule_->GetPosTop(), capsule_->GetPosDown(), WEAPON_RADIUS,
				hit.Position[0], hit.Position[1], hit.Position[2]);
			if (pHit)
			{
				// �@���̕����ɂ�����Ƃ����ړ�������
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, COLLISION_PUSHBACK_AMOUNT));
				transform_.pos = movedPos_;
				transform_.Update();
				continue;
			}
			break;
		}
	}
	// ���o�����|���S�����̌�n��
	MV1CollResultPolyDimTerminate(hits);
}

void Player::CollisionMist(void)
{
	for (int h = 0; h < mist_->GetMistTransforms().size(); h++) {
		MV1RefreshCollInfo(mist_->GetTransform().modelId);

		// �J�v�Z���Ƃ̏Փ˔���
		auto hits = MV1CollCheck_Capsule(
			mist_->GetMistTransforms()[h].modelId, -1,
			capsule_->GetPosTop(), capsule_->GetPosDown(), WEAPON_RADIUS);

		// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
		// �v���C���[�̈ʒu���ړ�������
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
			// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
			// �Փ˂����|���S���̖@�������������肷��
			for (int tryCnt = 0; tryCnt < MAX_COLLISION_RESOLVE_ATTEMPTS; tryCnt++)
			{
				// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
				// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
				int pHit = HitCheck_Capsule_Triangle(
					capsule_->GetPosTop(), capsule_->GetPosDown(), WEAPON_RADIUS,
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					// �@���̕����ɂ�����Ƃ����ړ�������
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, COLLISION_PUSHBACK_AMOUNT));
					//// �J�v�Z�����ꏏ�Ɉړ�������
					transform_.pos = movedPos_;
					transform_.Update();
					continue;
				}
				break;
			}
		}
		// ���o�����|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
	}
}

bool Player::IsRun(void)
{
	return isRun_;
}

float Player::GetAttackCnt(void)
{
	return attackCnt_;
}

float Player::GetAttackCnt2(void)
{
	return attackCnt2_;
}

float Player::GetAttackCnt3(void)
{
	return attackCnt3_;
}

float Player::GetStrongAttackCnt(void)
{
	return strongAttackCnt_;
}

float Player::GetRollingCnt(void)
{
	return rollingCnt_;
}

int Player::GetAttack(void)
{
	return attack_;
}

void Player::SetUltValue(float ultValue)
{
	ultValue_ = ultValue;
}

bool Player::IsLightning(void)
{
	return isLightning_;
}

void Player::StopEffect(void)
{
	StopEffekseer3DEffect(effectHitPlayId_);
	StopEffekseer3DEffect(effectUltPlayId_);
}
