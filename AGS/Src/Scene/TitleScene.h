#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;

class TitleScene : public SceneBase
{

public:

	struct Particle {
		float x, y;       // 位置
		float vx, vy;     // 移動速度（左右ゆらぎ＆下方向）
		int size;         // 粒の大きさ（1～3）
		int alpha;        // 透明度（100～200くらい）
	};

	// 音
	static constexpr int TITLE_BGM_VOL = 180;
	static constexpr int CLICK_SE_VOL = 255;

	static constexpr int PARTICLE_COUNT = 70;
	static constexpr float PARTICLE_VX_CENTER = 0.5f;
	static constexpr float PARTICLE_VX_RANGE = 0.2f;
	static constexpr float PARTICLE_VY_BASE = 0.2f;
	static constexpr int PARTICLE_VX_RAND_MAX = 100;
	static constexpr float PARTICLE_VX_DIVISOR = 100.0f;
	static constexpr int PARTICLE_VY_RAND_MAX = 10;
	static constexpr float PARTICLE_VY_DIVISOR = 50.0f;
	static constexpr int PARTICLE_SIZE_MIN = 1;
	static constexpr int PARTICLE_SIZE_VARIATION = 3;
	static constexpr int PARTICLE_ALPHA_MIN = 100;
	static constexpr int PARTICLE_ALPHA_VARIATION = 100;

	static constexpr int INIT_MENU_HIGHLIGHT_Y = 460;

	static constexpr float STICK_THRESHOLD = 0.5f;
	static constexpr int POV_UP = 0;
	static constexpr int POV_RIGHT = 9000;
	static constexpr int POV_DOWN = 18000;
	static constexpr int POV_LEFT = 27000;
	static constexpr float STICK_SCALE = 1000.0f;
	static constexpr int CONFIRM_OPTION_COUNT = 2;
	static constexpr int MENU_ITEM_COUNT = 2;
	static constexpr int OPTION_PADDING = 40;
	static constexpr int OPTION_Y_POS = 440;
	static constexpr int OPTION_HEIGHT = 34;
	static constexpr int FONT_SIZE_DEFAULT = 16;
	static constexpr int FONT_SIZE_CONFIRM = 28;
	static constexpr int FONT_SIZE_MENU = 36;
	static constexpr int MENU_TEXT_HEIGHT = 36;
	static constexpr int MENU_BASE_Y = 460;
	static constexpr int MENU_ITEM_INTERVAL = 60;
	static constexpr int MENU_PADDING_X = 20;
	static constexpr int MENU_PADDING_Y = 10;
	static constexpr float CONFIRM_LERP_SPEED = 0.2f;
	static constexpr float MENU_LERP_SPEED = 0.15f;

	static constexpr int TITLE_IMAGE_POS_Y = 250;
	static constexpr int CONFIRM_QUESTION_Y = 380;
	static constexpr int HIGHLIGHT_BOX_MARGIN_X = 10;
	static constexpr int HIGHLIGHT_BOX_MARGIN_Y = 6;
	static constexpr int SHADOW_OFFSET = 2;
	static constexpr float GLOW_OFFSET_Y = 2.0f;

	static constexpr int ALPHA_MENU_HIGHLIGHT = 120;
	static constexpr int ALPHA_MENU_NORMAL = 100;
	static constexpr int COLOR_MENU_HIGHLIGHT_R = 255;
	static constexpr int COLOR_MENU_HIGHLIGHT_G = 230;
	static constexpr int COLOR_MENU_HIGHLIGHT_B = 100;
	static constexpr int COLOR_MENU_NORMAL_R = 160;
	static constexpr int COLOR_MENU_NORMAL_G = 160;
	static constexpr int COLOR_MENU_NORMAL_B = 160;
	static constexpr int COLOR_HIGHLIGHT_BG_R = 255;
	static constexpr int COLOR_HIGHLIGHT_BG_G = 255;
	static constexpr int COLOR_HIGHLIGHT_BG_B = 180;
	static constexpr int COLOR_TEXT_SHADOW_R = 0;
	static constexpr int COLOR_TEXT_SHADOW_G = 0;
	static constexpr int COLOR_TEXT_SHADOW_B = 0;
	static constexpr int COLOR_TEXT_MAIN_R = 255;
	static constexpr int COLOR_TEXT_MAIN_G = 255;
	static constexpr int COLOR_TEXT_MAIN_B = 255;
	static constexpr int COLOR_SELECTED_BG_R = 255;
	static constexpr int COLOR_SELECTED_BG_G = 230;
	static constexpr int COLOR_SELECTED_BG_B = 160;

	static constexpr int COLOR_PARTICLE_R = 220;
	static constexpr int COLOR_PARTICLE_G = 220;
	static constexpr int COLOR_PARTICLE_B = 220;

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawUI(void) override;
	void UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)override;


private:

	// 画像
	int imgTitle_;
	// 音
	int titleBgm_;
	int clickSe_;

	Particle particles_[PARTICLE_COUNT];

	// 選択中のメニューインデックス
	int menuSelected_;

	// メニュー文字列の配列
	const char* menuItems_[2];

	float menuEffectPhase_;

	bool isConfirmingTutorial_;     // チュートリアル確認中かどうか
	int confirmSelected_;           // 「はい」「いいえ」の選択（0: はい, 1: いいえ）

	int prevPOVDirection_;

	float highlightPosY_;
	float confirmHighlightPosX_;
	float confirmHighlightPosY_;

	void DrawParticles(void);
};
