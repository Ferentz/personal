#pragma once
#include "utils.h"
#include "Things.h"


class PLayer;
class Texture;
class HealthManager;


class Enemy final: public Thing
{
public:
	Enemy(Vector2f& pos, float height, std::string& folder);
	virtual ~Enemy() override;
	virtual void UpdateThing(const PLayer& eyeObject, float elapsSec) override;
	virtual void DrawThing(Texture* tex) override;

	static void AssignPlayer(HealthManager& playerHealth);
	static HealthManager* pPlayerHealth;
	//void ShootPlayer(DoomCamera* eyeObject);

	//void ColideWLevel(const Level* pLevel);
private:

	int m_TexIndex{ 0 };
	float m_timer{};
	
	std::vector <Texture> m_pMonsterTexture{};
	std::vector <std::string> m_MonsterTextureSuffixes{};
	Vector2f m_direction{};
};

