#include "../EnemyBoss.h"
#include "EnemyBossStateMachine.h"

// static map �̎��̂��`
const std::map<EnemyBoss::STATE, const char*> EnemyStateMachine::stateNames_ = 
{
	{ EnemyBoss::STATE::IDLE,     "IDLE"     },
	{ EnemyBoss::STATE::OBSERVE,  "OBSERVE"  },
	{ EnemyBoss::STATE::MOVE,	  "MOVE"	 },
	{ EnemyBoss::STATE::CHASE,    "CHASE"    },
	{ EnemyBoss::STATE::ATTACK,   "ATTACK"   },
	{ EnemyBoss::STATE::STEP,	  "STEP"	 },
	{ EnemyBoss::STATE::STUN,	  "STUN"	 },
	{ EnemyBoss::STATE::RECOVER,  "RECOVER"	 },
	{ EnemyBoss::STATE::DEAD,     "DEAD"     },
};

EnemyStateMachine::EnemyStateMachine(EnemyBoss& boss)
	:
	boss_(boss),
	curStateId_(EnemyBoss::STATE::IDLE)
{

}

EnemyStateMachine::~EnemyStateMachine()
{
}

void EnemyStateMachine::Update(EnemyBoss& boss)
{
	if (curState_)
	{
		curState_->Update(boss);
	}
}

void EnemyStateMachine::Add(EnemyBoss::STATE stateId, std::shared_ptr<IEnemyState> stateInstance)
{
	states_[stateId] = stateInstance;
}

void EnemyStateMachine::ChangeState(EnemyBoss::STATE newState)
{
	if (states_.count(newState) == 0)
	{
		return;
	}
	if (curState_)
	{
		curState_->Exit(boss_);	// ���݂̏�Ԃ��I��
	}

	curState_ = states_[newState];
	curStateId_ = newState;

	if (curState_)
	{
		curState_->Enter(boss_);	// �V������Ԃ��J�n
	}
}

EnemyBoss::STATE EnemyStateMachine::GetCurrentStateId() const
{
	return curStateId_;
}

void EnemyStateMachine::DrawDebugInfo(const VECTOR& screenPos)
{
	const char* stateName = "UNKNOWN";

	auto it = stateNames_.find(curStateId_);
	if (it != stateNames_.end()) 
	{
		stateName = it->second;
	}

	DrawFormatString((int)screenPos.x, (int)screenPos.y, GetColor(255, 255, 255), "State: %s", stateName);
}
