#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../../../Application.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Camera.h"
#include "../../../Common/AnimationController.h"
#include "../../../Common/Capsule.h"
#include "../../../Common/Collider.h"
#include "../../../Stage/Planet.h"
#include "../../Weapon/EnemyWeapon.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerDummy.h"
#include "../../../Stage/Mist.h"

#include "State/EnemyBossStateMachine.h"
#include "Animation/EnemyBossAnimation.h"

#include "State/EnemyBossCombatSystem.h"
#include "State/EnemyBossCombatController.h"
#include "State/Attack/EnemyBossAttackStateMachine.h"

#include "../EnemyBoss/State/Idle/EIdleState.h"
#include "../EnemyBoss/State/Observe/EObserveState.h"
#include "../EnemyBoss/State/Move/EMoveState.h"
#include "../EnemyBoss/State/Chase/EChaseState.h"
#include "../EnemyBoss/State/Attack/EAttackState.h"
#include "../EnemyBoss/State/Step/EStepState.h"
#include "../EnemyBoss/State/Stun/EStunState.h"
#include "../EnemyBoss/State/Recover/ERecoverState.h"
#include "../EnemyBoss/State/Dead/EDeadState.h"

#include "State/Attack/Pattern/ComboAttack.h"
#include "State/Attack/Pattern/ThrustAttack.h"
#include "State/Attack/Pattern/RoarAttack.h"
#include "State/Attack/Pattern/ExplosionAttack.h"
#include "State/Attack/Pattern/SetUpAttack.h"

#include "EnemyBoss.h"

EnemyBoss::EnemyBoss(Player& player) 
	:
	player_(player),
	mist_(nullptr),
	weapon_(nullptr),
	stateMachine_(nullptr),
	animation_(nullptr),
	combat_(nullptr),
	combatController_(nullptr),
	attackFSM_(nullptr),
	hitPlayers_(0),
	hp_(0),
	maxHp_(0),
	nextAttackType_(ATK_STATE::NONE),
	moveDir_({}),
	movedPos_({}),
	movePow_({}),
	jumpPow_({}),
	isJump_(false),
	toPlayer_({}),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	damageHp_(0),
	colliders_({}),
	capsule_(nullptr),
	wristIndex_(0),
	wristPos({}),
	handIndex_(0),
	handPos({}),
	weaponTopIndex_(0),
	weaponTopPos_({}),
	weaponDownIndex_(0),
	weaponDownPos_({}),
	capsuleDir_({}),
	capsuleCenter_({}),
	radius_(0),
	isStunned_(false),
	stunTimer_(0.0f),
	effectStunPlayId_(-1),
	effectStunResId_(-1),
	isStunEffectActive_(false),
	effectBuffPlayId_(-1),
	effectBuffResId_(-1),
	isSetUpEffectActive_(false),
	currentAttackMultiplier_(0.0f),
	attackBuffTimer_(0.0f),
	isDead_(false),
	deadTimer_(0.0f),
	stepCooldown_(0.0f),
	sHandle_(0),
	isSound_(false)
{
	
}

EnemyBoss::~EnemyBoss(void)
{
	StopEffekseer3DEffect(effectBuffPlayId_);
	DeleteEffekseerEffect(effectBuffResId_);
}

void EnemyBoss::Init(void)
{
	// ���f���̏�����
	InitModel();

	// �e��p�����[�^�̏����l�ݒ�
	InitParam();
	
	// �X�e�[�g�}�V���̏�����(�s���Ǘ�)
	stateMachine_ = std::make_shared<EnemyStateMachine>(*this);
	InitStateMachine();

	// �A�j���[�V�����֘A�̏�����
	animation_ = std::make_shared<EnemyBossAnimation>(transform_.modelId);
	InitAnimation();

	// �퓬����̏�����
	combat_ = std::make_shared<EnemyBossCombatSystem>(*this);
	combatController_ = std::make_shared<EnemyBossCombatController>(*this);

	// �U���p�^�[���̏�ԑJ�ڏ�����
	attackFSM_ = std::make_shared<EnemyBossAttackStateMachine>();
	InitAttackPattern();
	

	// �G�t�F�N�g�ƃT�E���h�̏�����
	InitEffect();
	InitSound();

	// �J�v�Z���R���C�_(�����蔻��)�ݒ�
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, CAPSULE_TOP_LOCAL_POS_Y * SIZE, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, CAPSULE_DOWN_LOCAL_POS_Y, 0.0f });
	capsule_->SetRadius(CAPSULE_RADIUS * SIZE);

	// ���평����
	weapon_ = std::make_shared<EnemyWeapon>();
	weapon_->Init();

	// ���f�����̃t���[���C���f�b�N�X�擾(����Ǝ�̈ʒu�Q��)
	wristIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHand");
	handIndex_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHandMiddle1");
	weaponTopIndex_ = WEAPON_TOP_INDEX;
	weaponDownIndex_ = WEAPON_DOWN_INDEX;

	// ����̐�[�E���[�̃��[���h�ʒu�擾
	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	// ����̓����蔻��X�V
	UpdateWeapon();
	UpdateWeaponCapsule();

	// �������̂��ߍs���͂������A�ҋ@��Ԃɐݒ�
	stateMachine_->ChangeState(STATE::IDLE);
}

