#pragma once
#include "utils.h"
#include "Shootable.h"


class Player;
class Texture;
class HealthManager;
class Level;


class Enemy final: public Shootable
{
public:
	Enemy(Vector2f& pos, float height, std::string& folder);
	Enemy(const Enemy& toCopy);
	Enemy(Enemy&& toMove) noexcept;
	Enemy& operator=(const Enemy& rhs) = delete;
	Enemy& operator=(Enemy&& rhs) noexcept;
	virtual ~Enemy() noexcept override ;
	virtual void UpdateThing(const Player& player, float elapsSec) override;
	void UpdateEnemy(float elapsSec);
	bool Navigate(Level* pLevel, const Player& player, const float elapsSec);
	virtual void DrawThing(Texture* tex) override;
	
	static void AssignPlayer(HealthManager& playerHealth);
	static HealthManager* pPlayerHealth;

	Navigator* m_pNavigator;

private:
	void LoadTextures();
	int m_TexIndex;
	float m_timer;
	bool m_IsShooting{ false };
	bool m_CanShootAgain{ true };
	bool m_SpawnWeapon{ false };
	
	int m_TextureColums{ 5 };
	int m_DeathRow{};
	int m_HurtRow{};
	int m_AttackRow{};
	std::vector <Texture> m_pMonsterTexture{};
	std::vector <std::string> m_MonsterTexturePaths{};
	Vector2f m_direction;
};


