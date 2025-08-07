#pragma once
#include "ItemBase.h"

class HpPotion : public ItemBase
{
public:
	static constexpr int UI_LEFT = 55;
	static constexpr int UI_TOP = 450;
	static constexpr int UI_RIGHT = 145;
	static constexpr int UI_BOTTOM = 580;

	// コンストラクタ
	HpPotion(void);

	// デストラクタ
	~HpPotion(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Use(void);
	int GetCount(void);
private:
	int hpPotionSe_;
};

