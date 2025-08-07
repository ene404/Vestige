#pragma once
class EnemyBoss;

class IEnemyAttackState
{

public:

	virtual ~IEnemyAttackState(void) = default;
	virtual void Enter(EnemyBoss& boss) = 0;			//	攻撃開始時の初期化
	virtual void Update(EnemyBoss& boss) = 0;			// 毎フレーム
	virtual void Exit(EnemyBoss& boss) = 0;
	virtual bool IsFinished(void) const = 0;// 攻撃終了フラグ
	virtual void StopEffect(void) = 0;

protected:
	float timer_;
	bool isFinished_;
};


