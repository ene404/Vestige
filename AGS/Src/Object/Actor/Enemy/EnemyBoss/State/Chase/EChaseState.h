#pragma once
#include "../IEnemyState.h"
class EChaseState 
	: public IEnemyState
{

public:

	// バックステップを開始する距離
	static constexpr float ATTACK_RANGE = 75.0f;

	// 攻撃を開始する距離 (この距離より近いと攻撃候補を探す)
	static constexpr float ATTACK_INITIATE_RANGE = 800.0f;
	
	// 追跡する速度
	static constexpr float CHASE_SPEED = 5.5f;

	EChaseState(void);
	~EChaseState(void);

	void Enter(EnemyBoss& boss)override;
	void Update(EnemyBoss& boss)override;
	void Exit(EnemyBoss& boss)override;

public:
	float dist_;
};
