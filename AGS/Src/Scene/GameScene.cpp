#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Stage/Mist.h"
#include "../Object/Actor/Player/Player.h"
#include "../Object/Actor/Player/PlayerDummy.h"
#include "../Object/Stage/Planet.h"
#include "../Object/Actor/Enemy/EnemyBoss/EnemyBoss.h"
#include "../Object/Actor/Enemy/EnemyBoss/EnemyBossDummy.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	player_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
}

GameScene::~GameScene(void)
{
	if (helpBackImage_ != -1)
	{
		DeleteGraph(helpBackImage_);
		helpBackImage_ = -1;
	}
}

void GameScene::Init(void)
{
	// ��
	gameBgm_ = resMng_.Load(ResourceManager::SRC::GAME_BGM).handleId_;
	bgmVol_ = 160;
	ChangeVolumeSoundMem(bgmVol_, gameBgm_);
	isBgm_ = true;

	deadSe_ = resMng_.Load(ResourceManager::SRC::DEAD_SE).handleId_;
	ChangeVolumeSoundMem(255, deadSe_);

	helpSe_ = resMng_.Load(ResourceManager::SRC::HELP_SE).handleId_;
	ChangeVolumeSoundMem(220, helpSe_);

	ChangeFont("MS �S�V�b�N");

	// �v���C���[
	player_ = std::make_shared<Player>();
	player_->Init();

	playerDummy_ = std::make_shared<PlayerDummy>(*player_);

	// boss
	enemyBoss_ = std::make_shared<EnemyBoss>(*player_);
	enemyBoss_->Init();

	// boss�_�~�[
	enemyBossDummy_ = std::make_shared<EnemyBossDummy>(*enemyBoss_);

	mist_ = std::make_shared<Mist>();
	mist_->Init();

	// �X�e�[�W
	stage_ = std::make_unique<Stage>(*player_, *enemyBoss_);
	stage_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();


	SceneManager::GetInstance().GetCamera()->LockonSoundLoad();
	SceneManager::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	SceneManager::GetInstance().GetCamera()->SetFollowEnemy(&enemyBoss_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW_MOUSE);
	player_->SetEnemy(enemyBoss_);
	player_->SetEnemyDummy(enemyBossDummy_);
	player_->SetMist(mist_);
	enemyBoss_->SetMist(mist_);

	deathFadeAlpha_ = 0;
	youDiedTimer_ = 0;

	enemySlainFadeAlpha_ = 0;
	enemySlainTimer_ = 0;
	enemySlain_ = false;

	isHelp_ = false;
	helpBackImage_ = -1;
	wasHelp_ = false;

	// �|�X�g�G�t�F�N�g�p�X�N���[��
	postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// �|�X�g�G�t�F�N�g�p(�v���C���[�̎��E)
	lowHpVisionMaterial_ = std::make_unique<PixelMaterial>("LowHpVision.cso", 2);

	lowHpVisionMaterial_->AddConstBuf({ 1.0f, 1.0f, 1.0f, 1.0f });

	float hpRate = static_cast<float>(player_->GetHp()) / static_cast<float>(Player::MAX_HP);
	lowHpVisionMaterial_->AddConstBuf({ hpRate, 0.0f, 0.0f, 0.0f });

	lowHpVisionMaterial_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	lowHpVisionRenderer_ = std::make_unique<PixelRenderer>(*lowHpVisionMaterial_);
	lowHpVisionRenderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y)
	);

	// �|�X�g�G�t�F�N�g�p(���C�g�j���O)
	lightningMaterial_ = std::make_unique<PixelMaterial>("Lightning.cso", 1);

	lightningTime_ = GetNowCount() / 1000.0f;
	lightningPower_ = 0.0f;

	lightningMaterial_->AddConstBuf({ lightningTime_, lightningPower_, 1.0f, 1.0f });

	lightningMaterial_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	lightningRenderer_ = std::make_unique<PixelRenderer>(*lightningMaterial_);
	lightningRenderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y)
	);
}