void EnemyBoss::Update(void)
{
	// ���Ԃ̎擾
	float dt = SceneManager::GetInstance().GetDeltaTime();

	// �v���C���[�Ƃ̋����̌v�Z
	toPlayer_ = VSub(player_.GetTransform().pos, transform_.pos);
	
	// �U���ΏۂƂ�Y���̍��𖳎�����XY���ʂ̕����ɐ�������
	toPlayer_.y = 0.0f;

	// ����̓����蔻��X�V
	UpdateWeapon();
	UpdateWeaponCapsule();

	// �A�j���[�V�����Đ�
	animation_->Update();

	// �_���[�W���̔����Q�[�W�̍X�V
	Damage();

	// ���S��Ԃ��ŗD��
	if (isDead_)
	{
		return;
	}
	
	// �v���C���[���{�X�ɍU�����q�b�g�������ꍇ�AHP�����Z
	if (player_.IsHitAttackEnemyBoss())
	{
		hp_ -= player_.GetAttack();
	}

	// �v���C���[�����C�g�j���O�𔭓��������ꍇ�A�{�X���X�^�������AHP�����Z
	if (player_.IsLightning()) 
	{
		SetStunned(STUN_TIME);
		hp_ -= LIGHTNING_DAMAGE;
	}

	// HP��0�ȉ������݂̏�Ԃ����S��ԂłȂ��ꍇ�A���S�������J�n
	if (hp_ <= 0 && stateMachine_->GetCurrentStateId() != STATE::DEAD) 
	{
		// ��Ԃ����S�ɐ؂�ւ��A�A�j���[�V�����⏈�����J�n
		SetDead();
	}

	// ���݂̏�Ԃ����S��Ԃ̏ꍇ�̏���
	if (stateMachine_->GetCurrentStateId() == STATE::DEAD) 
	{
		// ���S��̗P�\���Ԃ�����������
		if (deadTimer_ > 0.0f) 
		{
			deadTimer_ -= dt;
			// �P�\���Ԃ�0�ȉ��ɂȂ����犮�S�Ɏ��S��ԂɈڍs
			if (deadTimer_ <= 0.0f) 
			{
				deadTimer_ = 0.0f;
				// ���S�Ɏ��S�������Ƃ������t���O���Z�b�g
				SetIsDead(true);
			}
		}

		// ���S���͂��ׂẴG�t�F�N�g���~����
		attackFSM_->StopEffect();
		StopStunEffect();
		StopSetupEffect();
		
		// ���S��ԌŗL�̏������s��
		if (stateMachine_)
		{
			stateMachine_->Update(*this);
		}

		// ���S���͑��̍X�V�������s�킸�A�����ŏ������I��
		return;
	}

	// �X�e�b�v�̃N�[���_�E���^�C�}�[���X�V
	// �N�[���_�E�����ł���΁A�o�ߎ��ԕ��������炷
	if (stepCooldown_ > 0.0f)
	{
		stepCooldown_ -= dt;
		// �}�C�i�X�l�ɂȂ�Ȃ��悤��0�ŃN���b�v
		if (stepCooldown_ < 0.0f)
		{
			stepCooldown_ = 0.0f;
		}
	}

	// �{�X�̃X�^����Ԏ��̍X�V
	if (isStunned_) 
	{
		// �X�^����Ԃ̎c�莞�Ԃ����������A0�ȉ��Ȃ�X�^�������A�c�莞�Ԃ�0�ɌŒ�
		stunTimer_ -= dt;
		if (stunTimer_ <= 0.0f) 
		{
			isStunned_ = false;
			stunTimer_ = 0.0f;
		}
		else 
		{
			// �X�^����Ԓ��̓X�^���p�G�t�F�N�g���J�n(���J�n�ł����)
			if (!IsStunEffectActive())
			{
				StartStunEffect();
			}
		}
	}
	else 
	{
		// �X�^����ԂȂ���΃X�^���p�G�t�F�N�g���~������
		if (IsStunEffectActive())
		{
			StopStunEffect();
		}
	}

	// �U���̓A�b�v�o�t�̌��ʎ��Ԃ��X�V
	if (attackBuffTimer_ > 0.0f) 
	{
		attackBuffTimer_ -= dt;

		// �o�t���Ԃ��I�������ꍇ�̏���
		if (attackBuffTimer_ <= 0.0f) 
		{
			RemoveAttackBuff();		// �U���̓A�b�v�̌��ʂ�����
			StopSetupEffect();		// �o�t�I�����ɃG�t�F�N�g�I��
			StopSoundMem(sHandle_);	// �o�t���ʉ��̒�~
		}
		else 
		{
			// �o�t���L���ȊԂ́A�G�t�F�N�g�ƃT�E���h���Đ�����
			if (!IsSetupEffectActive()) 
			{
				StartSetupEffect();	// �G�t�F�N�g���J�n
				if (!isSound_)
				{
					// ���[�v�Đ��Ō��ʉ��𗬂�
					PlaySoundMem(sHandle_, DX_PLAYTYPE_LOOP, true);
					isSound_ = true;
				}
			}
		}
	}
	else 
	{
		// �o�t���ʂ����ɐ؂�Ă���ꍇ�́A�G�t�F�N�g�ƃT�E���h�����S�ɒ�~
		if (IsSetupEffectActive()) 
		{
			StopSetupEffect();
			StopSoundMem(sHandle_);
		}
	}

	// �{�X���X�^�����̏ꍇ�A���̏�ԍX�V���X�L�b�v
	if (IsStunned()) 
	{
		// ���݂̃X�e�[�g��STUN�ȊO�ł���΁ASTUN�X�e�[�g�ɋ����J�ڂ�����
		if (stateMachine_->GetCurrentStateId() != STATE::STUN) 
		{
			stateMachine_->ChangeState(STATE::STUN);
		}

		// �X�^�����̓X�e�[�g�}�V���̍X�V�̂ݍs��(���̏����͍s��Ȃ�)
		stateMachine_->Update(*this);
		return;
	}

	// CombatController �̓����N�[���_�E���^�C�}�[��U���Ǘ��̍X�V
	// �N�[���_�E�����̍U���𐧌�������A�đI���\�ȏ�Ԃɖ߂�����
	if (combatController_) 
	{
		combatController_->Update(dt);
	}

	// ���C���X�e�[�g�}�V���̍X�V�ƁA�U���̑I�����W�b�N
	if (stateMachine_)
	{
		// ���݂̃X�e�[�g���uCHASE�v�܂��́uIDLE�v��Ԃł���ꍇ�A
		// ���̍U�����J�n����`�����X�Ƃ��Ĉ���
		if (stateMachine_->GetCurrentStateId() == STATE::CHASE ||
			stateMachine_->GetCurrentStateId() == STATE::IDLE)
		{
			// CombatController�ɍU������I�΂���(������󋵂ɉ����đI��)
			EnemyBoss::ATK_STATE nextAttack = combatController_->SelectAttackCandidate();
			
			// �U�����I�΂ꂽ�ꍇ�A(NONE�ȊO)�ɍU����Ԃ֑J��
			if (nextAttack != EnemyBoss::ATK_STATE::NONE) {
				
				// ���C���̃X�e�[�g���u�U��(ATTACK)�v��ԂɕύX
				ChangeState(STATE::ATTACK);

				// �I�����ꂽ����̍U���p�^�[����AttackStateMachine�ɐݒ�
				SetNextAttackType(nextAttack);

				// �I�����ꂽ�U���ɑΉ�����N�[���_�E����ݒ�
				combatController_->SetAttackCooldown(nextAttack);

				// �U����Ԃ֑J�ڂ����̂ŁA����ȍ~�̍X�V�̓X�L�b�v
				return;
			}
		}

		// �U���I�����Ȃ������ꍇ��A���̃X�e�[�g�̏ꍇ�̍X�V
		stateMachine_->Update(*this);
	}

	// �U���̓A�b�v�G�t�F�N�g�̈ʒu���{�X�̌��݂̍��W�ɒǏ]������
	SetPosPlayingEffekseer3DEffect(
				effectBuffPlayId_, 
				transform_.pos.x, transform_.pos.y, transform_.pos.z
				);

	// �d�͉e���v�Z
	CalcGravityPow();

	// �Փ˔��菈��(�n�ʂ�v���C���[�Ƃ̐ڐG�Ȃ�)
	Collision();

	// ���f������X�V
	transform_.Update();
}

