#pragma

#include "ActorBase.h"

class WeaponBase;

class CharactorBase : public ActorBase
{
public:

	static constexpr int WEAPON_TOP_INDEX = 3;									// 武器の頂点インデックス（上方向）
	static constexpr int WEAPON_DOWN_INDEX = 6;									// 武器の頂点インデックス（下方向）
	static constexpr VECTOR INIT_WEAPON_TOP_POS = { 0.0f, 0.0f, -100.0f };		// 武器の初期先端位置（Zマイナス方向）
	static constexpr float FORWARD_OFFSET = 20.0f;								// 武器位置の前方向オフセット
	static constexpr float UP_OFFSET = 0.0f;									// 武器位置の上方向オフセット
	static constexpr float RIGHT_OFFSET = 5.0f;									// 武器位置の右方向オフセット
	static constexpr VECTOR WEAPON_UPVEC = { 0.0f,1.0f,0.0f };					// 武器の上方向ベクトル
	static constexpr float WEAPON_CAPSULE_FORWARD_OFFSET = 20.0f;				// 武器の当たり判定カプセルの前方向へのオフセット
	static constexpr float WEAPON_CAPSULE_UP_OFFSET = 0.0f;						// 武器の当たり判定カプセルの上方向へのオフセット
	static constexpr float WEAPON_CAPSULE_RIGHT_OFFSET = 5.0f;					// 武器の当たり判定カプセルの右方向へのオフセット
	static constexpr VECTOR WEAPON_CAPSULE_UPVEC = { 0.0f,1.0f,0.0f };			// 武器の当たり判定カプセルの上方向ベクトル
	// コンストラクタ
	CharactorBase(void);

	//　デストラクタ
	virtual ~CharactorBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

protected:

	std::shared_ptr<WeaponBase> weapon_;

	// 手首のフレーム
	int wristIndex_;
	VECTOR wristPos_;

	// 手のフレーム
	int handIndex_;
	VECTOR handPos_;

	// 武器のトップ
	int weaponTopIndex_;
	VECTOR weaponTopPos_;

	// 武器の下
	int weaponDownIndex_;
	VECTOR weaponDownPos_;

	// 手のカプセル
	VECTOR capsuleDir_;
	VECTOR capsuleCenter_;

	VECTOR WeaponTopPos(VECTOR pos);
	VECTOR WeaponDownPos(VECTOR pos);

	// 武器の更新
	void UpdateWeapon(void);

	// 当たり判定用武器カプセルの更新
	void UpdateWeaponCapsule();

private:

};