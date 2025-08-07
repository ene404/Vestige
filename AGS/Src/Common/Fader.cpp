#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "Fader.h"

Fader::STATE Fader::GetState(void) const
{
	return state_;
}

bool Fader::IsEnd(void) const
{
	return isEnd_;
}

void Fader::SetFade(STATE state)
{
	state_ = state;
	if (state_ != STATE::NONE)
	{
		isPreEnd_ = false;
		isEnd_ = false;
		loadImg_ = resMng_.Load(ResourceManager::SRC::NOW_LOADING).handleId_;
	}
}

Fader::Fader(void) : resMng_(ResourceManager::GetInstance())
{
}

Fader::~Fader(void)
{
}

void Fader::Init(void)
{
	state_ = STATE::NONE;
	alpha_ = 0;
	isPreEnd_ = true;
	isEnd_ = true;
	loadingBarFrame_ = 0;
	loadImg_ = resMng_.Load(ResourceManager::SRC::NOW_LOADING).handleId_;
}

void Fader::Update(void)
{
	if (isEnd_)
	{
		return;
	}

	loadingBarFrame_++;

	// ���[�f�B���O�o�[��100���𒴂���������
	const int maxProgress = 100;
	bool isLoadingComplete = (loadingBarFrame_ >= maxProgress);



	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > 255)
		{
			alpha_ = 255;
			if (isPreEnd_)
			{
				// �t�F�[�h�A�E�g�I��
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	case STATE::FADE_IN:
		// ���[�f�B���O�o�[���I����Ă���t�F�[�h�C���J�n
		if (!isLoadingComplete)
		{
			return; // �o�[��100���ɂȂ�܂ł̓t�F�[�h�C����i�߂Ȃ�
		}

		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			alpha_ = 0;
			if (isPreEnd_)
			{
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	default:
		return;
	}
}

void Fader::Draw(void)
{

	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);

		// �w�i�̍��h��
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			0x000000, true);

		// ���[�f�B���O�o�[�`��
		if (!isEnd_)
		{
			const int barWidth = 400;
			const int barHeight = 20;
			const int cornerRadiusX = barHeight / 2;
			const int cornerRadiusY = barHeight / 2;
			const int maxProgress = 100;

			int x = (Application::SCREEN_SIZE_X - barWidth) / 2;
			int y = Application::SCREEN_SIZE_Y / 2;

			int progress = (loadingBarFrame_ < maxProgress) ? loadingBarFrame_ : maxProgress;
			float progressRatio = (float)progress / maxProgress;
			int filledWidth = static_cast<int>(barWidth * progressRatio);
			int filledRight = x + filledWidth;

			// �uNow Loading...�v�\���i������Ɂj
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 - 40, 1.0, 0.0, loadImg_, true);

			// �O���[�̔w�i�o�[�i�p�ہj
			DrawRoundRect(x, y, x + barWidth, y + barHeight, cornerRadiusX, cornerRadiusY, GetColor(150, 150, 150), TRUE);


			// �Ԃ��i�s�o�[�i�p�� or �ʏ��`�j
			if (filledWidth >= cornerRadiusX * 2)
			{
				// �ۂ݂������ԃo�[
				DrawRoundRect(x, y, filledRight, y + barHeight, cornerRadiusX, cornerRadiusY, GetColor(255, 0, 0), TRUE);

				// �p�ۂ̉e���������ɂ����`��
				int shadowTop = y + barHeight / 2;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100); // ������

				// �e�̋�`�i�ۂ݂͌��Ɠ����ŉ����������h��Ԃ��j
				DrawRoundRect(x, shadowTop, filledRight, y + barHeight, cornerRadiusX, cornerRadiusY, GetColor(0, 0, 0), TRUE);

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
			}
			else if (filledWidth > 0)
			{
				// �������� �� �ʏ��`�ŕ`��i�p�ۂ��ƕ����j
				DrawBox(x, y, filledRight, y + barHeight, GetColor(255, 0, 0), TRUE);

				// �e�i�������ɔ��������j
				int shadowY = y + barHeight / 2;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
				DrawBox(x, shadowY, filledRight, y + barHeight, GetColor(0, 0, 0), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
			}

			// �����g�i�p�ہj���h��Ԃ��Ȃ�
			DrawRoundRect(x, y, x + barWidth, y + barHeight, cornerRadiusX, cornerRadiusY, GetColor(255, 255, 255), FALSE);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}

void Fader::SetLoadingBarFrame(int loadingBarFrame)
{
	loadingBarFrame_ = loadingBarFrame;
}
