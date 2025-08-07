#pragma once
#include "../Manager/Camera.h"
class ResourceManager;

class SceneBase
{

public:

	// �R���X�g���N�^
	SceneBase(void);

	// �f�X�g���N�^
	virtual ~SceneBase(void) = 0;

	// ����������
	virtual void Init(void) = 0;

	// �X�V�X�e�b�v
	virtual void Update(void) = 0;

	// �`�揈��
	virtual void Draw(void) = 0;
	virtual void DrawUI(void) = 0;

	// �J�����I�u�W�F�N�g�������ɂƂ�A
	//�J�����̍X�V���Ϗ�����
	virtual void UpdateCamera(VECTOR& pos,VECTOR& targetPos,VECTOR& cameraUp) = 0;

protected:

	// ���\�[�X�Ǘ�
	ResourceManager& resMng_;

};
