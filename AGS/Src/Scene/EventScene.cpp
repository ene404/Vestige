#define NOMINMAX
#include <algorithm>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/EventStage.h"
#include "../Object/Stage/Mist.h"
#include "../Object/Actor/Player/EventPlayer.h"
#include "../Object/Actor/Enemy/EventEnemy.h"
#include "EventScene.h"


EventScene::EventScene(void)
{
	
	currentState_ = STATE::START;
	stateTimer_ = 0.0f;
	longPressStartTime = 0;
	isLongPressing = false;
	customFontHandle_ = -1;
	isStateFrame_ = false;
}

EventScene::~EventScene(void)
{
	currentState_ = STATE::START;
	stateTimer_ = 0.0f;
	if (customFontHandle_ != -1)
	{
		DeleteFontToHandle(customFontHandle_);
	}
}

void EventScene::Init(void)
{
	// �v���C���[
	player_ = std::make_shared<EventPlayer>();
	player_->Init();

	// �G
	enemy_ = std::make_shared<EventEnemy>(*player_);
	enemy_->Init();

	// �X�e�[�W
	stage_ = std::make_shared<EventStage>(*player_, *enemy_);
	stage_->Init();

	// ��
	mist_ = std::make_shared<Mist>();
	mist_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(EventStage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	InitSound();

	// �t�H���g�̃��[�h
	customFontHandle_ = CreateFontToHandle("Garamond", FONT_SIZE, FONT_THICK, DX_FONTTYPE_EDGE);
	if (customFontHandle_ == -1) {
		return;
	}
	ChangeFont("Garamond");

	// �J�����̐ݒ�
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::EVENT);

	SceneManager::GetInstance().GetCamera()->ResetEventCameraState();

	currentState_ = STATE::START;
	isLongPressing = false;
	longPressStartTime = 0; // �������J�n������������
	progressColor = GetColor(125, 252, 0);
}

void EventScene::Update(void)
{
	float dt = SceneManager::GetInstance().GetDeltaTime();
	stateTimer_ += dt;

	if (!isBgm_)
	{
		PlaySoundMem(bgmSHandle_, DX_PLAYTYPE_LOOP, true);
		isBgm_ = true;
	}

	InputManager& ins = InputManager::GetInstance();

	// Enter�L�[�܂���A�{�^����������Ă��邩���`�F�b�N
	bool isPressingButton = CheckHitKey(KEY_INPUT_RETURN) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);

	if (isPressingButton)
	{
		if (!isLongPressing) 
		{
			// �������J�n
			isLongPressing = true;
			longPressStartTime = GetNowCount();
			PlaySoundMem(iKeySHandle_, DX_PLAYTYPE_LOOP, true);
		}
	}
	else 
	{ 
		// ���N���b�N�������ꂽ�A�܂��̓{�^���̈�O�Ƀ}�E�X������
		// ���������f
		StopSoundMem(iKeySHandle_);
		isLongPressing = false;
		longPressStartTime = 0; // ���Z�b�g
	}

	if (isLongPressing) 
	{
		int currentTime = GetNowCount();
		int elapsedTime = currentTime - longPressStartTime;

		// ��������������
		if (elapsedTime >= LONG_PRESS_DURATION_MS) 
		{
			// �����������I�V�[���J��
			StopSoundMem(iKeySHandle_);
			PlaySoundMem(cMaxSHandle_, DX_PLAYTYPE_BACK, true);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
	}

	if (stateTimer_ == 0.0f)
	{
		isStateFrame_ = true;
	}
	else 
	{
		isStateFrame_ = false;
	}

	switch (currentState_)
	{
	case STATE::START:
		player_->ChangeState(EventPlayer::STATE::WALK);
		if (stateTimer_ >= START_TIME_LIMIT)
		{
			currentState_ = STATE::STEP_PLAYER_FORWARD;
			stateTimer_ = 0.0f;
		}
		break;
	case EventScene::STATE::STEP_PLAYER_FORWARD:
		player_->SetGoalRotate(0.0f);
		player_->ChangeState(EventPlayer::STATE::WALK);
		if (player_->GetTransform().pos.z > PLAYER_MOVED_LIMIT)
		{
			currentState_ = STATE::LOOK_AROUND;
			stateTimer_ = 0.0f;
		}
		break;
	case EventScene::STATE::LOOK_AROUND:
		player_->ChangeState(EventPlayer::STATE::LOOK);
		if (stateTimer_ >= LOOK_TIME_LIMIT)
		{
			currentState_ = STATE::CAMERA_PAN_TO_ENEMY;
			stateTimer_ = 0.0f;
		}
		break;
	case EventScene::STATE::CAMERA_PAN_TO_ENEMY:
		player_->ChangeState(EventPlayer::STATE::IDLE);
		enemy_->Update();
		if (stateTimer_ > PAN_TIME_LIMIT_FIRST) 
		{
			enemy_->ChangeState(EventEnemy::STATE::WALK);
		}
		if (stateTimer_ > PAN_TIME_LIMIT_SECOND)
		{
			currentState_ = STATE::ENEMY_ROAR;
			stateTimer_ = 0.0f;
			enemy_->ChangeState(EventEnemy::STATE::STANDBY); // ����\��
		}
		break;
	case EventScene::STATE::ENEMY_ROAR:
		enemy_->Update();
		StopEffekseer3DEffect(enemy_->GetEffectRoarFPlayId());
		if (enemy_->IsEndLanding(EventEnemy::ANIM_TYPE::STANDBY))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			stateTimer_ = 0.0f;
		}
		break;
	default:
		break;
	}

	// �ʏ�X�V
	skyDome_->Update();
	player_->Update();
	enemy_->Update();
	stage_->Update();
}

