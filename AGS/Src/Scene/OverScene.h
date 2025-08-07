#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class OverScene : public SceneBase
{

public:
	// 音
	static constexpr int OVER_BGM_VOL = 180;
	static constexpr int ENTER_SE_VOL = 255;

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

	static constexpr float PHASE_INCREMENT = 0.05f;

	static constexpr float OVER_IMAGE_SCALE = 0.7f;
	static constexpr int HINT_FONT_SIZE = 32;
	static constexpr int HINT_Y = 480;
	static constexpr int HINT_BG_ALPHA = 150;
	static constexpr int HINT_BG_PADDING_X = 20;
	static constexpr int HINT_BG_PADDING_Y = 5;
	static constexpr int HINT_BG_HEIGHT = 35;
	static constexpr int HINT_SHADOW_OFFSET = 2;
	static constexpr int COLOR_HINT_TEXT_R = 255;
	static constexpr int COLOR_HINT_TEXT_G = 230;
	static constexpr int COLOR_HINT_TEXT_B = 180;
	static constexpr int OVER_FONT_SIZE_SMALL = 16;

	static constexpr int COLOR_PARTICLE_R = 220;
	static constexpr int COLOR_PARTICLE_G = 220;
	static constexpr int COLOR_PARTICLE_B = 220;

	// コンストラクタ
	OverScene(void);

	// デストラクタ
	~OverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawUI(void) override;
	void UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)override;


private:
	// 音
	int overBgm_;
	bool isBgm_;
	int enterSe_;

	void DrawParticles(void);
	// 画像
	int imgOver_;

	float phase_;
	struct Particle {
		float x, y, vx, vy;
		int size, alpha;
	} particles_[PARTICLE_COUNT];
};
