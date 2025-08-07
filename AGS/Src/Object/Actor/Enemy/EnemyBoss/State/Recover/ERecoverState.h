#pragma once
#include "../IEnemyState.h"
class ERecoverState 
	: public IEnemyState
{

public:

	static constexpr float DURATION = 0.5f;

	ERecoverState(void);
	~ERecoverState(void);

	void Enter(EnemyBoss& boss)override;
	void Update(EnemyBoss& boss)override;
	void Exit(EnemyBoss& boss)override;

private:
};

