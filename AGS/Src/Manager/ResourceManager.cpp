#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// �������܂��񂪁A�ǂ����Ă��g����������
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_SND = Application::PATH_SOUND;

	std::shared_ptr<Resource> res;

	// �^�C�g���摜
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE, res);

	// �Q�[���I�[�o�[�摜
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Over.png");
	resourcesMap_.emplace(SRC::OVER, res);

	// �N���A�摜
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Clear.png");
	resourcesMap_.emplace(SRC::CLEAR, res);

	// �N���A�摜
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Light.png");
	resourcesMap_.emplace(SRC::LIGHT, res);

	// PushSpace
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, res);

	// �����o��
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "SpeechBalloon.png");
	resourcesMap_.emplace(SRC::SPEECH_BALLOON, res);
	
	// �v���C���[
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	// �v���C���[�e
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, res);

	// �v���C���[�_�~�[
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Dummy/dummyE2.mv1");
	resourcesMap_.emplace(SRC::PLAYER_DUMMY, res);

	// �K�E�Z�Q�[�W
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "GaugeFrame.png");
	resourcesMap_.emplace(SRC::GAUGE_FRAME, res);

	// ���@�~
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "qqq.png");
	resourcesMap_.emplace(SRC::MAGIC_CIRCLE, res);

	// ���[��
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Rune.png");
	resourcesMap_.emplace(SRC::RUNE, res);

	// HP�|�[�V����
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "HpPotion.png");
	resourcesMap_.emplace(SRC::HP_POTION, res);

	// ULT�|�[�V����
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "UltPotion.png");
	resourcesMap_.emplace(SRC::ULT_POTION, res);

	// ���@��
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Book.png");
	resourcesMap_.emplace(SRC::BOOK, res);

	// Now Loading
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Loading.png");
	resourcesMap_.emplace(SRC::NOW_LOADING, res);

	// �G
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Enemy/enemy.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BASE, res);

	// �G�_�~�[
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Dummy/dummyE2.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DUMMY, res);

	// �X�J�C�h�[��
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	// �ŏ��̘f��
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "stage/stage.mv1");
	resourcesMap_.emplace(SRC::MAIN_PLANET, res);

	// ��
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Gate/GateMist.mv1");
	resourcesMap_.emplace(SRC::MIST, res);

	// ����
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, res);

	// ���[�v�X�^�[���f��
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Star/star.mv1");
	resourcesMap_.emplace(SRC::WARP_STAR, res);

	// ���[�v�X�^�[�p��]�G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "StarDust/StarDust.efkefc");
	resourcesMap_.emplace(SRC::WARP_STAR_ROT_EFF, res);

	// ���[�v�̋O�Ր�
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "Warp/WarpOrbit.efkefc");
	resourcesMap_.emplace(SRC::WARP_ORBIT, res);

	// �u���b�N�z�[��
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "BlackHole/BlackHole.efkefc");
	resourcesMap_.emplace(SRC::BLACK_HOLE, res);

	// �S�[��
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "GoalStar/GoalStar.mv1");
	resourcesMap_.emplace(SRC::GOAL_STAR, res);

	// �^���N
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Tank/Body.mv1");
	resourcesMap_.emplace(SRC::TANK_BODY, res);
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Tank/Wheel.mv1");
	resourcesMap_.emplace(SRC::TANK_WHEEL, res);
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Tank/Barrel.mv1");
	resourcesMap_.emplace(SRC::TANK_BARREL, res);


	// ��
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Sword/Sword2.mv1");
	resourcesMap_.emplace(SRC::SWORD, res);

	// �{�X
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Enemy/enemy.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BOSS, res);
	
	// ������
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Scarecrow/Scarecrow.mv1");
	resourcesMap_.emplace(SRC::ENEMY_SCARECROW, res);

	// ULT�G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "Ult.efkefc");
	resourcesMap_.emplace(SRC::ULT_EFFECT, res);

	// HIT�G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "Hit.efkefc");
	resourcesMap_.emplace(SRC::HIT_EFFECT, res);

	// ���C�g�j���O�G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "Lightning.efkefc");
	resourcesMap_.emplace(SRC::LIGHTNING_EFFECT, res);

	// �o�t�G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "EEffect/enemy_buff.efkefc");
	resourcesMap_.emplace(SRC::BUFF_EFFECT, res);

	// �����G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "EEffect/explosion.efkefc");
	resourcesMap_.emplace(SRC::EXPLOSION_EFFECT, res);

	// ���K01�G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "EEffect/roar_01.efkefc");
	resourcesMap_.emplace(SRC::ROAR_01, res);

	// ���K02�G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "EEffect/roar_03.efkefc");
	resourcesMap_.emplace(SRC::ROAR_02, res);

	// �X�^���G�t�F�N�g
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "EEffect/stun.efkefc");
	resourcesMap_.emplace(SRC::STUN_EFFECT, res);

	// �^�C�g��BGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);

	// �`���[�g���A��BGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Tutorial.mp3");
	resourcesMap_.emplace(SRC::TUTORIAL_BGM, res);

	// �Q�[��BGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Game.mp3");
	resourcesMap_.emplace(SRC::GAME_BGM, res);

	// �N���ABGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Clear.mp3");
	resourcesMap_.emplace(SRC::CLEAR_BGM, res);

	// �Q�[���I�[�o�[BGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Over.mp3");
	resourcesMap_.emplace(SRC::OVER_BGM, res);

	// �N���b�NSE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Click.mp3");
	resourcesMap_.emplace(SRC::CLICK_SE, res);

	// ���[�����OSE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Rolling.mp3");
	resourcesMap_.emplace(SRC::ROLLING_SE, res);

	// �A�^�b�NSE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Attack.mp3");
	resourcesMap_.emplace(SRC::ATTACK_SE, res);

	// �q�b�gSE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Hit.mp3");
	resourcesMap_.emplace(SRC::HIT_SE, res);

	// ���b�N�I��SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Lockon.mp3");
	resourcesMap_.emplace(SRC::LOCKON_SE, res);

	// �K�ESE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Ult.mp3");
	resourcesMap_.emplace(SRC::ULT_SE, res);

	// �_���[�WSE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Damage.mp3");
	resourcesMap_.emplace(SRC::DAMAGE_SE, res);

	// HP�|�[�V����SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "HpPotion.mp3");
	resourcesMap_.emplace(SRC::HP_POTION_SE, res);

	// ULT�|�[�V����SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "UltPotion.mp3");
	resourcesMap_.emplace(SRC::ULT_POTION_SE, res);

	// ���@��SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Book.mp3");
	resourcesMap_.emplace(SRC::BOOK_SE, res);
	
	// ���C�g�j���OSE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Lightning.mp3");
	resourcesMap_.emplace(SRC::LIGHTNING_SE, res);

	// NEXT_SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Next.mp3");
	resourcesMap_.emplace(SRC::NEXT_SE, res);

	// DEAD_SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Dead.mp3");
	resourcesMap_.emplace(SRC::DEAD_SE, res);

	// ROAR_SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "roar.mp3");
	resourcesMap_.emplace(SRC::ROAR_SE, res);

	// EXP_SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "explosion.mp3");
	resourcesMap_.emplace(SRC::EXP_SE, res);

	// UNSHEATHED_BGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "unsheathed.mp3");
	resourcesMap_.emplace(SRC::UNSHEATHED_BGM, res);

	// WALK_BGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "walk.mp3");
	resourcesMap_.emplace(SRC::WALK_BGM, res);

	// GAUGE_BGM1
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "cMax.mp3");
	resourcesMap_.emplace(SRC::GAUGE_BGM1, res);

	// GAUGE_BGM2
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "gauge2.mp3");
	resourcesMap_.emplace(SRC::GAUGE_BGM2, res);

	// EVENT_BGM
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Event.mp3");
	resourcesMap_.emplace(SRC::EVENT_BGM, res);

	// HELP_SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + "Help.mp3");
	resourcesMap_.emplace(SRC::HELP_SE, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		//delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// ���\�[�X�o�^�`�F�b�N
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return dummy_;
	}

	// ���[�h����
	rPair->second->Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
