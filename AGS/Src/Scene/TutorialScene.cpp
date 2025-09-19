#include <sstream>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Stage/Mist.h"
#include "../Object/Actor/Player/Player.h"
#include "../Object/Actor/Enemy/EnemyBoss/EnemyBoss.h"
#include "../Object/Actor/Enemy/EnemyScarecrow.h"
#include "../Object/Actor/Enemy/EnemyScarecrowDummy.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene(void)
{
}

TutorialScene::~TutorialScene(void)
{
	if (helpBackImage_ != -1)
	{
		DeleteGraph(helpBackImage_);
		helpBackImage_ = -1;
	}
}

void TutorialScene::Init(void)
{
	ChangeFont("MS �S�V�b�N");

	// ��
	tutorialBgm_ = resMng_.Load(ResourceManager::SRC::TUTORIAL_BGM).handleId_;
	isBgm_ = true;
	ChangeVolumeSoundMem(TUTORIAL_BGM_VOL, tutorialBgm_);

	nextSe_ = resMng_.Load(ResourceManager::SRC::NEXT_SE).handleId_;
	ChangeVolumeSoundMem(NEXT_SE_VOL, nextSe_);

	helpSe_ = resMng_.Load(ResourceManager::SRC::HELP_SE).handleId_;
	ChangeVolumeSoundMem(HELP_SE_VOL, helpSe_);

	// �v���C���[
	player_ = std::make_shared<Player>();
	player_->Init();

	// boss
	enemyBoss_ = std::make_shared<EnemyBoss>(*player_);
	enemyBoss_->Init();

	// ������
	enemyScarecrow_ = std::make_shared<EnemyScarecrow>(*player_);
	enemyScarecrow_->Init();

	// �������_�~�[
	enemyScarecrowDummy_ = std::make_shared<EnemyScarecrowDummy>(*enemyScarecrow_);

	// �X�e�[�W
	stage_ = std::make_unique<Stage>(*player_, *enemyBoss_);
	stage_->Init();

	// �~�X�g
	mist_ = std::make_shared<Mist>();
	mist_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera()->LockonSoundLoad();
	SceneManager::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	SceneManager::GetInstance().GetCamera()->SetFollowEnemy(&enemyScarecrow_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_MOUSE);
	player_->SetEnemyScarecrow(enemyScarecrow_);
	player_->SetEnemyScarecrowDummy(enemyScarecrowDummy_);
	player_->SetMist(mist_);


	tutorialStep_ = TutorialStep::LOOK;

	lookCounter_ = 0;
	prevLookX_ = 0;
	prevLookY_ = 0;

	movedW_ = false;
	movedA_ = false;
	movedS_ = false;
	movedD_ = false;

	dashed_ = false;

	rolled_ = false;

	attacked_ = false;

	comboAttacked_ = false;

	heavyAttacked_ = false;

	lockedOn_ = false;

	usedItem_ = false;

	itemChanged_ = false;

	nextStepTimer_ = 0.0f;

	isHelp_ = false;
	helpBackImage_ = -1;
	wasHelp_ = false;


	// �|�X�g�G�t�F�N�g�p�X�N���[��
	postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// �|�X�g�G�t�F�N�g�p(���C�g�j���O)
	lightningMaterial_ = std::make_unique<PixelMaterial>("Lightning.cso", 1);

	lightningTime_ = GetNowCount() / MILLISECONDS_TO_SECONDS;
	lightningPower_ = 0.0f;

	lightningMaterial_->AddConstBuf({ lightningTime_, lightningPower_, 1.0f, 1.0f });

	lightningMaterial_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	lightningRenderer_ = std::make_unique<PixelRenderer>(*lightningMaterial_);
	lightningRenderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y)
	);
}