void EnemyBoss::Draw(void)
{
	// ����̕`��
	weapon_->Draw();

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// �f�o�b�N�̂�
#ifdef _DEBUG
	DrawDebug();
#endif // DEBUG

}

void EnemyBoss::SetPosition(VECTOR pos)
{
	transform_.pos = pos;
}

std::shared_ptr<EnemyStateMachine> EnemyBoss::GetStateMachine() const
{
	return stateMachine_;
}

std::shared_ptr<EnemyBossAnimation> EnemyBoss::GetAnimation() const
{
	return animation_;
}

std::shared_ptr<EnemyBossCombatSystem> EnemyBoss::GetAttackStateSystem() const
{
	return combat_;
}

std::shared_ptr<EnemyBossCombatController> EnemyBoss::GetAttackStateController() const
{
	return combatController_;
}

std::shared_ptr<EnemyBossAttackStateMachine> EnemyBoss::GetAttackStateMachine() const
{
	return attackFSM_;
}

void EnemyBoss::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void EnemyBoss::ClearCollider(void)
{
	colliders_.clear();
}

const std::shared_ptr<Capsule> EnemyBoss::GetCapsule(void) const
{
	return capsule_;
}

void EnemyBoss::DrawDebug(void)
{
	// �f�o�b�N�\��(��ԃX�e�[�g)
	if (stateMachine_)
	{
		VECTOR screenPos = { transform_.pos.x,transform_.pos.y + DEBUG_STATE_POS_Y,transform_.pos.z };
		VECTOR screen2D = ConvWorldPosToScreenPos(screenPos);
		if (screen2D.z >= 0.0f)
		{
			stateMachine_->DrawDebugInfo(screen2D);
		}
	}

	// �f�o�b�N�\��(�U���X�e�[�g)
	if (attackFSM_)
	{
		VECTOR screenPos = { transform_.pos.x,transform_.pos.y + DEBUG_ATTACK_POS_Y,transform_.pos.z };
		VECTOR screen2D = ConvWorldPosToScreenPos(screenPos);
		attackFSM_->DrawDebugInfo(screen2D);
	}

	DrawFormatString(DEBUG_POS_X, DEBUG_DIST_POS_Y, 0xffffff, "dist_=%f", GetPlayerDistanceXZ());
	DrawFormatString(DEBUG_POS_X, DEBUG_DEADTIME_POSY, 0xffffff, "deadt_=%f", deadTimer_);
	DrawFormatString(DEBUG_POS_X, DEBUG_ATTACKBUFF_POSY, 0xffffff, "bt_=%f", attackBuffTimer_);
}

