#include "LightManager.h"

LightManager::LightManager(void)
	:
	pos_({}),
	dir_({}),
	range_(0.0f),
	atten0_(0.0f),
	atten1_(0.0f),
	atten2_(0.0f)
	,color_({})
{
}

LightManager::~LightManager(void)
{
	pos_ = INIT_POS;
	dir_ = INIT_DIR;

}

void LightManager::Init(void)
{
}

void LightManager::Update(void)
{
}

void LightManager::Draw(void)
{
}

void LightManager::SetLightColor(COLOR_F color)
{
	color_ = color;
}
