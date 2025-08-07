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

	// 攻撃パターンの追加
	void Add(EnemyBoss::ATK_STATE type, std::shared_ptr<IEnemyAttackState> state);

	// サブステート(攻撃パターン)を変更
	void Change(EnemyBoss& boss, EnemyBoss::ATK_STATE type);
	void Update(EnemyBoss& boss);
	
	// 現在の攻撃パターンの取得
	EnemyBoss::ATK_STATE GetCurrentStateType() const;
	
	// 攻撃状態をリセット
	void Reset(EnemyBoss& boss);	
	
	// 現在の攻撃が完了したか？
	bool IsFinished() const;

	// エフェクトの停止
	void StopEffect(void);

	void DrawDebugInfo(const VECTOR& screenPos);

private:
	std::map<EnemyBoss::ATK_STATE, std::shared_ptr<IEnemyAttackState>> states_;
	std::shared_ptr<IEnemyAttackState> curState_;
	EnemyBoss::ATK_STATE curStateId_;

	static const std::map<EnemyBoss::ATK_STATE, const char*> attackStateNames_;
};

