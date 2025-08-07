#pragma once
#include "../Common/Transform.h"
class ResourceManager;
class SceneManager;

class ItemBase
{
public:

	// コンストラクタ
	ItemBase(void);

	// デストラクタ
	virtual ~ItemBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetTransform(void) const;

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform transform_;

	int itemImg_;
	int count_;

};

