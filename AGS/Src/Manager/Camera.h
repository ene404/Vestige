#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
class ResourceManager;
class Transform;

class Camera
{

public:

	// �J�����X�s�[�h(�x)
	static constexpr float SPEED = 1.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 50.0f, -400.0f };

	// �Ǐ]�ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 500.0f };

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 220.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 280.0f, -350.0f };

	// �p�b�g�̎��̃J�����̐���
	static constexpr float PAT_CAMERA_MAX_RX = 100.0f;
	static constexpr float PAT_CAMERA_MIN_RX = -100.0f;
	static constexpr float PAT_CAMERA_MAX_RY = 100.0f;
	static constexpr float PAT_CAMERA_MIN_RY = -100.0f;

	// �J�����A���O���̐���
	static constexpr float CAMERA_MAX_ANGLE_X = 60.0f;
	static constexpr float CAMERA_MIN_ANGLE_X = -60.0f;


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

	// �J�������[�h

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		FOLLOW_MOUSE,
		SELF_SHOT,
		EVENT
	};

	enum class GAME_CAMERA
	{
		NONE,
		MOUSE,
		TARGET
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void Draw(void);

	// �J�����ʒu
	VECTOR GetPos(void) const;
	// �J�����̑���p�x
	VECTOR GetAngles(void) const;
	// �J�����̒����_
	VECTOR GetTargetPos(void) const;

	// �J�����p�x
	Quaternion GetQuaRot(void) const;
	// X��]�𔲂����J�����p�x
	Quaternion GetQuaRotOutX(void) const;
	// �J�����̑O������
	VECTOR GetForward(void) const;

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);
	void ChangeGameCamera(GAME_CAMERA gameCamera);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);
	void SetFollowEnemy(const Transform* followEnemy);

	void MouseMove(float* x_m, float* y_m, const float fov_per);

	// ���b�N�I���}�[�J�[
	void DrawLockonMarker(void);
	void LockonSoundLoad(void);

	void ResetEventCameraState(void);

private:

	// ��
	int lockonSe_;

	// �V���O���g���Q��
	ResourceManager& resMng_;

	// �J�������Ǐ]�ΏۂƂ���Transform
	const Transform* followTransform_;
	const Transform* followEnemyTransform_;

	// �J�������[�h
	MODE mode_;

	// �Q�[�����̃J����
	GAME_CAMERA gameCamera_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// X����]�������p�x
	Quaternion rotOutX_;

	// �J�����p�x
	Quaternion rot_;

	// �����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	int lockonFrame_;

	bool isInitialized_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �Ǐ]�ΏۂƂ̈ʒu���������
	void SyncFollow(void);

	// �J��������
	void ProcessRot(void);

	// ���[�h�ʍX�V�X�e�b�v
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowMouse(void);
	void SetBeforeDrawSelfShot(void);
	void SetBeforeDrawEvent(void);

};

