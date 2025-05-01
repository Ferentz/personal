#pragma once
#include "utils.h"
#include "Things.h"

class DoomCamera;
class Texture;
class Enemy : public Thing
{
public:
	Enemy(Vector2f pos, float height, std::string folder );
	virtual ~Enemy() override;
	virtual void UpdateThing(const DoomCamera& eyeObject, float elapsSec) override;
	virtual void DrawThing(Texture* tex) override;

	//void ColideWLevel(const Level* pLevel);
private:

	int m_TexIndex{ 0 };
	float m_timer{};
	std::vector <Texture> m_pMonsterTexture{};
	std::vector <std::string> m_MonsterTextureSuffixes{};
	Vector2f m_direction{};
};