VECTOR EnemyBoss::GetWeaponCenter(void)
{
	VECTOR top = weaponTopPos_;
	VECTOR down = weaponDownPos_;

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}

void EnemyBoss::UpdateWeapon(void)
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

	// �E�x�N�g���Ə�x�N�g�����ȈՓI�ɋ��߂�
	// �����
	VECTOR upVec = WEAPON_UPVEC;

	// ��̌����Əォ��E���������
	VECTOR rightVec = VCross(upVec, dir);

	// ���K��
	rightVec = VNorm(rightVec); 

	// ������Ē���
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

void EnemyBoss::UpdateWeaponCapsule()
{
	// ��񂩂��̌����x�N�g�������߂�
	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	// ��������x�N�g��
	VECTOR dir = VSub(weaponTopPos_, weaponDownPos_);

	// ���K�����ĒP�ʃx�N�g���ɂ���
	dir = VNorm(dir); 

	// �ʒu������
	float forwardOffset = WEAPON_CAPSULE_FORWARD_OFFSET;
	float upOffset = WEAPON_CAPSULE_UP_OFFSET;
	float rightOffset = WEAPON_CAPSULE_RIGHT_OFFSET;

	// �E�x�N�g���Ə�x�N�g�����ȈՓI�ɋ��߂�
	
	// �����
	VECTOR upVec = WEAPON_CAPSULE_UPVEC;
	
	// ��̌����Əォ��E���������
	VECTOR rightVec = VCross(upVec, dir);
	
	// ���K��
	rightVec = VNorm(rightVec); 

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

void EnemyBoss::InitEffect(void)
{
	// �o�t�G�t�F�N�g
	effectBuffResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BUFF_EFFECT).handleId_;

	SetScalePlayingEffekseer3DEffect(effectBuffPlayId_, BUFF_EFFECT_SIZE, BUFF_EFFECT_SIZE, BUFF_EFFECT_SIZE);

	// �X�^���G�t�F�N�g
	effectStunResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::STUN_EFFECT).handleId_;

	SetScalePlayingEffekseer3DEffect(effectStunPlayId_, BUFF_EFFECT_SIZE, BUFF_EFFECT_SIZE, BUFF_EFFECT_SIZE);

}

void EnemyBoss::InitSound(void)
{
	sHandle_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::ULT_SE).handleId_;
	ChangeVolumeSoundMem(BUFF_EFFECT_SOUND, sHandle_);

	isSound_ = false;
}

int EnemyBoss::GetHp(void) const
{
	return hp_;
}

void EnemyBoss::ChangeState(STATE newState)
{
	if (stateMachine_)
	{
		stateMachine_->ChangeState(newState);
	}
}

void EnemyBoss::ChangeAttack(ATK_STATE newAttack)
{
	if (attackFSM_)
	{
		attackFSM_->Change(*this, newAttack);

	}
}

void EnemyBoss::SetNextAttackType(ATK_STATE type)
{
	nextAttackType_ = type;
}

