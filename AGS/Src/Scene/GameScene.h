#pragma once
#include "SceneBase.h"
class Stage;
class SkyDome;
class Player;
class PlayerDummy;
class EnemyBoss;
class EnemyBossDummy;
class Mist;
class PixelMaterial;
class PixelRenderer;


class GameScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawUI(void) override;
	void UpdateCamera(VECTOR& pos, VECTOR& targetPos, VECTOR& cameraUp)override;


private:

	// ��
	int gameBgm_;
	int bgmVol_;
	bool isBgm_;
	int deadSe_;
	int helpSe_;

	// �X�e�[�W
	std::unique_ptr<Stage> stage_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	std::shared_ptr<Mist> mist_;

	// �v���C���[
	std::shared_ptr<Player> player_;

	std::shared_ptr<PlayerDummy> playerDummy_;

	std::shared_ptr<EnemyBoss> enemyBoss_;

	std::shared_ptr<EnemyBossDummy> enemyBossDummy_;

	int deathFadeAlpha_;
	int youDiedTimer_;

	int enemySlainFadeAlpha_;
	int enemySlainTimer_;
	bool enemySlain_;

	// �|�X�g�G�t�F�N�g�p�X�N���[��
	int postEffectScreen_;

	// �|�X�g�G�t�F�N�g�p(�v���C���[�̎��E)
	std::unique_ptr<PixelMaterial> lowHpVisionMaterial_;
	std::unique_ptr<PixelRenderer> lowHpVisionRenderer_;

	// �|�X�g�G�t�F�N�g�p(���C�g�j���O)
	std::unique_ptr<PixelMaterial> lightningMaterial_;
	std::unique_ptr<PixelRenderer> lightningRenderer_;
	float lightningTime_;
	float lightningPower_;

	bool isHelp_;
	int helpBackImage_;
	bool wasHelp_;
	void DrawHelp(void);
};
