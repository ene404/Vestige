#include <vector>
#include <map>
#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "WarpStar.h"
#include "../Actor/Player/Player.h"
#include "Planet.h"
#include "../Actor/Enemy/EnemyBoss/EnemyBoss.h"
#include "../Common/Collider.h"
#include "../Common/Transform.h"
#include "Stage.h"

Stage::Stage(Player& player, EnemyBoss& enemyBoss)
	: resMng_(ResourceManager::GetInstance()), player_(player), enemyBoss_(enemyBoss)
{
	activeName_ = NAME::MAIN_PLANET;
	step_ = 0.0f;
}

Stage::~Stage(void)
{
	
	// ���[�v�X�^�[
	warpStars_.clear();
	
	// �f��
	planets_.clear();

}

void Stage::Init(void)
{
	MakeMainStage();
	MakeWarpStar();

	step_ = -1.0f;
}

void Stage::Update(void)
{

	// ���[�v�X�^�[
	for (const auto& s : warpStars_)
	{
		s->Update();
	}

	// �f��
	for (const auto& s : planets_)
	{
		s.second->Update();
	}

}

void Stage::Draw(void)
{

	//// ���[�v�X�^�[
	//for (const auto& s : warpStars_)
	//{
	//	s->Draw();
	//}

	// �f��
	for (const auto& s : planets_)
	{
		s.second->Draw();
	}

}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// �Ώۂ̃X�e�[�W���擾����
	activePlanet_ = GetPlanet(activeName_);

	// �X�e�[�W�̓����蔻����v���C���[�ɐݒ�
	player_.ClearCollider();
	player_.AddCollider(activePlanet_.lock()->GetTransform().collider);

	enemyBoss_.ClearCollider();
	enemyBoss_.AddCollider(activePlanet_.lock()->GetTransform().collider);
	/*enemyBoss_.AddCollider(player_.GetTransform().collider);
	enemyBoss_.ResolveCollisions();*/

	step_ = TIME_STAGE_CHANGE;

}

std::weak_ptr<Planet> Stage::GetPlanet(NAME type)
{
	if (planets_.count(type) == 0)
	{
		return nullPlanet;
	}

	return planets_[type];
}

void Stage::MakeMainStage(void)
{

	// �ŏ��̘f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	planetTrans.scl = { 5.0f, 5.0f, 5.0f };
	planetTrans.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(90.0f),
		0.0f
	);
	planetTrans.pos = { 0.0f, -100.0f, 0.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::MAIN_PLANET;
	Planet* planet =
		new Planet(
			name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	planets_.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeWarpStar(void)
{

	Transform trans;
	std::unique_ptr<WarpStar> star;

	// ���Ƃ����f���ւ̃��[�v�X�^�[
	//------------------------------------------------------------------------------
	trans.pos = { -910.0f, 200.0f, 894.0f };
	trans.scl = { 0.6f, 0.6f, 0.6f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-25.0f),
		AsoUtility::Deg2RadF(-50.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	star = std::make_unique<WarpStar>(player_, trans);
	star->Init();
	warpStars_.push_back(std::move(star));
	//------------------------------------------------------------------------------

}
