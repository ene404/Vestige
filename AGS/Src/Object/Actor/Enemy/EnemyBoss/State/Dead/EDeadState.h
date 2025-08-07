#pragma once
#include "../IEnemyState.h"
class EDeadState 
	: public IEnemyState
{

public:

	EDeadState(void);
	~EDeadState(void);

	void Enter(EnemyBoss& boss)override;
	void Update(EnemyBoss& boss)override;
	void Exit(EnemyBoss& boss)override;

private:

	// ���S�A�j���[�V�������Đ����ꂽ���ǂ���
	bool deathAnimationStarted_;

	// ���S�A�j���[�V�������I�����ꂽ���ǂ���
	bool deathAnimationFinishedReported_;

	// ���S�X�e�[�g�ɓ����Ă���̌o�ߎ���
	float timer_;

	// ���S�A�j���[�V�����̍Đ�����
	float deathAnimationDuration_;
};

