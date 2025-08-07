#pragma once
#include <string>
#include <unordered_map>
#include "../EnemyBoss.h"

class AnimationController;
class EnemyBossAnimation
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE,
		IDLE,
		WALK_F,
		WALK_B,
		WALK_R,
		WALK_L,
		OBSERVE,
		RUN,
		STEP,
		STUN_DOWN,
		STUN_IDLE,
		STUN_RECOVER,
		COMBO,
		THRUST,
		ROAR,
		EXPLOSION,
		SWEEP,
		FOLLOW_SLASH,
		RECOVER,
		SETUP,
		DEATH
	};

	EnemyBossAnimation(int modelId);
	~EnemyBossAnimation();

	void AddAnimation(ANIM_TYPE type, const std::string& path, float speed);
	void Play(ANIM_TYPE anim, bool loop = true, float blendRate = 1.0f, float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool force = false);
	bool IsEnd() const;

	void Update();

	ANIM_TYPE GetCurrentAnim() const;

private:
	int modelId_;										//
	ANIM_TYPE currentAnim_;								//現在のアニメーション
	std::unique_ptr<AnimationController> controller_;
	std::unordered_map<ANIM_TYPE, int> animIdMap_;		// int型の再生タイプ
};

