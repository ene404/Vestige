#pragma once
#include <DxLib.h>

class LightManager
{
public:

	// �������W
	const VECTOR INIT_POS = { 0.0f,0.0f,0.0f };

	// �����̌���
	const VECTOR INIT_DIR = { 0.3f, -0.7f, 0.8f };

	enum class Light_TYPE
	{
		NONE,
		DIRECTIONAL,
		POINT,
		SPOT
	};

	LightManager(void);
	~LightManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	void SetLightColor(COLOR_F color);

private:

	// ���W
	VECTOR pos_;

	// ����
	VECTOR dir_;

	// �L������
	float range_;

	// ���������p�����[�^�O
	float atten0_;

	// ���������p�����[�^�P
	float atten1_;

	// ���������p�����[�^�Q
	float atten2_;

	// ���C�g�n���h���̃J���[�l
	COLOR_F color_;

	// ���C�g�̃^�C�v
	Light_TYPE lType_;
};

