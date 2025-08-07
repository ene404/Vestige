#pragma once
#include "../../../Utility/AsoUtility.h"
#include "../ActorBase.h"

class Player;

class EnemyScarecrow :
    public ActorBase
{

public:

    // サイズ
    static constexpr float SIZE = 4.0f;

	// 初期座標
	static constexpr VECTOR INIT_POS = { 0.0f, 70.0f, 660.0f };

	// 初期ローカル回転
	const VECTOR INIT_LOCAL_ROT = { 0.0f,AsoUtility::Deg2RadF(90.0f),0.0f };

	// HP
    static constexpr int MAX_HP = 1500;

	// HP比率
	static constexpr float HP_RATE = 1.0f;

	// 減速ゲージのスピード
	static constexpr float FOLLOW_SPEED = 0.005f;

	// HPバー関連
	static constexpr int WIDTH = 650;
	static constexpr int HEIGHT = 20;
	static constexpr int X_ADJUSTMENT = 60;
	static constexpr int Y_ADJUSTMENT = 80;
	static constexpr int FONT_SIZE = 20;
	static constexpr int NAME_HEIGHT = 24;
	static constexpr int PADDING_LEFT = 8;
	static constexpr int PADDING = 8;
	static constexpr int NAME_BLEND_PARAM = 160;
	static constexpr int MAX_COL = 255;
	static constexpr int TEXCOL_R = 255;
	static constexpr int TEXCOL_G = 255;
	static constexpr int TEXCOL_B = 255;
	static constexpr int A_QUARTER_COL = 60;
	static constexpr int DELAYED_COL_LIGHT_R = 255;
	static constexpr int DELAYED_COL_LIGHT = 200;
	static constexpr int DELAYED_COL_TOP_R = 255;
	static constexpr int DELAYED_COL_TOP_GB = 100;
	static constexpr int DELAYED_COL_BOTTOM_R = 200;
	static constexpr int DELAYED_COL_BOTTOM_GB = 50;
	static constexpr int HP_COL_TOP_R = 255;
	static constexpr int HP_COL_TOP_GB = 80;
	static constexpr int HP_COL_BOTTOM_R = 180;
	static constexpr int HP_COL_BOTTOM_GB = 40;
	static constexpr int BORDER_DARK_COL = 40;
	static constexpr int BORDER_LIGHT_COL = 200;
	static constexpr int DAMAGE_BLEND_PARAM = 180;
	static constexpr int BORDER_DARK_ADJUSTMENT = 2;
	static constexpr int BORDER_LIGHT_ADJUSTMENT = 1;

	EnemyScarecrow(Player& player);
	~EnemyScarecrow(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	// HPバーの描画
	void DrawHpBar(void);

	void Damage(int damage);
	bool IsAlive(void);
private:
	// プレイヤー
	Player& player_;

	int hp_;
	float delayedHpRate_;

	bool isAlive_;
};