void EventScene::Draw(void)
{
	skyDome_->Draw();
	mist_->Draw();
	stage_->Draw();
	enemy_->Draw();

	if (currentState_ != STATE::CAMERA_PAN_TO_ENEMY)
	{
		player_->Draw();
	}

	// �v���O���X�o�[�̕`��
	// �L�[��������Ă��Ȃ��Ă��v���O���X�o�[�̕`��֐����Ăяo��
	int elapsedTime = isLongPressing ? (GetNowCount() - longPressStartTime) : 0;
	DrawProgressBar(elapsedTime, LONG_PRESS_DURATION_MS);

	// �{�^�����b�Z�[�W�̕`��
	const char* buttonMessage = "Press Enter or A";

	// �J�X�^���t�H���g�ŕ`�悵���ۂ̃��b�Z�[�W�̕��ƍ������擾���܂�
	int msgWidth = GetDrawStringWidthToHandle(buttonMessage, static_cast<int>(strlen(buttonMessage)), customFontHandle_);
	int msgHeight = GetFontSizeToHandle(customFontHandle_);

	// �E�������̂��߂�X���W���v�Z���܂�:
	// ��ʂ̕� - ���b�Z�[�W�̕� - �E����̗]��
	int msgX = Application::SCREEN_SIZE_X - msgWidth - FONT_MARGIN_RIGHT;

	// �E�������̂��߂�Y���W���v�Z���܂�:
	// ��ʂ̍��� - ���b�Z�[�W�̍��� - ������̗]��
	int msgY = Application::SCREEN_SIZE_Y - FONT_MARGIN_UNDER;
	DrawStringToHandle(msgX, msgY, buttonMessage, GetColor(FONT_MAX_COL, FONT_MAX_COL, FONT_MAX_COL), customFontHandle_);
}

void EventScene::DrawUI(void)
{

}

void EventScene::UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)
{
	VECTOR playerPos = GetPlayer()->GetTransform().pos;;
	VECTOR enemyPos = GetEnemy()->GetTransform().pos;
	VECTOR initialPlayerPos = { 0.0f,0.0f, 0.0f };

	// �C�x���g�V�[���ɓ������ŏ��̃t���[���ł̂ݏ����J�����ʒu��ݒ�
	if (!isInitialized_ && GetState() == EventScene::STATE::START && IsStateEnterFrame()) 
	{
		initialPlayerPos = GetPlayer()->GetTransform().pos;
		pos = VAdd(initialPlayerPos, EVENT_CAMERA_POS);
		targetPos = VAdd(initialPlayerPos, EVENT_TARGET_POS);
		isInitialized_ = true; // ����������
	}

	float angle = 0.0f;
	float radius = 0.0f;
	float panAmount = 0.0f;
	float t = 0.0f;
	switch (GetState())
	{
	case EventScene::STATE::START:
		if (!isInitialized_) 
		{
			// �v���C���[�̍��O����ɃJ������z�u
			initialPlayerPos = GetPlayer()->GetTransform().pos;

			// �J�����̈ʒu�F�v���C���[�̉E����
			pos = VAdd(initialPlayerPos, EVENT_START_CAMERA_POS);
			isInitialized_ = true;
		}
		// �^�[�Q�b�g: �v���C���[�̈ʒu�i�v���C���[������j
		targetPos = VAdd(initialPlayerPos, EVENT_START_TARGET_POS);
		break;
	case EventScene::STATE::STEP_PLAYER_FORWARD:
		if (!isInitialized_) 
		{
			// �v���C���[�̍��O����ɃJ������z�u
			initialPlayerPos = GetPlayer()->GetTransform().pos;
			// �J�����̈ʒu�F�v���C���[�̉E����
			pos = VAdd(playerPos, EVENT_STEP_CAMERA_POS);
			isInitialized_ = true;
		}

		pos = VAdd(initialPlayerPos, EVENT_STEP_TARGET_POS); // �����ɌŒ�
		targetPos = GetPlayer()->GetTransform().pos;
		break;

	case EventScene::STATE::LOOK_AROUND:

		// �J�������E�と�����A���と�E���ɃX�C���O
		angle = sinf(GetStateTimer() * 1.0f);
		pos = VAdd(playerPos, VGet(EVENT_LOOK_CAMER_POS_X * angle, EVENT_LOOK_CAMER_POS_Y, EVENT_LOOK_CAMER_POS_Z));
		targetPos = VAdd(playerPos, EVENT_LOOK_TARGET_POS);
		break;

	case EventScene::STATE::CAMERA_PAN_TO_ENEMY:

		t = std::clamp(GetStateTimer() / 2.0f, 0.0f, 1.0f);

		static VECTOR startPanPos;
		static VECTOR endPanPos;

		startPanPos = VAdd(playerPos, EVENT_PAN_CAMERA_START_POS);
		endPanPos = VAdd(playerPos, EVENT_PAN_CAMERA_END_POS);

		pos = AsoUtility::Lerp(startPanPos, endPanPos, t);
		enemyPos = VAdd(enemyPos, EVENT_PAN_ENEMY_POS);
		targetPos = enemyPos;

		break;

	case EventScene::STATE::ENEMY_ROAR:
		// �G�̑��� �� �����
		pos = VAdd(enemyPos, VGet(EVENT_ROAR_CAMERA_POS_X,
			EVENT_ROAR_CAMERA_POS_Y + GetStateTimer() * EVENT_ROAR_TIME_ADJUSTMOMENT, EVENT_ROAR_CAMERA_POS_Z));
		enemyPos = VAdd(enemyPos, EVENT_ROAR_ENEMY_POS);
		targetPos = enemyPos;
		break;
	default:
		break;
	}
}

