#pragma once
#include <map>
#include <vector>
#include <unordered_set>
#include "../EnemyBoss.h"

class EnemyBoss;

// 攻撃状態処理管理
class EnemyBossCombatSystem
{

public:
	
	static constexpr float COMBO_RESET_TIME = 2.0f;
	static constexpr float ATTACK_COOLDOWN = 1.0f;
	static constexpr float ATTACK_DURATION = 0.5f;
	static constexpr int MAX_COMBO = 2;

	static constexpr float EFFECT_SIZE = 25.0f;

	static constexpr float SPHERE_RADIUS = 128.0f;

	EnemyBossCombatSystem(EnemyBoss& boss);
	~EnemyBossCombatSystem();
	void Update(float deltaTime);

	// 攻撃処理
	void ExcuteAttack(EnemyBoss::ATK_STATE pattern);

	// プレイイヤーにあったか
	bool IsHit();

	// 攻撃をリセット
	void ClearHitRecord();

	// 攻撃可能か(クールダウン等)
	bool CanAttack() const;

	std::unordered_set<int> hitPlayers_;

private:
	EnemyBoss& boss_;

	float attackCooldown_;
	float currentCooldown_;

	bool isAttacking_;
	float attackDuration_;
	float attackTimer_;

	int comboCount_;
	int maxCombo_;		//最大コンボ数(３回)
	float comboResetTimer_;
	const float comboRestTime_;

	// エフェクト
	int effectHitPlayId_;
	int effectHitResId_;

	void InitEffect(void);
};

