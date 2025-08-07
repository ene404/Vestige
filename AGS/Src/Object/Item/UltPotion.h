#pragma once
#include "ItemBase.h"
class UltPotion :
    public ItemBase
{
public:
	static constexpr int UI_LEFT = 55;
	static constexpr int UI_TOP = 450;
	static constexpr int UI_RIGHT = 145;
	static constexpr int UI_BOTTOM = 580;
	
	// コンストラクタ
	UltPotion(void);

	// デストラクタ
	~UltPotion(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Use(void);
	int GetCount(void);
private:
	int ultPotionSe_;
};

