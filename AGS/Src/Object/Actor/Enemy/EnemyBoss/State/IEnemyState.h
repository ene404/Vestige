#pragma once
class EnemyBoss;

class IEnemyState
{

public:

	virtual void Enter(EnemyBoss& boss) = 0;
	virtual void Update(EnemyBoss& boss) = 0;
	virtual void Exit(EnemyBoss& boss) = 0;
	virtual ~IEnemyState(void) = default;


protected:
	float time_;
};