void TutorialScene::Update(void)
{
	if (isBgm_)
	{
		PlaySoundMem(tutorialBgm_, DX_PLAYTYPE_LOOP, true);
		isBgm_ = false;
	}
	SceneManager::GetInstance().HelpUpdate();

	InputManager& ins = InputManager::GetInstance();
	wasHelp_ = false;

	if (ins.IsTrgDown(KEY_INPUT_TAB) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::SEVEN))
	{
		PlaySoundMem(helpSe_, DX_PLAYTYPE_BACK, true);
		isHelp_ = !isHelp_;

		if (isHelp_)
		{
			// ���łɍ���Ă�����폜
			if (helpBackImage_ != -1)
			{
				DeleteGraph(helpBackImage_);
				helpBackImage_ = -1;
			}
			// �w���v�p�w�i�摜���쐬
			helpBackImage_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

			// ���݂̃��C����ʂ��R�s�[�i�L���v�`���j
			int mainScreen = SceneManager::GetInstance().GetMainScreen();
			SetDrawScreen(helpBackImage_);
			DrawGraph(0, 0, mainScreen, false);
			SetDrawScreen(mainScreen); // ���ɖ߂�
		}
		else
		{
			wasHelp_ = true;
		}
	}

	if (!isHelp_)
	{
		lightningTime_ = GetNowCount() / MILLISECONDS_TO_SECONDS;
		if (player_->IsLightning())
		{
			lightningPower_ = LIGHTNING_POWER_MAX;
		}
		if (lightningPower_ > 0.0f)
		{
			lightningPower_ -= LIGHTNING_DECAY_RATE;
			if (lightningPower_ < 0.0f) lightningPower_ = 0.0f;
		}

		// �`���[�g���A���i�s����
		switch (tutorialStep_)
		{
		case TutorialStep::LOOK:
			LookUpdate();
			break;
		case TutorialScene::TutorialStep::MOVE:
			MoveUpdate();
			break;
		case TutorialStep::DASH:
			DashUpdate();
			break;
		case TutorialStep::ROLLING:
			RollingUpdate();
			break;
		case TutorialStep::ATTACK:
			AttackUpdate();
			break;
		case TutorialStep::COMBO_ATTACK:
			ComboAttackUpdate();
			break;
		case TutorialStep::HEAVY_ATTACK:
			HeavyAttackUpdate();
			break;
		case TutorialStep::LOCKON:
			LockonUpdate();
			break;
		case TutorialStep::ULT:
			UltUpdate();
			break;
		case TutorialStep::ITEM:
			ItemUpdate();
			break;
		case TutorialStep::ITEM_CHANGE:
			ItemChangeUpdate();
			break;
		case TutorialStep::HELP:
			HelpUpdate();
			break;
		case TutorialStep::DEFEAT_ENEMY:
			DefeatEnemyUpdate();
			break;
		case TutorialStep::NEXT:
			nextStepTimer_ += SceneManager::GetInstance().GetDeltaTime();

			// 5�b��ɃQ�[���V�[����
			if (nextStepTimer_ >= TIME_TO_NEXT_SCENE) {
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EVENT);
			}
			break;
		}


		skyDome_->Update();

		stage_->Update();

		player_->Update();
		player_->CollisionEnemyScarecrow();

		enemyScarecrow_->Update();
		enemyScarecrowDummy_->Update();

		player_->CollisionMist();
	}
}

void TutorialScene::Draw(void)
{
	if (isHelp_ && helpBackImage_ != -1)
	{
		return; // �ʏ�`��̓X�L�b�v
	}

	// �w�i
	skyDome_->Draw();
	mist_->Draw();
	stage_->Draw();

	enemyScarecrow_->Draw();

	player_->Draw();
	SceneManager::GetInstance().GetCamera()->DrawLockonMarker();

	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	// �|�X�g�G�t�F�N�g(���C�g�j���O)
	//-----------------------------------------
	lightningMaterial_->SetConstBuf(0, { lightningTime_, lightningPower_, 0.0f, 0.0f });

	SetDrawScreen(postEffectScreen_);

	// ��ʂ�������
	ClearDrawScreen();

	lightningRenderer_->Draw();

	// ���C���ɖ߂�
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);
	//-----------------------------------------

}

