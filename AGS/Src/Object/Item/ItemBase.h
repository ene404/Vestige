#pragma once
#include "../Common/Transform.h"
class ResourceManager;
class SceneManager;

class ItemBase
{
public:

	// �R���X�g���N�^
	ItemBase(void);

	// �f�X�g���N�^
	virtual ~ItemBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetTransform(void) const;

protected:

	// �V���O���g���Q��
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// ���f������̊�{���
	Transform transform_;

	int itemImg_;
	int count_;

};

