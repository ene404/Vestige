#pragma once
#include <map>
#include <vector>
#include <unordered_set>
#include "../EnemyBoss.h"

class EnemyBoss;

// �U����ԏ����Ǘ�
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

	// �U������
	void ExcuteAttack(EnemyBoss::ATK_STATE pattern);

	// �v���C�C���[�ɂ�������
	bool IsHit();

	// �U�������Z�b�g
	void ClearHitRecord();

	// �U���\��(�N�[���_�E����)
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
	int maxCombo_;		//�ő�R���{��(�R��)
	float comboResetTimer_;
	const float comboRestTime_;

	// �G�t�F�N�g
	int effectHitPlayId_;
	int effectHitResId_;

	void InitEffect(void);
};