void GameScene::Update(void)
{
	if (isBgm_)
	{
		PlaySoundMem(gameBgm_, DX_PLAYTYPE_LOOP, true);
		isBgm_ = false;
	}


	InputManager& ins = InputManager::GetInstance();

	if (player_->GetHp() > 0 && enemyBoss_->GetHp() > 0)
	{
		SceneManager::GetInstance().HelpUpdate();
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
		}
	}

	if (!isHelp_)
	{
		// �X�J�C�h�[����I�u�W�F�N�g�̍X�V
		skyDome_->Update();
		stage_->Update();
		player_->Update();
		if (enemyBoss_->GetHp() > 0)
		{
			player_->CollisionEnemyBoss();
		}
		player_->CollisionMist();
		playerDummy_->Update();
		enemyBoss_->Update();
		enemyBoss_->CollisionMist();
		enemyBossDummy_->Update();

		lightningTime_ = GetNowCount() / 1000.0f;
		if (player_->IsLightning())
		{
			lightningPower_ = 1.0f;
		}
		if (lightningPower_ > 0.0f)
		{
			lightningPower_ -= 0.05f; // 1�b��0�ɂȂ�i= 20fps ��0.05f���j
			if (lightningPower_ < 0.0f) lightningPower_ = 0.0f;
		}

		// �G���j���̉��o�J�n�iScene�J�ڂ�x�点��j
		if (!enemySlain_ && enemyBoss_->GetHp() <= 0)
		{
			enemySlain_ = true;
			enemySlainFadeAlpha_ = 0;
			enemySlainTimer_ = 0;
		}

		if (enemySlain_)
		{
			enemySlainTimer_++;

			// �t�F�[�h�C���i0�`60�t���[���j
			if (enemySlainTimer_ <= 60)
			{
				enemySlainFadeAlpha_ = enemySlainTimer_ * (255 / 60);
			}
			// �\���L�[�v�i61�`180�t���[���j
			else if (enemySlainTimer_ <= 180)
			{
				enemySlainFadeAlpha_ = 255;
			}
			// �t�F�[�h�A�E�g�i181�`280�t���[���j
			else if (enemySlainTimer_ <= 280)
			{
				enemySlainFadeAlpha_ = 255 - (enemySlainTimer_ - 180) * (255 / 60);
			}

			// �I����ɃN���A�V�[���֑J��
			if (enemySlainTimer_ > 280)
			{
				player_->StopEffect();
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
			}
		}

		// YOU DIED �̏���
		if (player_->IsDead())
		{
			if (deathFadeAlpha_ < 190)
			{
				deathFadeAlpha_ += 2;
				if (deathFadeAlpha_ > 190) deathFadeAlpha_ = 190;
			}
			youDiedTimer_++;

			if (youDiedTimer_ == 1)
			{
				PlaySoundMem(deadSe_, DX_PLAYTYPE_BACK, true);
			}

			// ���g���C�L�[���͂ŃV�[���ď�����
			if (youDiedTimer_ > 90 &&
				(ins.IsTrgDown(KEY_INPUT_E) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT)))
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
				return;
			}

			if (youDiedTimer_ > 660)
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::OVER);
			}

		}
		if (player_->GetHp() <= 0)
		{
			if (bgmVol_ > 120)
			{
				bgmVol_--;
			}
			ChangeVolumeSoundMem(bgmVol_, gameBgm_);
		}
	}
}

void GameScene::Draw(void)
{
	if (isHelp_ && helpBackImage_ != -1)
	{
		return; // �ʏ�`��̓X�L�b�v
	}

	// �w�i
	skyDome_->Draw();
	stage_->Draw();
	mist_->Draw();

	enemyBoss_->Draw();

	player_->Draw();

	SceneManager::GetInstance().GetCamera()->DrawLockonMarker();

	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	// �|�X�g�G�t�F�N�g(�v���C���[�̎��E)
	//-----------------------------------------
	float hpRate = static_cast<float>(player_->GetHp()) / static_cast<float>(Player::MAX_HP);
	lowHpVisionMaterial_->SetConstBuf(1, { hpRate, 0.0f, 0.0f, 0.0f });

	SetDrawScreen(postEffectScreen_);

	// ��ʂ�������
	ClearDrawScreen();

	lowHpVisionRenderer_->Draw();

	// ���C���ɖ߂�
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);
	//-----------------------------------------

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