EventScene::STATE EventScene::GetState()
{
	return currentState_;
}

float EventScene::GetStateTimer()
{
	return stateTimer_;
}

std::shared_ptr<EventPlayer> EventScene::GetPlayer()
{
	return player_;
}

std::shared_ptr<EventEnemy> EventScene::GetEnemy()
{
	return enemy_;
}

bool EventScene::IsStateEnterFrame(void) const
{
	return isStateFrame_;
}

std::string EventScene::GetStateName(STATE state)
{
	switch (state)
	{
	case STATE::STEP_PLAYER_FORWARD: return "STEP_PLAYER_FORWARD";
	case STATE::LOOK_AROUND:         return "LOOK_AROUND";
	case STATE::CAMERA_PAN_TO_ENEMY: return "CAMERA_PAN_TO_ENEMY";
	case STATE::ENEMY_ROAR:          return "ENEMY_ROAR";
	default:                         return "UNKNOWN";
	}
}

void EventScene::DrawProgressBar(int elapsedTime, int totalDuration)
{
	int barX = Application::SCREEN_SIZE_X - BAR_X_ADJUSTMOMENT;
	int barY = Application::SCREEN_SIZE_Y - BAR_Y_ADJUSTMOMENT; // �{�^���̉��ɕ\��
	int barHeight = BAR_HEIGHT;
	int barMaxWidth = BAR_MAX_WIDTH;

	// �i�s�x���v�Z (0.0 �` 1.0)
	double progress = (double)elapsedTime / totalDuration;
	progress = std::max(0.0, std::min(1.0, progress));
	int currentBarWidth = (int)(barMaxWidth * progress);

	// �v���O���X�o�[�̔w�i (���g)
	DrawBox(barX - 1, barY - 1, barX + barMaxWidth + 1, barY + barHeight + 1, GetColor(FONT_MAX_COL, FONT_MAX_COL, FONT_MAX_COL), TRUE);

	// �v���O���X�o�[�̔w�i (�Z���O���[)
	DrawBox(barX, barY, barX + barMaxWidth, barY + barHeight, GetColor(FONT_GRAY_COL, FONT_GRAY_COL, FONT_GRAY_COL), TRUE);

	// �v���O���X�o�[�{�� (��)
	DrawBox(barX, barY, barX + currentBarWidth, barY + barHeight, progressColor, TRUE);

	int underprogreCol = GetColor(50, 205, 50);

	// �v���O���X�o�[�{��(��) (��)
	DrawBox(barX, barY + 10, barX + currentBarWidth, barY + barHeight, underprogreCol, TRUE);

	const char* skipText = "Skip";

	int textWidth = GetDrawStringWidthToHandle(skipText, static_cast<int>(strlen(skipText)), customFontHandle_);
	int fontHeight = GetFontSizeToHandle(customFontHandle_);

	// �e�L�X�g���o�[��Y���W�̒����ɔz�u
	int textX = barX + (barMaxWidth - textWidth) / 2;
	int textY = barY + (barHeight - fontHeight) / 2;

	DrawStringToHandle(textX, textY, skipText, GetColor(FONT_MAX_COL, FONT_MAX_COL, FONT_MAX_COL), customFontHandle_);
}

void EventScene::InitSound(void)
{
	// bgm
	bgmSHandle_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::EVENT_BGM).handleId_;
	ChangeVolumeSoundMem(BGM_VALUME, bgmSHandle_);

	// �Đ������ǂ���
	isBgm_ = false;

	// ���͂���Ă���Ƃ��̉�
	iKeySHandle_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::GAUGE_BGM2).handleId_;
	ChangeVolumeSoundMem(INPUTKEY_VALUME, iKeySHandle_);

	// ���͂����������Ƃ��̉�
	cMaxSHandle_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::GAUGE_BGM1).handleId_;
	ChangeVolumeSoundMem(COUNTMAX_VALUME, cMaxSHandle_);
}