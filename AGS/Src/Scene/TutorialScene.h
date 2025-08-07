#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class Stage;
class Mist;
class SkyDome;
class Player;
class EnemyBoss;
class EnemyScarecrow;
class EnemyScarecrowDummy;
class PixelMaterial;
class PixelRenderer;

class TutorialScene :
	public SceneBase
{
public:
	// 音
	static constexpr int TUTORIAL_BGM_VOL = 150;
	static constexpr int NEXT_SE_VOL = 220;
	static constexpr int HELP_SE_VOL = 220;

	static constexpr float MILLISECONDS_TO_SECONDS = 1000.0f;
	static constexpr float LIGHTNING_POWER_MAX = 1.0f;
	static constexpr float LIGHTNING_DECAY_RATE = 0.05f;
	static constexpr float TIME_TO_NEXT_SCENE = 5.0f;

	static constexpr float TUTORIAL_BOX_WIDTH_RATIO = 0.41f;
	static constexpr int TUTORIAL_BOX_HEIGHT = 100;
	static constexpr int TUTORIAL_BOX_X = 20;
	static constexpr int TUTORIAL_BOX_CORNER_RADIUS = 14;
	static constexpr int BG_ALPHA = 120;
	static constexpr int BORDER_LAYER_COUNT = 3;
	static constexpr int BORDER_ALPHA_START = 50;
	static constexpr int BORDER_ALPHA_STEP = 15;
	static constexpr int BORDER_COLOR_R = 100;
	static constexpr int BORDER_COLOR_G = 20;
	static constexpr int BORDER_COLOR_B = 40;
	static constexpr int INNER_BORDER_COLOR_R = 70;
	static constexpr int INNER_BORDER_COLOR_G = 70;
	static constexpr int INNER_BORDER_COLOR_B = 75;
	static constexpr int MAIN_FONT_SIZE = 18;
	static constexpr int SUB_FONT_SIZE = 16;
	static constexpr int TEXT_LEFT_PADDING = 22;
	static constexpr int TEXT_TOP_PADDING = 24;
	static constexpr int SUBTEXT_SPACING = 10;
	static constexpr int SHADOW_OFFSET = 1;
	static constexpr int SHADOW_OFFSET_2 = 2;
	static constexpr int MAIN_TEXT_SHADOW_COLOR1_R = 15;
	static constexpr int MAIN_TEXT_SHADOW_COLOR1_G = 10;
	static constexpr int MAIN_TEXT_SHADOW_COLOR1_B = 20;
	static constexpr int MAIN_TEXT_SHADOW_COLOR2_R = 40;
	static constexpr int MAIN_TEXT_SHADOW_COLOR2_G = 30;
	static constexpr int MAIN_TEXT_SHADOW_COLOR2_B = 50;
	static constexpr int MAIN_TEXT_COLOR_R = 210;
	static constexpr int MAIN_TEXT_COLOR_G = 210;
	static constexpr int MAIN_TEXT_COLOR_B = 220;
	static constexpr int SUB_TEXT_SHADOW_COLOR_R = 40;
	static constexpr int SUB_TEXT_SHADOW_COLOR_G = 35;
	static constexpr int SUB_TEXT_SHADOW_COLOR_B = 50;
	static constexpr int SUB_TEXT_COLOR_R = 170;
	static constexpr int SUB_TEXT_COLOR_G = 160;
	static constexpr int SUB_TEXT_COLOR_B = 180;
	static constexpr int ALPHA_SHIFT = 24;



	static constexpr int LOOK_MOUSE_MOVE_THRESHOLD = 2;
	static constexpr int LOOK_STICK_THRESHOLD = 100;
	static constexpr int REQUIRED_LOOK_MOVES = 60;

	static constexpr int STICK_MOVE_THRESHOLD = 500;

	static constexpr float REQUIRED_RUN_TIME = 2.0f;

	static constexpr float JUDGE_MARGIN = 1.0f;

	static constexpr int PAD_BUTTON_LOCKON = 9;

	static constexpr int PAD_BUTTON_ULT = 4;
	static constexpr int TUTORIAL_DAMAGE = 10;

	static constexpr int PAD_BUTTON_ITEM = 2;

	static constexpr int WHEEL_THRESHOLD = 3;
	static constexpr int POV_RIGHT = 9000;
	static constexpr int POV_LEFT = 27000;

	static constexpr int LIGHTNING_DAMAGE = 300;

	static constexpr int HELP_FONT_SIZE = 18;
	static constexpr int HELP_LINE_SPACING = 14;
	static constexpr int HELP_BOX_WIDTH = 660;
	static constexpr int HELP_TITLE_TOP_MARGIN = 40;
	static constexpr int HELP_PADDING = 20;
	static constexpr int HELP_CONTENT_TOP_OFFSET = 60;
	static constexpr int HELP_LABEL_LEFT = 40;
	static constexpr int HELP_INPUT_LEFT = 170;
	static constexpr int HELP_INPUT_OFFSET = 60;
	static constexpr int COLOR_WHITE_R = 255;
	static constexpr int COLOR_WHITE_G = 255;
	static constexpr int COLOR_WHITE_B = 255;
	static constexpr int COLOR_COLOR_BLACK_R = 0;
	static constexpr int COLOR_COLOR_BLACK_G = 0;
	static constexpr int COLOR_COLOR_BLACK_B = 0;


	enum class TutorialStep {
		LOOK,
		MOVE,
		DASH,
		ROLLING,
		ATTACK,
		COMBO_ATTACK,
		HEAVY_ATTACK,
		LOCKON,
		ULT,
		ITEM,
		ITEM_CHANGE,
		HELP,
		DEFEAT_ENEMY,
		NEXT
	};


	// コンストラクタ
	TutorialScene(void);

	// デストラクタ
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawUI(void) override;
	void UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)override;