void GameScene::DrawUI(void)
{
	player_->DrawHp(static_cast<float>(player_->GetHp()), Player::MAX_HP);
	player_->DrawStamina(static_cast<float>(player_->GetStamina()), Player::MAX_STAMINA);
	player_->DrawItem();
	player_->DrawUlt();


	enemyBoss_->DrawHpBar();

	// �uENEMY SLAIN�v���o
	if (enemySlain_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, enemySlainFadeAlpha_);

		int screenW = Application::SCREEN_SIZE_X;
		int screenH = Application::SCREEN_SIZE_Y;
		const char* msg = "ENEMY SLAIN";

		int fontSize = 80;
		SetFontSize(fontSize);

		int msgW = GetDrawStringWidth(msg, static_cast<int>(strlen(msg)));
		int msgH = fontSize;
		int x = (screenW - msgW) / 2;
		int y = (screenH - msgH) / 2;

		// �w�i�̍��{�b�N�X
		int padding = 10;
		DrawBox(0, y - padding, screenW, y + msgH + padding, GetColor(0, 0, 0), TRUE);

		// �e�t�������i���F�j
		int shadowOffset = 5;
		DrawString(x + shadowOffset, y + shadowOffset, msg, GetColor(0, 0, 0)); // �e
		DrawString(x, y, msg, GetColor(255, 215, 0)); // �{�́i���j

		SetFontSize(16);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// �uYOU DIED�v���o
	if (player_->IsDead())
	{
		// ��ʂ̍��I�[�o�[���C�i�Ó]�j
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, deathFadeAlpha_);
		DrawBox(0, 0, 1024, 640, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �uYOU DIED�v�̕������t�F�[�h�C���\���i�e�t���j
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, deathFadeAlpha_);

		int screenW = Application::SCREEN_SIZE_X;
		int screenH = Application::SCREEN_SIZE_Y;
		const char* msg = "YOU DIED";

		int fontSize = 80;
		SetFontSize(fontSize);

		int msgW = GetDrawStringWidth(msg, static_cast<int>(strlen(msg)));
		int msgH = fontSize;

		// �����ʒu
		int x = (screenW - msgW) / 2;
		int y = (screenH - msgH) / 2;

		// === �w��̍��{�b�N�X�i�I�v�V�����j ===
		int padding = 10;
		DrawBox(
			0,
			y - padding,
			screenW,
			y + msgH + padding,
			GetColor(0, 0, 0),
			TRUE
		);

		// === �e�t������ ===
		int shadowOffset = 5; // �e�̃Y���ʁi�s�N�Z���j
		// �e�i�������j���������E�E�ɃY�����ĕ`��
		DrawString(x + shadowOffset, y + shadowOffset, msg, GetColor(0, 0, 0));

		// �{�́i�ԕ����j����ɏd�˂�
		DrawString(x, y, msg, GetColor(255, 0, 0));
		SetFontSize(16);

		// Retry���b�Z�[�W�\���i��莞�Ԍo�ߌ�j
		if (youDiedTimer_ > 90)
		{
			const char* retryMsg = "E�L�[ or X�{�^�� �Ń��g���C";

			int retryFontSize = 32;
			SetFontSize(retryFontSize);

			int retryW = GetDrawStringWidth(retryMsg, static_cast<int>(strlen(retryMsg)));
			int retryH = retryFontSize;

			int rx = (screenW - retryW) / 2;
			int ry = y + msgH + 80;

			// �_�Ł{������Ԃ֕ω�������A���t�@�v�Z
			float time = youDiedTimer_ / 60.0f;  // �b��
			int blinkAlpha = (int)((sin(time * 3.14f) * 0.5f + 0.5f) * 255);
			if (blinkAlpha > 255) blinkAlpha = 255;
			if (blinkAlpha < 0) blinkAlpha = 0;

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, blinkAlpha);

			// �e�i���j
			DrawString(rx + 2, ry + 2, retryMsg, GetColor(0, 0, 0));
			// �{�́i�����ԂփO���f�[�V�����j
			DrawString(rx, ry, retryMsg, GetColor(255, 255, 255));

			// �u�����J�ڂ܂Ŏc��X�b�v�̃��b�Z�[�W��`��
			int autoMsgFontSize = 20;
			SetFontSize(autoMsgFontSize);

			int autoTransitionTimeSec = 10;

			// �؂�グ�ł�������u10�b��v����J�E���g�_�E������
			int remaining = (660 - youDiedTimer_ + 59) / 60;
			if (remaining < 0)
			{
				remaining = 0;
			}

			std::string autoMsg = std::to_string(remaining) + " �b����߂܂�";

			int autoMsgW = GetDrawStringWidth(autoMsg.c_str(), (int)autoMsg.size());
			int ax = (screenW - autoMsgW) / 2;
			int ay = ry + retryH + 10;

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
			DrawString(ax, ay, autoMsg.c_str(), GetColor(255, 255, 255));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			SetFontSize(16);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// �w���v
	if (isHelp_ && helpBackImage_ != -1)
	{
		// �w�i�̃L���v�`����`��
		DrawGraph(0, 0, helpBackImage_, false);
		DrawHelp();
	}
}

void GameScene::UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)
{
}

void GameScene::DrawHelp(void)
{
	const int screenWidth = 1024;
	const int screenHeight = 640;

	// === �e�L�X�g�ݒ� ===
	const int fontSize = 18;
	const int lineHeight = fontSize + 14;
	SetFontSize(18);

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
	int contentWidth = 660;
	int contentHeight = 40 + lineCount * lineHeight + 20;

	int boxX1 = (screenWidth - contentWidth) / 2;
	int boxY1 = (screenHeight - contentHeight) / 2;
	int boxX2 = boxX1 + contentWidth;
	int boxY2 = boxY1 + contentHeight;

	// �w�i
	DrawBox(boxX1, boxY1, boxX2, boxY2, GetColor(0, 0, 0), TRUE);

	// �^�C�g��
	DrawString(boxX1 + 20, boxY1 + 20, "�y��������z", GetColor(255, 255, 255));

	// �`��ʒu
	int labelX = boxX1 + 40;
	int inputX = boxX1 + 170;
	int y = boxY1 + 60;

	for (int i = 0; i < lineCount; ++i)
	{
		DrawString(labelX, y + i * lineHeight, helpLabels[i], GetColor(255, 255, 255));
		DrawString(inputX, y + i * lineHeight, "�F", GetColor(255, 255, 255));
		DrawString(inputX + 60, y + i * lineHeight, std::string(helpInputs[i]).c_str(), GetColor(255, 255, 255));
	}
	SetFontSize(16);
}