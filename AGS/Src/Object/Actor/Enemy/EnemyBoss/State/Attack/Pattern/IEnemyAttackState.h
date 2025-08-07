#pragma once
class EnemyBoss;

class IEnemyAttackState
{

public:

	virtual ~IEnemyAttackState(void) = default;
	virtual void Enter(EnemyBoss& boss) = 0;			//	�U���J�n���̏�����
	virtual void Update(EnemyBoss& boss) = 0;			// ���t���[��
	virtual void Exit(EnemyBoss& boss) = 0;
	virtual bool IsFinished(void) const = 0;// �U���I���t���O
	virtual void StopEffect(void) = 0;

protected:
	float timer_;
	bool isFinished_;
};