EnemyBoss::ATK_STATE EnemyBoss::GetNextAttackType() const
{
	return nextAttackType_;
}

Player& EnemyBoss::GetPlayer()
{
	return player_;
}

Transform& EnemyBoss::GetTransformMutable()
{
	return transform_;
}

void EnemyBoss::SetStunned(float duration)
{
	isStunned_ = true;
	stunTimer_ = duration;
}

bool EnemyBoss::IsStunned(void) const
{
	return isStunned_;
}

float EnemyBoss::GetStunTimer(void) const
{
	return stunTimer_;
}

void EnemyBoss::SetAttackBuff(float duration, float muktiplier)
{
	attackBuffTimer_ = duration;
	currentAttackMultiplier_ = muktiplier;
}

void EnemyBoss::RemoveAttackBuff(void)
{
	attackBuffTimer_ = 0.0f;
	currentAttackMultiplier_ = 1.0f;
}

float EnemyBoss::GetCurrentAttackMultiplier() const
{
	return attackBuffTimer_;
}

bool EnemyBoss::IsDead() const
{
	return isDead_;
}

void EnemyBoss::SetIsDead(bool flag)
{
	isDead_ = flag;
}

void EnemyBoss::SetDead()
{
	ChangeState(STATE::DEAD);
	isStunned_ = false;
	stunTimer_ = 0.0f;
	SceneManager::GetInstance().GetCamera()->ChangeGameCamera(Camera::GAME_CAMERA::MOUSE);
}

float EnemyBoss::GetDeadTimer() const
{
	return deadTimer_;
}

void EnemyBoss::OnDeathAnimationFinished(void)
{
	deadTimer_ = DEAD_INTERVAL_TIME;
}

void EnemyBoss::StartStunEffect(void)
{
	if (!isStunEffectActive_)
	{
		isStunEffectActive_ = true;
	}

	effectStunPlayId_ = PlayEffekseer3DEffect(effectStunResId_);

	SetScalePlayingEffekseer3DEffect(effectStunPlayId_, STUN_EFFECT_SIZE, STUN_EFFECT_SIZE, STUN_EFFECT_SIZE);

	SetPosPlayingEffekseer3DEffect(effectStunPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void EnemyBoss::StopStunEffect(void)
{
	if (isStunEffectActive_) 
	{
		isStunEffectActive_ = false;
		StopEffekseer3DEffect(effectStunPlayId_);
	}
}

bool EnemyBoss::IsStunEffectActive(void)
{
	return isStunEffectActive_;
}

void EnemyBoss::StartSetupEffect()
{
	if (!isSetUpEffectActive_)
	{
		isSetUpEffectActive_ = true;
	}

	effectBuffPlayId_ = PlayEffekseer3DEffect(effectBuffResId_);

	SetScalePlayingEffekseer3DEffect(effectBuffPlayId_, BUFF_EFFECT_SIZE, BUFF_EFFECT_SIZE, BUFF_EFFECT_SIZE);

	SetPosPlayingEffekseer3DEffect(effectBuffPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void EnemyBoss::StopSetupEffect()
{
	if (isSetUpEffectActive_) 
	{
		isSetUpEffectActive_ = false;
		StopEffekseer3DEffect(effectBuffPlayId_);
	}
}

bool EnemyBoss::IsSetupEffectActive()
{
	return isSetUpEffectActive_;
}

VECTOR EnemyBoss::GetTopPos()
{
	return weaponTopPos_;
}

VECTOR EnemyBoss::GetDownPos()
{
	return weaponDownPos_;
}

int EnemyBoss::GetMaxHp() const
{
	return maxHp_;
}

float EnemyBoss::GetHpRatio() const
{
	if (maxHp_ <= 0)
	{
		return 0.0f;
	}
	else
	{
		return static_cast<float>(hp_) / maxHp_;
	}
}

EnemyBoss::STATE EnemyBoss::GetCurrentStateId() const
{
	return stateMachine_->GetCurrentStateId();
}

float EnemyBoss::GetPlayerDistanceXZ() const
{
	return VSize(toPlayer_);
}

VECTOR EnemyBoss::GetToPlayer()
{
	return toPlayer_;
}

void EnemyBoss::InitModel(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_BOSS));
	transform_.scl = { SIZE, SIZE, SIZE };
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ INIT_QUAROT_LOCAL_X, AsoUtility::Deg2RadF(INIT_QUAROT_LOCAL_Y), INIT_QUAROT_LOCAL_Z });
	transform_.Update();
}

void EnemyBoss::InitParam(void)
{
	// �p�����[�^�̊�{�ݒ�
	hp_ = MAX_HP_VALUE;
	maxHp_ = hp_;
	damageHp_ = hp_;
	isDead_ = false;
	nextAttackType_ = ATK_STATE::NONE;
	hasUsedSetupBelow40Percent_ = false;
}

