#pragma once
#include "../IEnemyState.h"
#include <DxLib.h>
class EStepState 
	: public IEnemyState
{

public:

	// �o�b�N�X�e�b�v�̈ړ�����
	static constexpr float DURATION = 0.7f;

	// ��x�ڂ̈ړ�����
	static constexpr float FIRST_DIS = 10.0f;

	// ��x�ڂ̈ړ�����
	static constexpr float SECOND_DIS = 13.0f;
	
	// �X�e�b�v�̉�
	static constexpr int STEP_MAX_COUNT = 2;
	
	// �~����
	static constexpr float PI = 3.14159f;

	EStepState(void);
	~EStepState(void);

	void Enter(EnemyBoss& boss)override;
	void Update(EnemyBoss& boss)override;
	void Exit(EnemyBoss& boss)override;

private:

	// �X�e�b�v�̉�
	int stepCount_;

	// �X�e�b�v�X�e�[�g���̎���
	float backStepTimer_;

	// ����
	float height_;

	// �ړ��v�Z
	VECTOR CalculateBackstepPosition(const VECTOR& startPos, const VECTOR& dir, float elapsedTime, float duration, float distance, float height);
};

