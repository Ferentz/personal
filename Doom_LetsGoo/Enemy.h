#pragma once
#include "Shootable.h"

class Player;
class HealthManager;
class Texture;
class Level;
class Navigator;

class Enemy final: public Shootable
{
public:
	explicit Enemy(Vector2f& pos, float height, std::string& folder);
	virtual ~Enemy() noexcept override;

	//rule of 5
	Enemy(const Enemy& toCopy);
	Enemy(Enemy&& toMove) noexcept;
	Enemy& operator=(Enemy&& rhs) noexcept;
	Enemy& operator=(const Enemy& rhs) = delete;
	
	//thing expansion
	virtual void UpdateThing(const Player& player, float elapsSec) override;
	virtual void DrawThing(Texture* tex) override;

	//enemy behaviour
	void UpdateEnemy(float elapsSec);
	bool Navigate(Level* pLevel, const Player& player, const float elapsSec);
	Navigator* GetpNavigator() const;
	
	//link to player health
	static void AssignPlayerHealth(HealthManager& playerHealth);
	static HealthManager* s_pPlayerHealth;

private:
	// Rule of 5 helper — implemented with the Rule of 5 in the .cpp
	void MoveEnemyMembers(Enemy&& rhs);

	//behavior
	Navigator* m_pNavigator;
	const float m_IntervalTime{ 0.5f };
	float m_timer{};
	bool m_IsShooting{ false };
	bool m_CanShootAgain{ true };
	bool m_SpawnWeapon{ false };
	
	//texture related
	const int m_TextureColums{ 5 };
	int m_TexIndex{};
	std::vector <std::string> m_MonsterTexturePaths{};

	void LoadTextures(); // loads the following
	int m_DeathRow{};
	int m_HurtRow{};
	int m_AttackRow{};
	std::vector <Texture> m_pMonsterTexture{};
};


