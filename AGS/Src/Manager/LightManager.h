#pragma once
#include <DxLib.h>

class LightManager
{
public:

	// 初期座標
	const VECTOR INIT_POS = { 0.0f,0.0f,0.0f };

	// 初期の向き
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

	// 座標
	VECTOR pos_;

	// 向き
	VECTOR dir_;

	// 有効距離
	float range_;

	// 距離減衰パラメータ０
	float atten0_;

	// 距離減衰パラメータ１
	float atten1_;

	// 距離減衰パラメータ２
	float atten2_;

	// ライトハンドルのカラー値
	COLOR_F color_;

	// ライトのタイプ
	Light_TYPE lType_;
};

