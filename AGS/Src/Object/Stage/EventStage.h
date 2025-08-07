#pragma once
#include <map>
#include "../Common/Transform.h"
class ResourceManager;
class WarpStar;
class Planet;
class EventPlayer;
class EventEnemy;

class EventStage
{
public:

	// �X�e�[�W�̐؂�ւ��Ԋu
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	// �X�e�[�W��
	enum class NAME
	{
		MAIN_PLANET
	};

	// �R���X�g���N�^
	EventStage(EventPlayer& player, EventEnemy& enemy);

	// �f�X�g���N�^
	~EventStage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// �X�e�[�W�ύX
	void ChangeStage(NAME type);

	// �ΏۃX�e�[�W���擾
	std::weak_ptr<Planet> GetPlanet(NAME type);

private:

	// �V���O���g���Q��
	ResourceManager& resMng_;

	EventPlayer& player_;

	EventEnemy& enemy_;

	// �X�e�[�W�A�N�e�B�u�ɂȂ��Ă���f���̏��
	NAME activeName_;
	std::weak_ptr<Planet> activePlanet_;

	// �f��
	std::map<NAME, std::shared_ptr<Planet>> planets_;

	// ���Planet
	std::shared_ptr<Planet> nullPlanet = nullptr;

	float step_;

	// �ŏ��̃X�e�[�W
	void MakeMainStage(void);
};

