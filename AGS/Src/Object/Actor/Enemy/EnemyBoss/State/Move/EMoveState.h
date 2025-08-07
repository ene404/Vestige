#pragma once
#include "../IEnemyState.h"
class EMoveState 
	: public IEnemyState
{

public:

	// �ڕW�Ƃ���v���C���[����̋���
	static constexpr float TARGET_DISTANCE = 350.0f;

	// �ړ����x
	static constexpr float MOVE_SPEED = 2.0f;

	// �ڕW��������̋��e�덷
	static constexpr float DISTANCE_TOLERANCE = 50.0f;

	EMoveState(void);
	~EMoveState(void);

	void Enter(EnemyBoss& boss)override;
	void Update(EnemyBoss& boss)override;
	void Exit(EnemyBoss& boss)override;

private:

};

