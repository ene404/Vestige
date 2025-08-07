#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../Player/Player.h"
#include "EnemyScarecrow.h"

EnemyScarecrow::EnemyScarecrow(Player& player) 
    :
    player_(player),
    hp_(0),
    delayedHpRate_(0.0f),
    isAlive_(false)
{
}

EnemyScarecrow::~EnemyScarecrow(void)
{
}

void EnemyScarecrow::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SCARECROW));
	transform_.scl = { SIZE, SIZE, SIZE };
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
    transform_.quaRotLocal =
        Quaternion::Euler(INIT_LOCAL_ROT);
	transform_.Update();

	hp_ = MAX_HP;
    delayedHpRate_ = HP_RATE;
	isAlive_ = true;
}

void EnemyScarecrow::Update(void)
{
    if (hp_ <= 0)
    {
        isAlive_ = false;
        hp_ = 0;  // 0�����ɂȂ�Ȃ��悤�ɔO�̂���
    }

    // ���C���Q�[�W�̊���
    float hpRate = static_cast<float>(hp_) / MAX_HP;

    // �Ǐ]�Q�[�W�̊��������������C���Q�[�W�ɋ߂Â���
    const float followSpeed = FOLLOW_SPEED;  // ���鑬�x�B���l�𒲐����čD�݂̃X�s�[�h��
    if (delayedHpRate_ > hpRate)
    {
        delayedHpRate_ -= followSpeed;
        if (delayedHpRate_ < hpRate)
        {
            delayedHpRate_ = hpRate;
        }
    }
    else
    {
        // HP�񕜎��͒Ǐ]�Q�[�W�������ɖ߂��i�K�v�ɉ����āj
        delayedHpRate_ = hpRate;
    }

    transform_.Update();
}

void EnemyScarecrow::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void EnemyScarecrow::DrawHpBar(void)
{
    int width = WIDTH;
    int height = HEIGHT;  // �{�X�Ɠ���������

    int x = (Application::SCREEN_SIZE_X - width) / 2 + X_ADJUSTMENT;
    int y = Application::SCREEN_SIZE_Y - height - Y_ADJUSTMENT;

    float hpRate = static_cast<float>(hp_) / MAX_HP;
    int filledWidth = static_cast<int>(width * hpRate);

    int delayedWidth = static_cast<int>(width * delayedHpRate_);

    // �G�̖��O�\������
    const char* enemyName = "�P���p�̂�����";
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

    // �g�F�ݒ�
    int borderDark = GetColor(BORDER_DARK_COL, BORDER_DARK_COL, BORDER_DARK_COL);
    int borderLight = GetColor(BORDER_LIGHT_COL, BORDER_LIGHT_COL, BORDER_LIGHT_COL);
    int bgColor = GetColor(A_QUARTER_COL, A_QUARTER_COL, A_QUARTER_COL);

    // �_���[�W�F�i�Ǐ]�Q�[�W�j
    int delayedColorLight = GetColor(DELAYED_COL_LIGHT_R, DELAYED_COL_LIGHT, DELAYED_COL_LIGHT);
    int delayedColorTop = GetColor(DELAYED_COL_TOP_R, DELAYED_COL_TOP_GB, DELAYED_COL_TOP_GB);
    int delayedColorBottom = GetColor(DELAYED_COL_BOTTOM_R, DELAYED_COL_BOTTOM_GB, DELAYED_COL_BOTTOM_GB);

    // HP�o�[�F�i�㉺�O���f�j
    int hpColorTop = GetColor(HP_COL_TOP_R, HP_COL_TOP_GB, HP_COL_TOP_GB);
    int hpColorBottom = GetColor(HP_COL_BOTTOM_R, HP_COL_BOTTOM_GB, HP_COL_BOTTOM_GB);

    // �g2�d�`��
    DrawBox(x - BORDER_DARK_ADJUSTMENT, y - BORDER_DARK_ADJUSTMENT, x + width + BORDER_DARK_ADJUSTMENT, y + height + BORDER_DARK_ADJUSTMENT, borderDark, TRUE);
    DrawBox(x - BORDER_LIGHT_ADJUSTMENT, y - BORDER_LIGHT_ADJUSTMENT, x + width + BORDER_LIGHT_ADJUSTMENT, y + height + BORDER_LIGHT_ADJUSTMENT, borderLight, TRUE);

    // �w�i�o�[
    DrawBox(x, y, x + width, y + height, bgColor, TRUE);

    // �Ǐ]�Q�[�W��HP�o�[�̉E���ɔ������O���f�ŕ`��
    if (delayedWidth > filledWidth)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, DAMAGE_BLEND_PARAM);

        int halfH = height / 2;
        // ���x�[�X���ۂ�����ߔw�i
        DrawBox(x + filledWidth, y, x + delayedWidth, y + height, delayedColorLight, TRUE);
        DrawBox(x + filledWidth, y, x + delayedWidth, y + halfH, delayedColorTop, TRUE);
        DrawBox(x + filledWidth, y + halfH, x + delayedWidth, y + height, delayedColorBottom, TRUE);

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // HP�o�[�i�㉺�O���f�j
    if (filledWidth > 0)
    {
        int halfH = height / 2;
        DrawBox(x, y, x + filledWidth, y + halfH, hpColorTop, TRUE);
        DrawBox(x, y + halfH, x + filledWidth, y + height, hpColorBottom, TRUE);
    }

    // �g���i���j
    DrawBox(x, y, x + width, y + height, GetColor(MAX_COL, MAX_COL, MAX_COL), FALSE);

    SetFontSize(16);
}

void EnemyScarecrow::Damage(int damage)
{
	hp_ -= damage;
}

bool EnemyScarecrow::IsAlive(void)
{
	return isAlive_;
}
