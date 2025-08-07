#pragma once
#include "../../../../Object/Common/Transform.h"
class ResourceManager;
class SceneManager;

class EnemyBoss;

class EnemyBossDummy
{
public:
	// �T�C�Y
	static constexpr float SIZE = 1.5f;

	EnemyBossDummy(EnemyBoss& enemyBoss);

	~EnemyBossDummy(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	const Transform& GetTransform(void) const;

private:
	EnemyBoss& enemyBoss_;

	// �V���O���g���Q��
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// ���f������̊�{���
	Transform transform_;

};