void TutorialScene::DrawUI(void)
{
	if (!isHelp_)
	{
		player_->DrawHp(static_cast<float>(player_->GetHp()), Player::MAX_HP);
		player_->DrawStamina(static_cast<float>(player_->GetStamina()), Player::MAX_STAMINA);
		player_->DrawItem();
		player_->DrawUlt();


		// �`���[�g���A���i�s����
		switch (tutorialStep_)
		{
		case TutorialStep::LOOK:
			DrawTutorialText(
				"�}�E�X or �E�X�e�B�b�N�𓮂�����\n���_�𑀍삵�Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::MOVE:
			DrawTutorialText(
				"WASD�L�[ or ���X�e�B�b�N��\n�ړ����Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::DASH:
			DrawTutorialText(
				"Shift�������Ȃ���ړ� or A�{�^����\n�����Ȃ���ړ�����\n���b�_�b�V�����Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::ROLLING:
			DrawTutorialText(
				"�X�y�[�X�L�[ or B�{�^����\n������Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::ATTACK:
			DrawTutorialText(
				"���N���b�N or RB�{�^����\n�U�����Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::COMBO_ATTACK:
			DrawTutorialText(
				"���N���b�N�A�� or RB�{�^���A�ł�\n3�A���U�������Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::HEAVY_ATTACK:
			DrawTutorialText(
				"�E�N���b�N or RT�{�^����\n���U�����Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::LOCKON:
			DrawTutorialText(
				"�z�C�[���N���b�N or �E�X�e�B�b�N�������݂�\n���b�N�I��/�������Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::ULT:
			DrawTutorialText(
				"Q�L�[ or LB�{�^����\n�K�E�Z���g���Ă݂܂��傤",
				"\n(�g�p���͍U����UP!)");
			break;
		case TutorialStep::ITEM:
			DrawTutorialText(
				"R�L�[ or X�{�^����\n�A�C�e�����g�p���Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::ITEM_CHANGE:
			DrawTutorialText(
				"�}�E�X�z�C�[�� or �\���L�[���E��\n�A�C�e����ύX���Ă݂܂��傤",
				nullptr);
			break;
		case TutorialStep::HELP:
			DrawTutorialText(
				"TAB�L�[ or ���j���[�{�^����������\n���������\�����Ă݂܂��傤",
				"\n�i������x�����ƕ����܂��j"
			);
			break;
		case TutorialStep::DEFEAT_ENEMY:
			DrawTutorialText(
				"�G��|���Ă݂܂��傤�I",
				nullptr);
			enemyScarecrow_->DrawHpBar();
			break;
		case TutorialStep::NEXT:
			int remain = static_cast<int>(ceilf(TIME_TO_NEXT_SCENE - nextStepTimer_));
			if (remain < 0) remain = 0;

			std::ostringstream oss;
			oss << "�`���[�g���A�������I\n�Q�[�����J�n���܂��i" << remain << "�j";

			DrawTutorialText(oss.str().c_str(), nullptr);
			break;
		}
	}
	// �w���v
	if (isHelp_ && helpBackImage_ != -1)
	{
		// �w�i�̃L���v�`����`��
		DrawGraph(0, 0, helpBackImage_, false);
		DrawHelp();
	}
}

void TutorialScene::UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)
{
}

void TutorialScene::DrawTutorialText(const char* text, const char* subText)
{
	const int screenW = Application::SCREEN_SIZE_X;
	const int screenH = Application::SCREEN_SIZE_Y;

	const int boxWidth = static_cast<int>(screenW * TUTORIAL_BOX_WIDTH_RATIO);
	const int boxY = (screenH - TUTORIAL_BOX_HEIGHT) / 2;

	// �y�w�i�F���𔼓����ɂ��Ď��͂�����������悤�Ɂz	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BG_ALPHA);
	DrawRoundRect(TUTORIAL_BOX_X, boxY, TUTORIAL_BOX_X + boxWidth, boxY + TUTORIAL_BOX_HEIGHT, TUTORIAL_BOX_CORNER_RADIUS, TUTORIAL_BOX_CORNER_RADIUS, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �y�g���F�O���ɈÂ��Ԏ��̂ڂ�����鉏���z
	for (int i = 0; i < BORDER_LAYER_COUNT; ++i)
	{
		int alpha = BORDER_ALPHA_START - i * BORDER_ALPHA_STEP;
		if (alpha < 0) alpha = 0;
		DrawRoundRect(
			TUTORIAL_BOX_X - i, boxY - i, TUTORIAL_BOX_X + boxWidth + i, boxY + TUTORIAL_BOX_HEIGHT + i,
			TUTORIAL_BOX_CORNER_RADIUS + i, TUTORIAL_BOX_CORNER_RADIUS + i,
			GetColor(BORDER_COLOR_R, BORDER_COLOR_G, BORDER_COLOR_B) & 0x00FFFFFF | (alpha << ALPHA_SHIFT),
			FALSE
		);
	}

	// �y�g���F�����̎K�т��S���ۂ��O���[�z
	DrawRoundRect(TUTORIAL_BOX_X, boxY, TUTORIAL_BOX_X + boxWidth, boxY + TUTORIAL_BOX_HEIGHT,
		TUTORIAL_BOX_CORNER_RADIUS, TUTORIAL_BOX_CORNER_RADIUS, GetColor(INNER_BORDER_COLOR_R, INNER_BORDER_COLOR_G, INNER_BORDER_COLOR_B), FALSE);

	// �y���C���e�L�X�g�F�₽����D�F�A�������������e�t���z
	SetFontSize(MAIN_FONT_SIZE);
	int mainTextHeight = GetFontSize();

	DrawString(TUTORIAL_BOX_X + TEXT_LEFT_PADDING + SHADOW_OFFSET_2, boxY + TEXT_TOP_PADDING + SHADOW_OFFSET_2, text, GetColor(MAIN_TEXT_SHADOW_COLOR1_R, MAIN_TEXT_SHADOW_COLOR1_G, MAIN_TEXT_SHADOW_COLOR1_B));
	DrawString(TUTORIAL_BOX_X + TEXT_LEFT_PADDING + SHADOW_OFFSET, boxY + TEXT_TOP_PADDING + SHADOW_OFFSET, text, GetColor(MAIN_TEXT_SHADOW_COLOR2_R, MAIN_TEXT_SHADOW_COLOR2_G, MAIN_TEXT_SHADOW_COLOR2_B));
	DrawString(TUTORIAL_BOX_X + TEXT_LEFT_PADDING, boxY + TEXT_TOP_PADDING, text, GetColor(MAIN_TEXT_COLOR_R, MAIN_TEXT_COLOR_G, MAIN_TEXT_COLOR_B));

	// �y�T�u�e�L�X�g�F�����O���[�ŉe�t���z
	if (subText != nullptr)
	{
		SetFontSize(SUB_FONT_SIZE);

		DrawString(TUTORIAL_BOX_X + TEXT_LEFT_PADDING + SHADOW_OFFSET, boxY + TEXT_TOP_PADDING + mainTextHeight + SUBTEXT_SPACING + SHADOW_OFFSET, subText, GetColor(SUB_TEXT_SHADOW_COLOR_R, SUB_TEXT_SHADOW_COLOR_G, SUB_TEXT_SHADOW_COLOR_B));
		DrawString(TUTORIAL_BOX_X + TEXT_LEFT_PADDING, boxY + TEXT_TOP_PADDING + mainTextHeight + SUBTEXT_SPACING, subText, GetColor(SUB_TEXT_COLOR_R, SUB_TEXT_COLOR_G, SUB_TEXT_COLOR_B));
	}

	SetFontSize(SUB_FONT_SIZE);
}


void TutorialScene::LookUpdate(void)
{
	int nowX, nowY;
	GetMousePoint(&nowX, &nowY);

	int dx = nowX - prevLookX_;
	int dy = nowY - prevLookY_;

	if (abs(dx) > LOOK_MOUSE_MOVE_THRESHOLD || abs(dy) > LOOK_MOUSE_MOVE_THRESHOLD) 
	{
		lookCounter_++;
	}

	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (input.Rx > LOOK_STICK_THRESHOLD)
	{
		lookCounter_++;
	}
	if (input.Rx < -LOOK_STICK_THRESHOLD)
	{
		lookCounter_++;
	}
	if (input.Ry > LOOK_STICK_THRESHOLD)
	{
		lookCounter_++;
	}
	if (input.Ry < -LOOK_STICK_THRESHOLD)
	{
		lookCounter_++;
	}


	prevLookX_ = nowX;
	prevLookY_ = nowY;

	if (lookCounter_ > REQUIRED_LOOK_MOVES) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::MOVE;
	}
}

void TutorialScene::MoveUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (ins.IsTrgDown(KEY_INPUT_W) || input.Y < -STICK_MOVE_THRESHOLD) movedW_ = true;
	if (ins.IsTrgDown(KEY_INPUT_A) || input.X < -STICK_MOVE_THRESHOLD) movedA_ = true;
	if (ins.IsTrgDown(KEY_INPUT_S) || input.Y > STICK_MOVE_THRESHOLD) movedS_ = true;
	if (ins.IsTrgDown(KEY_INPUT_D) || input.X > STICK_MOVE_THRESHOLD) movedD_ = true;

	if (movedW_ && movedA_ && movedS_ && movedD_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::DASH;  // DASH��
	}
}

void TutorialScene::DashUpdate(void)
{
	static float runTime = 0.0f;

	if (player_->IsRun()) 
	{
		runTime += SceneManager::GetInstance().GetDeltaTime();
	}

	if (runTime >= REQUIRED_RUN_TIME) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::ROLLING;
		runTime = 0.0f;
	}
}

void TutorialScene::RollingUpdate(void)
{
	if (player_->GetRollingCnt() >= Player::MAX_ROLLING_CNT - JUDGE_MARGIN) 
	{
		rolled_ = true;
	}

	if (rolled_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::ATTACK;
	}
}

void TutorialScene::AttackUpdate(void)
{
	if (player_->GetAttackCnt() >= Player::MAX_ATTACK_CNT - JUDGE_MARGIN) 
	{
		attacked_ = true;
	}

	if (attacked_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::COMBO_ATTACK;
	}
}

void TutorialScene::ComboAttackUpdate(void)
{
	if (player_->GetAttackCnt2() <= 0.0f && player_->GetAttackCnt3() >= Player::MAX_ATTACK_CNT_3 - JUDGE_MARGIN) 
	{
		comboAttacked_ = true;
	}

	if (comboAttacked_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::HEAVY_ATTACK;
	}
}

void TutorialScene::HeavyAttackUpdate(void)
{
	if (player_->GetStrongAttackCnt() >= Player::MAX_STRONG_ATTACK_CNT - JUDGE_MARGIN) 
	{
		heavyAttacked_ = true;
	}

	if (heavyAttacked_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::LOCKON;
	}
}

void TutorialScene::LockonUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();

	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (ins.IsTrgMouseMiddle() || input.Buttons[PAD_BUTTON_LOCKON]) 
	{
		lockedOn_ = true;
	}

	if (lockedOn_) 
	{
		player_->TutorialUlt();
		player_->SetUltValue(Player::ULT_MAX_VALUE);
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::ULT;
	}
}

void TutorialScene::UltUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (ins.IsTrgDown(KEY_INPUT_Q) || input.Buttons[PAD_BUTTON_ULT]) 
	{
		usedUlt_ = true;
	}

	if (usedUlt_) 
	{
		player_->Damage(TUTORIAL_DAMAGE);
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::ITEM;
	}
}

void TutorialScene::ItemUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (ins.IsTrgDown(KEY_INPUT_R) || input.Buttons[PAD_BUTTON_ITEM]) 
	{
		usedItem_ = true;
	}

	if (usedItem_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::ITEM_CHANGE;
	}
}

void TutorialScene::ItemChangeUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// �z�C�[���̗ݐω�]�ʂ� static �ŕێ�
	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();

	// ��]�ʂ�~��
	wheelCounter += wheelDelta;

	if (wheelCounter >= WHEEL_THRESHOLD || input.POV[0] == POV_RIGHT) 
	{
		itemChanged_ = true;
	}
	else if (wheelCounter <= -WHEEL_THRESHOLD || input.POV[0] == POV_LEFT)
	{
		itemChanged_ = true;
	}
	if (itemChanged_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::HELP;
	}
}

void TutorialScene::HelpUpdate(void)
{
	if (wasHelp_) 
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::DEFEAT_ENEMY;
	}
}

void TutorialScene::DefeatEnemyUpdate(void)
{
	if (player_->IsLightning()) 
	{
		enemyScarecrow_->Damage(LIGHTNING_DAMAGE);
	}
	if (player_->IsHitAttackEnemyScarecrow()) 
	{
		enemyScarecrow_->Damage(player_->GetAttack());
	}
	if (!enemyScarecrow_->IsAlive())
	{
		PlaySoundMem(nextSe_, DX_PLAYTYPE_BACK, true);
		tutorialStep_ = TutorialStep::NEXT;
		nextStepTimer_ = 0.0f;
	}
}

void TutorialScene::DrawHelp(void)
{
	const int screenWidth = Application::SCREEN_SIZE_X;
	const int screenHeight = Application::SCREEN_SIZE_Y;

	// === �e�L�X�g�ݒ� ===
	const int lineHeight = HELP_FONT_SIZE + HELP_LINE_SPACING;
	SetFontSize(HELP_FONT_SIZE);

	// �R�}���h�Ɛ����ɕ�����
	const char* helpLabels[] = 
	{
		"���_����", "�ړ�", "�_�b�V��", "���", "�U��", "���U��",
		"���b�N�I��", "�K�E�Z", "�A�C�e���g�p", "�A�C�e���ύX",
		"����m�F"
	};
	const char* helpInputs[] = 
	{
		"�}�E�X or �E�X�e�B�b�N",
		"WASD�L�[ or ���X�e�B�b�N",
		"Shift or A�{�^��",
		"�X�y�[�X�L�[ or B�{�^��",
		"���N���b�N or RB�{�^��",
		"�E�N���b�N or RT�{�^��",
		"�z�C�[���N���b�N or �E�X�e�B�b�N��������",
		"Q�L�[ or LB�{�^��",
		"R�L�[ or X�{�^��",
		"�}�E�X�z�C�[�� or �\���L�[���E",
		"Tab�L�[ or ���j���[�{�^��"
	};
	const int lineCount = sizeof(helpLabels) / sizeof(helpLabels[0]);

	// ���C�A�E�g
	int contentHeight = HELP_TITLE_TOP_MARGIN + lineCount * lineHeight + HELP_PADDING;

	int boxX1 = (screenWidth - HELP_BOX_WIDTH) / 2;
	int boxY1 = (screenHeight - contentHeight) / 2;
	int boxX2 = boxX1 + HELP_BOX_WIDTH;
	int boxY2 = boxY1 + contentHeight;

	// �w�i
	DrawBox(boxX1, boxY1, boxX2, boxY2, GetColor(COLOR_COLOR_BLACK_R, COLOR_COLOR_BLACK_G, COLOR_COLOR_BLACK_B), TRUE);

	// �^�C�g��
	DrawString(boxX1 + HELP_PADDING, boxY1 + HELP_PADDING, "�y��������z", GetColor(COLOR_WHITE_R, COLOR_WHITE_G, COLOR_WHITE_B));

	// �`��ʒu
	int labelX = boxX1 + HELP_LABEL_LEFT;
	int inputX = boxX1 + HELP_INPUT_LEFT;
	int y = boxY1 + HELP_CONTENT_TOP_OFFSET;

	for (int i = 0; i < lineCount; ++i)
	{
		DrawString(labelX, y + i * lineHeight, helpLabels[i], GetColor(COLOR_WHITE_R, COLOR_WHITE_G, COLOR_WHITE_B));
		DrawString(inputX, y + i * lineHeight, "�F", GetColor(COLOR_WHITE_R, COLOR_WHITE_G, COLOR_WHITE_B));
		DrawString(inputX + HELP_INPUT_OFFSET, y + i * lineHeight, std::string(helpInputs[i]).c_str(), GetColor(COLOR_WHITE_R, COLOR_WHITE_G, COLOR_WHITE_B));
	}
	SetFontSize(DEFAULT_FONT_SIZE);
}