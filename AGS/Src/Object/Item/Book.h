#pragma once
#include "ItemBase.h"

class Book : public ItemBase
{
public:
	static constexpr int UI_LEFT = 55;
	static constexpr int UI_TOP = 450;
	static constexpr int UI_RIGHT = 145;
	static constexpr int UI_BOTTOM = 580;

	// コンストラクタ
	Book(void);

	// デストラクタ
	~Book(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Use(void);
	int GetCount(void);

	void LightningUpdate(VECTOR pos);
private:
	int bookSe_;
	int lightningSe_;

	// エフェクト
	int effectLightningPlayId_;
	int effectLightningResId_;
};

