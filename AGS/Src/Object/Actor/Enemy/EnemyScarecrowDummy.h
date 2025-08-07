#pragma once
#include "../ActorBase.h"

class EnemyScarecrow;

class EnemyScarecrowDummy :
    public ActorBase
{

public:
	// �T�C�Y
	static constexpr float SIZE = 1.8f;

	// �������W
	static constexpr VECTOR INIT_POS = { 0.0f, 70.0f, 660.0f };

	// �������[�J����]
	static constexpr float INIT_QUAROT_LOCAL_X = 0.0f;
	static constexpr float INIT_QUAROT_LOCAL_Y = 90.0f;
	static constexpr float INIT_QUAROT_LOCAL_Z = 0.0f;

	// ���W����
	static constexpr float POS_Y_ADJUSTMENT = 200.0f;

	EnemyScarecrowDummy(EnemyScarecrow& enemyScarecrow);

	~EnemyScarecrowDummy(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

private:
	EnemyScarecrow& enemyScarecrow_;
};

