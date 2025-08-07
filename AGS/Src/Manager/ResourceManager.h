#pragma once
#include <map>
#include <string>
#include <memory>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		TITLE,
		OVER,
		CLEAR,
		LIGHT,
		PUSH_SPACE,
		SPEECH_BALLOON,
		PLAYER,
		PLAYER_DUMMY,
		PLAYER_SHADOW,
		GAUGE_FRAME,
		MAGIC_CIRCLE,
		RUNE,
		HP_POTION,
		ULT_POTION,
		BOOK,
		NOW_LOADING,
		ENEMY_BASE,
		ENEMY_DUMMY,
		SKY_DOME,
		MAIN_PLANET,
		MIST,
		FOOT_SMOKE,
		WARP_STAR,
		WARP_STAR_ROT_EFF,
		WARP_ORBIT,
		BLACK_HOLE,
		GOAL_STAR,
		TANK_BODY,
		TANK_BARREL,
		TANK_WHEEL,
		SWORD,
		ENEMY_BOSS,
		ENEMY_SCARECROW,
		ULT_EFFECT,
		HIT_EFFECT,
		LIGHTNING_EFFECT,
		BUFF_EFFECT,
		EXPLOSION_EFFECT,
		ROAR_01,
		ROAR_02,
		STUN_EFFECT,

		TITLE_BGM,
		TUTORIAL_BGM,
		GAME_BGM,
		CLEAR_BGM,
		OVER_BGM,
		CLICK_SE,
		ROLLING_SE,
		ATTACK_SE,
		HIT_SE,
		LOCKON_SE,
		ULT_SE,
		DAMAGE_SE,
		HP_POTION_SE,
		ULT_POTION_SE,
		BOOK_SE,
		LIGHTNING_SE,
		NEXT_SE,
		DEAD_SE,
		ROAR_SE,
		EXP_SE,
		UNSHEATHED_BGM,
		WALK_BGM,
		GAUGE_BGM1,
		GAUGE_BGM2,
		EVENT_BGM,
		HELP_SE,
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, std::shared_ptr<Resource>> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
