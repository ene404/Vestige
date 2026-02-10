#include "../../Utility/AsoUtility.h"
#include "Weapon/WeaponBase.h"
#include "CharactorBase.h"

CharactorBase::CharactorBase(void)
	:
	weapon_(nullptr),
	wristIndex_(0),
	wristPos_{},
	handIndex_(),
	handPos_{},
	weaponTopIndex_(),
	weaponTopPos_{},
	weaponDownIndex_(),
	weaponDownPos_{},
	capsuleDir_{},
	capsuleCenter_{}
{
}

CharactorBase::~CharactorBase(void)
{
}

VECTOR CharactorBase::WeaponTopPos(VECTOR pos)
{
	return VAdd(weaponTopPos_, pos);
}

VECTOR CharactorBase::WeaponDownPos(VECTOR pos)
{
	return VAdd(weaponDownPos_, pos);
}

void CharactorBase::UpdateWeapon(void)
{

	// 手首 → 手 の向きベクトル
	wristPos_ = MV1GetFramePosition(transform_.modelId, wristIndex_);
	handPos_ = MV1GetFramePosition(transform_.modelId, handIndex_);

	VECTOR dir = VSub(handPos_, wristPos_); // 向きベクトル
	dir = VNorm(dir); // 正規化して単位ベクトルにする

	// 向きから角度を算出
	float yaw = atan2f(dir.x, dir.z);
	float pitch = atan2f(-dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z));
	float roll = 0.0f;

	// 剣の回転を設定
	MV1SetRotationXYZ(weapon_->GetModelHandle(), VGet(pitch, yaw, roll));

	// 位置微調整
	float forwardOffset = FORWARD_OFFSET;
	float upOffset = UP_OFFSET;
	float rightOffset = RIGHT_OFFSET;

	// 右ベクトルと上ベクトルを簡易的に求める（完全な回転行列がない前提）
	VECTOR upVec = AsoUtility::DIR_U; // 上方向
	VECTOR rightVec = VCross(upVec, dir);  // 手の向きと上から右方向を作る
	rightVec = VNorm(rightVec); // 正規化

	// 上方向再調整（手のひねりは考慮しない）
	upVec = VCross(dir, rightVec);
	upVec = VNorm(upVec);

	// オフセット合成
	VECTOR offsetPos = handPos_;
	offsetPos = VAdd(offsetPos, VScale(dir, forwardOffset));   // 前方
	offsetPos = VAdd(offsetPos, VScale(upVec, upOffset));      // 上
	offsetPos = VAdd(offsetPos, VScale(rightVec, rightOffset));// 右

	// 剣の位置を設定
	MV1SetPosition(weapon_->GetModelHandle(), offsetPos);
}

void CharactorBase::UpdateWeaponCapsule()
{
	// 手首から手の向きベクトルを求める
	weaponTopPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponTopIndex_);
	weaponDownPos_ = MV1GetFramePosition(weapon_->GetModelHandle(), weaponDownIndex_);

	// 武器方向ベクトル
	VECTOR dir = VSub(weaponTopPos_, weaponDownPos_);

	// 正規化して単位ベクトルにする
	dir = VNorm(dir);

	// 位置微調整
	float forwardOffset = WEAPON_CAPSULE_FORWARD_OFFSET;
	float upOffset = WEAPON_CAPSULE_UP_OFFSET;
	float rightOffset = WEAPON_CAPSULE_RIGHT_OFFSET;

	// 右ベクトルと上ベクトルを簡易的に求める

	// 上方向
	VECTOR upVec = WEAPON_CAPSULE_UPVEC;

	// 手の向きと上から右方向を作る
	VECTOR rightVec = VCross(upVec, dir);

	// 正規化
	rightVec = VNorm(rightVec);

	// 上方向再調整（手のひねりは考慮しない）
	upVec = VCross(dir, rightVec);
	upVec = VNorm(upVec);

	// オフセット合成
	VECTOR offsetPos = handPos_;
	offsetPos = VAdd(offsetPos, VScale(dir, forwardOffset));   // 前方
	offsetPos = VAdd(offsetPos, VScale(upVec, upOffset));      // 上
	offsetPos = VAdd(offsetPos, VScale(rightVec, rightOffset));// 右

	// 剣の位置を設定
	MV1SetPosition(weapon_->GetModelHandle(), offsetPos);
}
