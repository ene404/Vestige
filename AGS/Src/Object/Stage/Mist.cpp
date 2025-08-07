#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Transform.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "Mist.h"

Mist::Mist(void)
{
}

Mist::~Mist(void)
{
}

void Mist::Init(void)
{

	transforms_.resize(6);
	for (int i = 0;i < 6;i++)
	{
		transforms_[i].SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::MIST));
		transforms_[i].scl = {8.0f, 13.0f, 1.5f};
	}

	transforms_[0].pos = { 0.0f,500.0f,4000.0f };
	transforms_[0].scl = { 8.0f, 15.0f, 1.5f };

	transforms_[1].pos = { 1800.0f,500.0f,4000.0f };
	transforms_[1].scl = { 3.0f, 7.0f, 1.5f };

	transforms_[2].pos = { -1800.0f,500.0f,4000.0f };
	transforms_[2].scl = { 3.0f, 7.0f, 1.5f };
	transforms_[3].pos = { 0.0f,500.0f,-4000.0f };
	transforms_[3].scl = { 8.0f, 15.0f, 1.5f };

	transforms_[4].pos = { 1800.0f,300.0f,-4000.0f };
	transforms_[4].scl = { 3.0f, 7.0f, 1.5f };

	transforms_[5].pos = { -1800.0f,300.0f,-4000.0f };
	transforms_[5].scl = { 3.0f, 7.0f, 1.5f };

	for (int i = 0; i < 6; i++)
	{
		transforms_[i].Update();
	}

	// ���f���`��p
	material_ = std::make_unique<ModelMaterial>(
		"GateFogVS.cso", 0,
		"GateFogPS.cso", 1
		);

	// ���Ԃ̒ǉ�
	time_ = SceneManager::GetInstance().GetDeltaTime();

	// �X�N���[�����x
	float scrollSpeed = 0.001f;

	material_->AddConstBufPS({ time_,scrollSpeed,0.0f,0.0f });

	renderer_.resize(6);
	for (int i = 0; i < 6; i++)
	{
		renderer_[i] = std::make_unique<ModelRenderer>(
			transforms_[i].modelId, *material_
			);
	}
}

void Mist::Update(void)
{
}

void Mist::Draw(void)
{
	// ���Ԃ̒ǉ�
	time_ += SceneManager::GetInstance().GetDeltaTime();

	// �X�N���[�����x
	float scrollSpeed = 0.1f;
	material_->SetConstBufPS(0, { time_,scrollSpeed, 0.0f, 1.0f });

	for (int i = 0; i < 6; i++)
	{
		renderer_[i]->Draw();
	}
}

std::vector<Transform>& Mist::GetMistTransforms(void)
{
	return transforms_;
}
