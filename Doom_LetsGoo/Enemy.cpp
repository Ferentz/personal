#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include <string>
#include <filesystem>
#include <iostream>

#include "DoomCamera.h"
#include "Navigator.h"


namespace fs = std::filesystem;

Enemy::Enemy(Vector2f pos, float height, std::string folder)
	:Thing{ pos, height , utils::ThingsEnum::Zombieman }
{
	m_pNavigator = new Navigator(m_location.point3D);
	std::string path = "POSS";
	for (const auto& entry : fs::directory_iterator(folder))
	{
		std::string path{ entry.path().string() };
		std::string name{ entry.path().filename().string() };
		std::string suffic{ entry.path().filename().string().erase(name.size() - 4, name.size() - 1) };
		suffic.erase(0, 4);
		m_MonsterTextureSuffixes.push_back(suffic);
		m_pMonsterTexture.emplace_back(Texture(path));
	}
}

Enemy::~Enemy()
{
	delete m_pNavigator;
	m_pNavigator = nullptr;
}

void Enemy::UpdateThing(const DoomCamera& eyeObject, float elapsSec)
{
	if (m_pNavigator->m_hasLineOfSight &&
		!m_pNavigator -> m_IsShooting &&
		m_pNavigator->m_CanShootAgain)
	{
		m_pNavigator->m_IsShooting = true;
		m_timer = 0;
		m_TexIndex = 0;
	}
	m_timer += elapsSec;
	if (m_timer >= 0.5)
	{
		if (m_pNavigator->m_IsShooting)
		{
			
			if (m_TexIndex >= 1)
			{
				m_TexIndex = 0;
				m_pNavigator->m_IsShooting = false;
				m_pNavigator->m_CanShootAgain = false;
			}
			m_TexIndex++;
			m_timer = 0;
		}
		else {
			m_pNavigator->m_CanShootAgain = true;
			m_TexIndex++;
			if (m_TexIndex >= 4)
			{
				m_TexIndex = 0;
			}
			m_timer = 0;
		}
	}
	Thing::UpdateThing(eyeObject, elapsSec);
	
	//m_location.point2D = eyeObject.PerspCalc(m_location, m_location.point3D);
}

void Enemy::DrawThing(Texture* tex)
{
	float scale{ 0.001 };
	float width{ m_pMonsterTexture[m_TexIndex].GetWidth() / (m_location.Dist3D * scale) };
	float height{ m_Height / (m_location.Dist3D * scale) };
	Rectf dest{ m_location.point2D.x - width / 2, m_location.point2D.y, width, height };
	//utils::SetColor(1, 1, 1, 1);
	//utils::DrawRect(dest);
	int index{ m_TexIndex * 5 };
	if (m_pNavigator->m_IsShooting)
	{
		index += (4 * 5);
	}
	m_pMonsterTexture[index].Draw(dest);
}
