#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "ClearScene.h"

ClearScene::ClearScene(void)
{
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Init(void)
{
	// ��
	clearBgm_ = resMng_.Load(ResourceManager::SRC::CLEAR_BGM).handleId_;
	ChangeVolumeSoundMem(CLEAR_BGM_VOL, clearBgm_);
	enterSe_ = resMng_.Load(ResourceManager::SRC::CLICK_SE).handleId_;
	ChangeVolumeSoundMem(ENTER_SE_VOL, enterSe_);
	isBgm_ = true;

	ChangeFont("Garamond");
	// �摜�ǂݍ���
	imgClear_ = resMng_.Load(ResourceManager::SRC::CLEAR).handleId_;

	// ��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
	SceneManager::GetInstance().GetCamera()->ChangeGameCamera(Camera::GAME_CAMERA::MOUSE);

	for (int i = 0; i < PARTICLE_COUNT; ++i) {
		particles_[i].x = static_cast<float>(rand() % Application::SCREEN_SIZE_X);
		particles_[i].y = static_cast<float>(rand() % Application::SCREEN_SIZE_Y);
		particles_[i].vx = ((rand() % PARTICLE_VX_RAND_MAX) / PARTICLE_VX_DIVISOR - PARTICLE_VX_CENTER) * PARTICLE_VX_RANGE;
		particles_[i].vy = PARTICLE_VY_BASE + (rand() % PARTICLE_VY_RAND_MAX) / PARTICLE_VY_DIVISOR;
		particles_[i].size = PARTICLE_SIZE_MIN + rand() % PARTICLE_SIZE_VARIATION;
		particles_[i].alpha = PARTICLE_ALPHA_MIN + rand() % PARTICLE_ALPHA_VARIATION;
	}
}

void ClearScene::Update(void)
{
	if (isBgm_)
	{
		PlaySoundMem(clearBgm_, DX_PLAYTYPE_BACK, true);
		isBgm_ = false;
	}

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();

	phase_ += PHASE_INCREMENT;

	if (ins.IsTrgDown(KEY_INPUT_RETURN) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		PlaySoundMem(enterSe_, DX_PLAYTYPE_BACK, true);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void ClearScene::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, CLEAR_IMAGE_SCALE, 0.0, imgClear_, true);

	// �q���g�\���iEnter�Ŗ߂�j
	SetFontSize(HINT_FONT_SIZE);
	const char* hint = "Press Enter or A Button";
	int hintWidth = GetDrawStringWidth(hint, static_cast<int>(strlen(hint)));
	int hintX = (Application::SCREEN_SIZE_X - hintWidth) / 2;

	// �������̍��w�i
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, HINT_BG_ALPHA);
	DrawBox(hintX - HINT_BG_PADDING_X, HINT_Y - HINT_BG_PADDING_Y, hintX + hintWidth + HINT_BG_PADDING_X, HINT_Y + HINT_BG_HEIGHT, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �t�`�t�������Ŗڗ�������
	DrawString(hintX + HINT_SHADOW_OFFSET, HINT_Y + HINT_SHADOW_OFFSET, hint, GetColor(0, 0, 0));                // �e
	DrawString(hintX, HINT_Y, hint, GetColor(COLOR_HINT_TEXT_R, COLOR_HINT_TEXT_G, COLOR_HINT_TEXT_B));                 // �{��

	// �p�[�e�B�N��
	DrawParticles();

	SetFontSize(CLEAR_FONT_SIZE_SMALL);
}

void ClearScene::DrawUI(void)
{
}

void ClearScene::UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)
{
}

void ClearScene::DrawParticles(void)
{
	for (int i = 0; i < PARTICLE_COUNT; ++i) {
		particles_[i].x += particles_[i].vx;
		particles_[i].y += particles_[i].vy;

		if (particles_[i].y > Application::SCREEN_SIZE_Y) particles_[i].y = 0;
		if (particles_[i].x < 0) particles_[i].x = Application::SCREEN_SIZE_X;
		if (particles_[i].x > Application::SCREEN_SIZE_X) particles_[i].x = 0;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, particles_[i].alpha);
		DrawCircle((int)particles_[i].x, (int)particles_[i].y, particles_[i].size, GetColor(COLOR_PARTICLE_R, COLOR_PARTICLE_G, COLOR_PARTICLE_B), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
