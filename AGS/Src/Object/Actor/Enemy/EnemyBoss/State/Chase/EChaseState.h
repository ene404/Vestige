#pragma once
#include "../IEnemyState.h"
class EChaseState 
	: public IEnemyState
{

public:

	// �o�b�N�X�e�b�v���J�n���鋗��
	static constexpr float ATTACK_RANGE = 75.0f;

	// �U�����J�n���鋗�� (���̋������߂��ƍU������T��)
	static constexpr float ATTACK_INITIATE_RANGE = 800.0f;
	
	// �ǐՂ��鑬�x
	static constexpr float CHASE_SPEED = 5.5f;

	EChaseState(void);
	~EChaseState(void);

	void Enter(EnemyBoss& boss)override;
	void Update(EnemyBoss& boss)override;
	void Exit(EnemyBoss& boss)override;

public:
	float dist_;
};