void EnemyBoss::InitStateMachine(void)
{
	stateMachine_->Add(STATE::IDLE, std::make_shared<EIdleState>());
	stateMachine_->Add(STATE::OBSERVE, std::make_shared<EObserveState>());
	stateMachine_->Add(STATE::MOVE, std::make_shared<EMoveState>());
	stateMachine_->Add(STATE::CHASE, std::make_shared<EChaseState>());
	stateMachine_->Add(STATE::ATTACK, std::make_shared<EAttackState>());
	stateMachine_->Add(STATE::STEP, std::make_shared<EStepState>());
	stateMachine_->Add(STATE::STUN, std::make_shared<EStunState>());
	stateMachine_->Add(STATE::RECOVER, std::make_shared<ERecoverState>());
	stateMachine_->Add(STATE::DEAD, std::make_shared<EDeadState>());

}

void EnemyBoss::InitAnimation(void)
{
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::IDLE, "idle.mv1", IDLE_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::WALK_F, "Walk Forward.mv1", WALK_F_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::WALK_B, "Standing Walk Back.mv1", WALK_B_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::RUN, "Unarmed Run Forward.mv1", RUN_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::STEP, "Jump.mv1", STEP_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::COMBO, "Standing Melee Attack Downward.mv1", COMBO_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::THRUST, "Upward Thrust.mv1", THRUST_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::ROAR, "Standing Taunt Battlecry.mv1", ROAR_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::EXPLOSION, "Sword And Shield Casting.mv1", EXPLOSION_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::STUN_IDLE, "Standing React Large From Left.mv1", STUN_IDLE_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::SETUP, "Standing React Large From Left.mv1", SET_UP_ANIM_SPEED);
	animation_->AddAnimation(EnemyBossAnimation::ANIM_TYPE::DEATH, "Two Handed Sword Death.mv1", DEATH_ANIM_SPEED);

	animation_->Play(EnemyBossAnimation::ANIM_TYPE::IDLE, true, ANIM_BLEND_RATE);
}

void EnemyBoss::InitAttackPattern(void)
{
	attackFSM_->Add(EnemyBoss::ATK_STATE::COMBO, std::make_shared<ComboAttack>(*this));
	attackFSM_->Add(EnemyBoss::ATK_STATE::THRUST, std::make_shared<ThrustAttack>(*this));
	attackFSM_->Add(EnemyBoss::ATK_STATE::ROAR, std::make_shared<RoarAttack>(*this));
	attackFSM_->Add(EnemyBoss::ATK_STATE::EXPLOSION, std::make_shared<ExplosionAttack>(*this));
	attackFSM_->Add(EnemyBoss::ATK_STATE::SETUP, std::make_shared<SetUpAttack>(*this));
}

void EnemyBoss::Collision(void)
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

void EnemyBoss::CollisionGravity(void)
{
	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// ���㔻�莞�̉��o��
	float checkPow = GRAVITY_CHECK_POW;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));

	//�V��Փ˃`�F�b�N�̂��߂ɍL�߂�2.0f�������Ĕ�����Ƃ�
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto& c : colliders_)
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

void EnemyBoss::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
	for (const auto& c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			for (int tryCnt = 0; tryCnt < CAPSULE_LIMIT_COUNT; tryCnt++)
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
		MV1CollResultPolyDimTerminate(hits);
	}
}

void EnemyBoss::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// ����
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}
}

void EnemyBoss::Damage(void)
{
	float speed = DECREASE_SPEED; // �������x����

	// �_���[�W���̔����Q�[�W��������茸�炷
	if (damageHp_ > hp_)
	{
		damageHp_ -= static_cast<int>(speed);
		if (damageHp_ < hp_) damageHp_ = hp_;
	}
}

