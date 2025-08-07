#pragma once
class Collider
{

public :

	// �Փˎ��
	enum class TYPE
	{
		STAGE,
	};

	// �R���X�g���N�^
	Collider(TYPE type, int modelId);

	// �f�X�g���N�^
	~Collider(void);

	bool IsCollidiingWith(const Collider& other) const;
	VECTOR GetPushVector(const Collider& other) const;	// ���o�x�N�g����Ԃ�
	void SetPosition(const VECTOR& pos);				// ���f���Ɠ���
	VECTOR GetPosition(void) const;

	// �Փˎ��
	TYPE type_;

	// ���f���̃n���h��ID
	int modelId_;

	// ���S���W
	VECTOR center_;

	// ��/����/���s���̔���
	VECTOR halfSize_;
};
