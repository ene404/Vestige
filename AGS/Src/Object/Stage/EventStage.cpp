#include <vector>
#include <map>
#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Actor/Player/EventPlayer.h"
#include "../Actor/Enemy/EventEnemy.h"
#include "Planet.h"
#include "../Common/Collider.h"
#include "../Common/Transform.h"
#include "EventStage.h"

EventStage::EventStage(EventPlayer& player, EventEnemy& enemy)
    :resMng_(ResourceManager::GetInstance()), player_(player),enemy_(enemy)
{
    activeName_ = NAME::MAIN_PLANET;
    step_ = 0.0f;
}

EventStage::~EventStage(void)
{
    // �f��
    planets_.clear();
}

void EventStage::Init(void)
{
    MakeMainStage();

    step_ = -1.0f;
}

void EventStage::Update(void)
{
    // �f��
    for (const auto& s : planets_)
    {
        s.second->Update();
    }
}

void EventStage::Draw(void)
{
    // �f��
    for (const auto& s : planets_)
    {
        s.second->Draw();
    }
}

void EventStage::ChangeStage(NAME type)
{
    activeName_ = type;

    // �Ώۂ̃X�e�[�W���擾����
    activePlanet_ = GetPlanet(activeName_);

    // �X�e�[�W�̓����蔻����C�x���g�V�[���p�̃v���C���[�ɐݒ�
    player_.ClearCollider();
    player_.AddCollider(activePlanet_.lock()->GetTransform().collider);

    // �X�e�[�W�̓����蔻����C�x���g�V�[���p�̓G�ɐݒ�
    enemy_.ClearCollider();
    enemy_.AddCollider(activePlanet_.lock()->GetTransform().collider);
    
    step_ = TIME_STAGE_CHANGE;
}

std::weak_ptr<Planet> EventStage::GetPlanet(NAME type)
{
    if (planets_.count(type) == 0)
    {
        return nullPlanet;
    }

    return planets_[type];
}

void EventStage::MakeMainStage(void)
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