void EnemyBoss::DrawHpBar(void)
{
	int width = WIDTH;
	int height = HEIGHT;

	int x = (Application::SCREEN_SIZE_X - width) / 2 + X_ADJUSTMENT;
	int y = Application::SCREEN_SIZE_Y - height - Y_ADJUSTMENT;

	float hpRate = static_cast<float>(hp_) / maxHp_;
	int filledWidth = width;

	// �G�̖��O�\������
	const char* enemyName = "��̕��m";
	SetFontSize(FONT_SIZE);
	int nameWidth = GetDrawStringWidth(enemyName, static_cast<int>(strlen(enemyName)));
	int nameHeight = NAME_HEIGHT;

	int paddingLeft = PADDING_LEFT;
	int nameX = x + paddingLeft;
	int nameY = y - nameHeight - PADDING;

	int padding = PADDING;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, NAME_BLEND_PARAM);
	DrawBox(nameX - padding / 2, nameY - padding / 2,
	nameX + nameWidth + padding, nameY + nameHeight + padding / 2,
	GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int outlineColor = GetColor(0, 0, 0);
	int textColor = GetColor(TEXCOL_R, TEXCOL_G, TEXCOL_B);
	DrawString(nameX - 1, nameY, enemyName, outlineColor);
	DrawString(nameX + 1, nameY, enemyName, outlineColor);
	DrawString(nameX, nameY - 1, enemyName, outlineColor);
	DrawString(nameX, nameY + 1, enemyName, outlineColor);
	DrawString(nameX, nameY, enemyName, textColor);

	// �F�ݒ�
	int bgColor = GetColor(A_QUARTER_COL, A_QUARTER_COL, A_QUARTER_COL);          // �o�[�w�i�i�Â��D�F�j
	
	// �_���[�W�F�͔����x�[�X�ɐԂ̃O���f�[�V�����œ��ߓI�ɕ\��
	int damageColorLight = GetColor(DAMEGE_COL_LIGHT_R, DAMEGE_COL_LIGHT_GB, DAMEGE_COL_LIGHT_GB);		// �_���[�W�������邢�F�i�����ۂ��ԁj
	int damageColorTop = GetColor(DAMEGE_COL_TOP_R, DAMEGE_COL_TOP_GB, DAMEGE_COL_TOP_GB);				// �_���[�W�㔼���i���邢�ԁj
	int damageColorBottom = GetColor(DAMEGE_COL_BOTTOM_R, DAMEGE_COL_BOTTOM_GB, DAMEGE_COL_BOTTOM_GB);  // �_���[�W�������i�Â��ԁj

	int hpColorTop = GetColor(HP_COL_TOP_R, HP_COL_TOP_GB, HP_COL_TOP_GB);				// HP�o�[�㔼���i���邢�ԁj
	int hpColorBottom = GetColor(HP_COL_BOTTOM_R, HP_COL_BOTTOM_GB, HP_COL_BOTTOM_GB);  // HP�o�[�������i�Â��ԁj
	int borderDark = GetColor(BORDER_DARK_COL, BORDER_DARK_COL, BORDER_DARK_COL);       // �O�g�i�Â߁j
	int borderLight = GetColor(BORDER_LIGHT_COL, BORDER_LIGHT_COL, BORDER_LIGHT_COL);   // ���g�i����߁j

	// �O�g�i2�d�ŗ��̊��j
	DrawBox(x - BORDER_DARK_ADJUSTMENT, y - BORDER_DARK_ADJUSTMENT, x + width + BORDER_DARK_ADJUSTMENT, y + height + BORDER_DARK_ADJUSTMENT, borderDark, TRUE);
	DrawBox(x - BORDER_LIGHT_ADJUSTMENT, y - BORDER_LIGHT_ADJUSTMENT, x + width + BORDER_LIGHT_ADJUSTMENT, y + height + BORDER_LIGHT_ADJUSTMENT, borderLight, TRUE);

	// �w�i�o�[
	DrawBox(x, y, x + width, y + height, bgColor, TRUE);

	if (hp_ > 0)
	{
		// �_���[�W�����̕`��i���������O���f�[�V�����j
		int damageW = static_cast<int>(filledWidth * damageHp_ / maxHp_);
		if (damageW > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, DAMAGE_BLEND_PARAM);

			// ���x�[�X�̓��ߓI�Ȑԕ���
			DrawBox(x, y, x + damageW, y + height, damageColorLight, TRUE);

			// �Ԃ̃O���f�[�V�����㉺�ɕ`��
			int halfH = height / 2;
			DrawBox(x, y, x + damageW, y + halfH, damageColorTop, TRUE);
			DrawBox(x, y + halfH, x + damageW, y + height, damageColorBottom, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// ����HP�o�[�i�㉺�ŐF�����j
		int hpW = static_cast<int>(filledWidth * hpRate);
		if (hpW > 0)
		{
			int halfH = height / 2;
			DrawBox(x, y, x + hpW, y + halfH, hpColorTop, TRUE);
			DrawBox(x, y + halfH, x + hpW, y + height, hpColorBottom, TRUE);
		}
	}

	// �g���i�O�̂��߁j
	DrawBox(x, y, x + width, y + height, textColor, FALSE);
}

void EnemyBoss::LookAt(VECTOR pos)
{
	VECTOR BossPos = transform_.pos;
	VECTOR targetPosXZ = pos;
	targetPosXZ.y = BossPos.y;//y������(������]�̂�)

	VECTOR dir = VSub(targetPosXZ, BossPos);
	float length = AsoUtility::MagnitudeF(dir);
	if (length < LENGTH_LIMIT)return;

	dir = VNorm(dir);
	Quaternion lookRot = Quaternion::LookRotation(dir);

	const float slerpRotio = SLERP_ROTIO;
	// �X���[�Y�ɉ�](���ʐ��`���)
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, lookRot, slerpRotio);
}

