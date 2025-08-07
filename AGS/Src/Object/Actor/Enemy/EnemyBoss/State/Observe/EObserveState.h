#pragma once
#include "../IEnemyState.h"
class EObserveState 
	: public IEnemyState
{

public:

	static constexpr float DIST_LIMIT = 120.0f;
	static constexpr float OBSERVE_TIME = 3.0f;

	EObserveState(void);
	~EObserveState(void);

	void Enter(EnemyBoss& boss)override;
	void Update(EnemyBoss& boss)override;
	void Exit(EnemyBoss& boss)override;

public:

};

