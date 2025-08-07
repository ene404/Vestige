#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "Book.h"

Book::Book(void)
{
}

Book::~Book(void)
{
}

void Book::Init(void)
{
    bookSe_ = resMng_.Load(ResourceManager::SRC::BOOK_SE).handleId_;
    ChangeVolumeSoundMem(220, bookSe_);

    lightningSe_ = resMng_.Load(ResourceManager::SRC::LIGHTNING_SE).handleId_;
    ChangeVolumeSoundMem(220, lightningSe_);

    itemImg_ = resMng_.Load(ResourceManager::SRC::BOOK).handleId_;
    count_ = 1;

    // �G�t�F�N�g
    effectLightningResId_ = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::LIGHTNING_EFFECT).handleId_;

    // �傫��
    float SCALE = 20.0f;
    SetScalePlayingEffekseer3DEffect(effectLightningPlayId_, SCALE, SCALE, SCALE);
}

void Book::Update(void)
{

}

void Book::Draw(void)
{
    // �w�i
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(UI_LEFT + 2, UI_TOP + 2, UI_RIGHT - 2, UI_BOTTOM - 2, 0x000000, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �g��
    for (int i = 0; i < 3; ++i) {
        DrawBox(UI_LEFT - i, UI_TOP - i, UI_RIGHT + i, UI_BOTTOM + i, 0xffffff, false);
    }
    DrawBox(UI_LEFT + 3, UI_TOP + 3, UI_RIGHT - 3, UI_BOTTOM - 3, 0xaaaaaa, false);

    // ������0�Ȃ�Â��\��
    if (count_ == 0) {
        SetDrawBright(100, 100, 100); // �Â߂̐F������
    }

    // �A�C�e���摜
    DrawRotaGraph(100, 515, 0.08, 0.0, itemImg_, true);

    // ���邳�����ɖ߂�
    SetDrawBright(255, 255, 255);

    // �u���@���v�̃��x���Ɣw�i
    SetFontSize(18);
    const char* label = "���@��";
    int textWidth = GetDrawStringWidth(label, static_cast<int>(strlen(label)));
    int textX = 100 - textWidth / 2;
    int textY = 590;
    int paddingX = 10;
    int paddingY = 4;

    // �w�i�сi���������j
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
    DrawBox(
        textX - paddingX,
        textY - paddingY,
        textX + textWidth + paddingX,
        textY + 16 + paddingY,
        0x000000, true
    );
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // ���x������
    DrawString(textX, textY, label, 0xffffff);

    // �������̕\���i�ʏ�\���j
    DrawFormatString(131, 561, 0x000000, "%d", count_);
    DrawFormatString(130, 560, 0xffffff, "%d", count_);
}

void Book::Use(void)
{
    PlaySoundMem(bookSe_, DX_PLAYTYPE_BACK, true);
    count_--;
}

int Book::GetCount(void)
{
    return count_;
}

void Book::LightningUpdate(VECTOR pos)
{
    // �G�t�F�N�g
    effectLightningPlayId_ = PlayEffekseer3DEffect(effectLightningResId_);
    // �傫��
    float SCALE = 30.0f;
    SetScalePlayingEffekseer3DEffect(effectLightningPlayId_, SCALE, SCALE, SCALE);
    SetPosPlayingEffekseer3DEffect(effectLightningPlayId_, pos.x, pos.y - 200.0f, pos.z);
    PlaySoundMem(lightningSe_, DX_PLAYTYPE_BACK, true);
}
