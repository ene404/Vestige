#pragma once
#include <map>
#include <memory>
#include "../../EnemyBoss.h"

class IEnemyAttackState;

class EnemyBossAttackStateMachine
{

public:

	EnemyBossAttackStateMachine(void);
	~EnemyBossAttackStateMachine(void);

	// �U���p�^�[���̒ǉ�
	void Add(EnemyBoss::ATK_STATE type, std::shared_ptr<IEnemyAttackState> state);

	// �T�u�X�e�[�g(�U���p�^�[��)��ύX
	void Change(EnemyBoss& boss, EnemyBoss::ATK_STATE type);
	void Update(EnemyBoss& boss);
	
	// ���݂̍U���p�^�[���̎擾
	EnemyBoss::ATK_STATE GetCurrentStateType() const;
	
	// �U����Ԃ����Z�b�g
	void Reset(EnemyBoss& boss);	
	
	// ���݂̍U���������������H
	bool IsFinished() const;

	// �G�t�F�N�g�̒�~
	void StopEffect(void);

	void DrawDebugInfo(const VECTOR& screenPos);

private:
	std::map<EnemyBoss::ATK_STATE, std::shared_ptr<IEnemyAttackState>> states_;
	std::shared_ptr<IEnemyAttackState> curState_;
	EnemyBoss::ATK_STATE curStateId_;

	static const std::map<EnemyBoss::ATK_STATE, const char*> attackStateNames_;
};