private:

	// 音
	int tutorialBgm_;
	bool isBgm_;
	int nextSe_;
	int helpSe_;

	// ステージ
	std::unique_ptr<Stage> stage_;

	// ミスと
	std::shared_ptr<Mist> mist_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	// プレイヤー
	std::shared_ptr<Player> player_;

	std::shared_ptr<EnemyBoss> enemyBoss_;

	std::shared_ptr<EnemyScarecrow> enemyScarecrow_;

	std::shared_ptr<EnemyScarecrowDummy> enemyScarecrowDummy_;

	TutorialStep tutorialStep_;
	float uiAlpha_;

	int lookCounter_;
	int prevLookX_;
	int prevLookY_;

	bool movedW_;
	bool movedA_;
	bool movedS_;
	bool movedD_;

	bool dashed_;

	bool rolled_;

	bool attacked_;

	bool comboAttacked_;

	bool heavyAttacked_;

	bool lockedOn_;

	bool usedUlt_;

	bool usedItem_;

	bool itemChanged_;

	float nextStepTimer_;

	void DrawTutorialText(const char* text, const char* subTex);
	void LookUpdate(void);
	void MoveUpdate(void);
	void DashUpdate(void);
	void RollingUpdate(void);
	void AttackUpdate(void);
	void ComboAttackUpdate(void);
	void HeavyAttackUpdate(void);
	void LockonUpdate(void);
	void UltUpdate(void);
	void ItemUpdate(void);
	void ItemChangeUpdate(void);
	void HelpUpdate(void);
	void DefeatEnemyUpdate(void);

	// ポストエフェクト用スクリーン
	int postEffectScreen_;

	// ポストエフェクト用(ライトニング)
	std::unique_ptr<PixelMaterial> lightningMaterial_;
	std::unique_ptr<PixelRenderer> lightningRenderer_;
	float lightningTime_;
	float lightningPower_;

	bool isHelp_;
	int helpBackImage_;
	bool wasHelp_;
	void DrawHelp(void);
};

