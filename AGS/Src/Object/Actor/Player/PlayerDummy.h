#pragma once
#include "../ActorBase.h"
class ResourceManager;
class SceneManager;
class Player;

class PlayerDummy :public ActorBase
{
public:
	// ÉTÉCÉY
	static constexpr float SIZE = 1.5f;

	PlayerDummy(Player& playerDummy);

	~PlayerDummy(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

private:
	Player& player_;

};

