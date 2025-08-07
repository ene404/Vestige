#pragma once
#include <map>
#include <functional>
#include "../EnemyBoss.h"
#include "IEnemyState.h"

// ��ԑJ�ڊǗ�
class EnemyStateMachine
{

public:

	EnemyStateMachine(EnemyBoss& boss);
	~EnemyStateMachine();

	void Add(EnemyBoss::STATE stateId, std::shared_ptr<IEnemyState> stateInstance);
	void ChangeState(EnemyBoss::STATE newState);
	void Update(EnemyBoss& boss);
	EnemyBoss::STATE GetCurrentStateId() const;

	void DrawDebugInfo(const VECTOR& screenPos);

private:
	EnemyBoss& boss_;
	std::map<EnemyBoss::STATE, std::shared_ptr<IEnemyState>> states_;
	std::shared_ptr<IEnemyState> curState_;
	EnemyBoss::STATE curStateId_;

	static const std::map<EnemyBoss::STATE, const char*> stateNames_;
};



