#pragma once
#include "utils.h"
#include "Shootable.h"


class PLayer;
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
	virtual void UpdateThing(const PLayer& player, float elapsSec) override;
	void UpdateTexture(float elapsSec);
	bool Navigate(Level* pLevel, const PLayer& player, const float elapsSec);
	virtual void DrawThing(Texture* tex) override;

	static void AssignPlayer(HealthManager& playerHealth);
	static HealthManager* pPlayerHealth;
	//void ShootPlayer(DoomCamera* eyeObject);

	//void ColideWLevel(const Level* pLevel);
	Navigator* m_pNavigator;
private:
	void LoadTextures();
	int m_TexIndex;
	float m_timer;

	

	std::vector <Texture> m_pMonsterTexture{};
	std::vector <std::string> m_MonsterTexturePaths{};
	Vector2f m_direction;
};


