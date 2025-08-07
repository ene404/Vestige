#pragma once
#include <memory>
#include <string>
#include "SceneBase.h"
class EventStage;
class SkyDome;
class EventPlayer;
class EventEnemy;
class Mist;

class EventScene : public SceneBase
{
public:

	// �t�H���g�֘A
	static constexpr int FONT_SIZE = 24;
	static constexpr int FONT_THICK = 6;
	static constexpr int FONT_MAX_COL = 255;
	static constexpr int FONT_GRAY_COL = 50;
	static constexpr int BAR_HEIGHT = 20;
	static constexpr int BAR_MAX_WIDTH = 200;
	static constexpr int BAR_X_ADJUSTMOMENT = 220;
	static constexpr int BAR_Y_ADJUSTMOMENT = 32;

	static constexpr int FONT_MARGIN_RIGHT = 30;
	static constexpr int FONT_MARGIN_UNDER = 64;

	// �J�������[�N�֘A
	static constexpr float START_TIME_LIMIT = 1.5f;
	static constexpr float LOOK_TIME_LIMIT = 4.0f;
	static constexpr float PAN_TIME_LIMIT_FIRST = 0.5f;
	static constexpr float PAN_TIME_LIMIT_SECOND = 2.0f;

	static constexpr float PLAYER_MOVED_LIMIT = 300.0f;

	// �X�L�b�v�@�\�̑ҋ@����
	static constexpr int LONG_PRESS_DURATION_MS = 2000;

	// �T�E���h�֘A
	static constexpr int BGM_VALUME = 255;
	static constexpr int INPUTKEY_VALUME = 255;
	static constexpr int COUNTMAX_VALUME = 255;

	// �C�x���g�V�[���p�J�����֘A
	static constexpr VECTOR EVENT_CAMERA_POS = { -50.0f, -60.0f, 260.0f };
	static constexpr VECTOR EVENT_TARGET_POS = { 0.0f, 80.0f, 0.0f };

	static constexpr VECTOR EVENT_START_CAMERA_POS = { -50.0f, -60.0f, 260.0f };
	static constexpr VECTOR EVENT_START_TARGET_POS = { 0.0f, 80.0f, 0.0f };

	static constexpr VECTOR EVENT_STEP_CAMERA_POS = { -50.0f, 30.0f, 500.0f };
	static constexpr VECTOR EVENT_STEP_TARGET_POS = { -100.0f, 30.0f, 200.0f };

	static constexpr float EVENT_LOOK_CAMER_POS_X = 50.0f;
	static constexpr float EVENT_LOOK_CAMER_POS_Y = 120.0f;
	static constexpr float EVENT_LOOK_CAMER_POS_Z = 300.0f;
	static constexpr VECTOR EVENT_LOOK_TARGET_POS = { 20.0f, 50.0f, 0.0f };

	static constexpr VECTOR EVENT_PAN_CAMERA_START_POS = { 0.0f, 80.0f, 0.0f };
	static constexpr VECTOR EVENT_PAN_CAMERA_END_POS = { 0.0f, 50.0f, 300.0f };
	static constexpr VECTOR EVENT_PAN_ENEMY_POS = { 0.0f, 100.0f, 0.0f };


	static constexpr float EVENT_ROAR_CAMERA_POS_X = -30.0f;
	static constexpr float EVENT_ROAR_CAMERA_POS_Y = 25.0f;
	static constexpr float EVENT_ROAR_TIME_ADJUSTMOMENT = 40.0f;
	static constexpr float EVENT_ROAR_CAMERA_POS_Z = -200.0f;
	static constexpr VECTOR EVENT_ROAR_ENEMY_POS = { 0.0f, 80.0f, 0.0f };


	enum class STATE {
		START,
		STEP_PLAYER_FORWARD,
		LOOK_AROUND,
		CAMERA_PAN_TO_ENEMY,
		ENEMY_ROAR,
	};

	// �R���X�g���N�^
	EventScene(void);

	// �f�X�g���N�^
	~EventScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawUI(void) override;
	void UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)override;

	STATE GetState();
	float GetStateTimer();

	std::shared_ptr<EventPlayer> GetPlayer();
	std::shared_ptr<EventEnemy> GetEnemy();

	bool IsStateEnterFrame(void) const;

private:

	STATE currentState_;
	float stateTimer_;

	// �X�e�[�W
	std::shared_ptr<EventStage> stage_;

	// ��
	std::shared_ptr<Mist> mist_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	// �v���C���[
	std::shared_ptr<EventPlayer> player_;

	// �G
	std::shared_ptr<EventEnemy> enemy_;

	std::string GetStateName(STATE state);

	// �������֘A
	bool isLongPressing = false;
	int longPressStartTime; // �������J�n���� (DxLib��GetNowCount()�Ŏ擾)

	bool isInitialized_;

	// �v���O���X�o�[UI�̐F
	int progressColor; // ��

	// �t�H���g�n���h��
	int customFontHandle_;

	void DrawProgressBar(int elapsedTime, int totalDuration);

	void InitSound(void);

	// �T�E���h�n���h��
	int bgmSHandle_;
	int iKeySHandle_;
	int cMaxSHandle_;

	// ���t���[�����𐶐������Ȃ�����
	bool isBgm_;

	// �L�[�������Ă���t���[����0���ǂ���
	bool isStateFrame_;
};