void EnemyBoss::MoveToward(const VECTOR& target)
{
	VECTOR dir = AsoUtility::VNormalize(VSub(target, transform_.pos));
	transform_.pos.x += dir.x * MOVE_SPEED;
	transform_.pos.z += dir.z * MOVE_SPEED;
}

void EnemyBoss::DrawCircleOnGround(VECTOR center, float radius, int segment, int color)
{
	float angleStep = static_cast<float>(DX_TWO_PI) / segment;
	for (int i = 0; i < segment; ++i) 
	{
		float angle1 = i * angleStep;
		float angle2 = (i + 1) * angleStep;

		VECTOR p1 = VAdd(center, VGet(cosf(angle1) * radius, 0.0f, sinf(angle1) * radius));
		VECTOR p2 = VAdd(center, VGet(cosf(angle2) * radius, 0.0f, sinf(angle2) * radius));

		DrawLine3D(p1, p2, color);
	}
}

bool EnemyBoss::IsPlayerInFront(float viewAngleDeg, float maxDistance)
{
	VECTOR playerPos = GetPlayer().GetTransform().pos;
	VECTOR bossPos = transform_.pos;
	VECTOR toPlayer = VSub(playerPos, bossPos);

	// ��������
	float distToPlayer = AsoUtility::MagnitudeF(toPlayer);
	if (distToPlayer > maxDistance) 
	{
		return false; // �v���C���[������͈͂̍ő勗���O�ɂ���
	}

	// �G�̐��ʕ����x�N�g�����v�Z
	// transform_.quaRot ���� Yaw (Y����]) �p�x���擾
	VECTOR rad = Quaternion::ToEuler(transform_.quaRot);

	// Quaternion::ToEuler �� XYZ �̏��ŃI�C���[�p��Ԃ��̂ŁAY����]�� rad.y �ɑΉ�
	float bossYaw = rad.y; // �{�X��Y����]�i���E�̌����j

	// Y����]���琳�ʃx�N�g�����v�Z
	// �����������������v�Z���Ɠ������@
	float forwardX = sinf(bossYaw);
	float forwardZ = cosf(bossYaw);
	VECTOR bossForward = VGet(forwardX, 0.0f, forwardZ); // Y���͖��� (�n�ʂɕ��s�ȕ���)

	// �G����v���C���[�ւ̕����x�N�g���ƁA�G�̐��ʕ����x�N�g���̂Ȃ��p�x���v�Z
	toPlayer.y = 0.0f; // �v���C���[�ւ̃x�N�g����Y���͖������A���������̔���ɂ���
	toPlayer = VNorm(toPlayer); // ���K��

	// ���ς��g���Ċp�x�̃R�T�C���l���擾
	float dotProduct = VDot(bossForward, toPlayer);

	// ���������_�덷�ɂ���������邽�߃N�����v
	dotProduct = fmaxf(-1.0f, fminf(1.0f, dotProduct));

	// �Ȃ��p�x�i���W�A���j���v�Z
	float angleToPlayerRadians = acosf(dotProduct);

	// ����p��臒l�����W�A���ɕϊ� (viewAngleDeg �͕Б��̊p�x)
	float viewAngleRadians = AsoUtility::Deg2RadF(viewAngleDeg);

	// �Ȃ��p�x������p�͈͓̔�������
	return angleToPlayerRadians <= viewAngleRadians;
}

void EnemyBoss::CollisionMist(void)
{
	for (int h = 0; h < mist_->GetMistTransforms().size(); h++)
	{
		MV1RefreshCollInfo(mist_->GetTransform().modelId);

		// �J�v�Z���Ƃ̏Փ˔���
		auto hits = MV1CollCheck_Capsule(
			mist_->GetMistTransforms()[h].modelId, -1,
			capsule_->GetPosTop(), capsule_->GetPosDown(), CAPSULE_RADIUS);

		// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
		// �v���C���[�̈ʒu���ړ�������
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
			// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
			// �Փ˂����|���S���̖@�������������肷��
			for (int tryCnt = 0; tryCnt < 12; tryCnt++)
			{
				// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
				// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
				int pHit = HitCheck_Capsule_Triangle(
					capsule_->GetPosTop(), capsule_->GetPosDown(), CAPSULE_RADIUS,
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					// �@���̕����ɂ�����Ƃ����ړ�������
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					// �J�v�Z�����ꏏ�Ɉړ�������
					transform_.pos = movedPos_;
					transform_.Update();
					continue;
				}
				break;
			}
		}
		MV1CollResultPolyDimTerminate(hits);
	}
}

float EnemyBoss::GetStepCooldown(void)
{
	return stepCooldown_;
}

void EnemyBoss::SetStepCooldown(float duration)
{
	stepCooldown_ = duration;
}

void EnemyBoss::SetMist(std::shared_ptr<Mist> mist)
{
	mist_ = mist;
}
