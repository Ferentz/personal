#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include <string>
#include <filesystem>
#include <iostream>

#include "PLayer.h"
#include "Navigator.h"
#include "Interactor.h"


namespace fs = std::filesystem;
HealthManager* Enemy::pPlayerHealth{};

Enemy::Enemy(Vector2f& pos, float height, std::string& folder)
	:Thing{ pos, height , utils::ThingsEnum::Zombieman, folder },
	m_TexIndex{},
	m_timer{},
	m_direction{}
{
	m_pNavigator = new Navigator(m_location.point3D);
	m_pHealthManager = new HealthManager(20);
	//m_pInteractor = new Interactor();
	//std::string path = "POSS";
	for (const auto& entry : fs::directory_iterator(folder))
	{
		std::string path{ entry.path().string() };
		/*std::string name{ entry.path().filename().string() };
		std::string suffic{ entry.path().filename().string().erase(name.size() - 4, name.size() - 1) };
		suffic.erase(0, 4);*/
		m_pMonsterTexture.emplace_back(Texture(path));
	}
	LoadTextures();
}

Enemy::Enemy(const Enemy& toCopy)
	:Thing{ Thing(toCopy) },
	m_TexIndex{ toCopy.m_TexIndex },
	m_timer{ toCopy.m_timer },
	m_direction{ toCopy.m_direction }
{
	m_MonsterTexturePaths.reserve(toCopy.m_MonsterTexturePaths.size());
	for (const std::string& suffix : toCopy.m_MonsterTexturePaths)
	{
		m_MonsterTexturePaths.push_back(suffix);
	}

	LoadTextures();
}

// try this
Enemy::Enemy(Enemy&& toMove) noexcept
	:Thing( Thing( toMove ) ),
	m_TexIndex{ std::move( toMove.m_TexIndex ) },
	m_timer{ std::move( toMove.m_timer )},
	m_direction{ std::move( toMove.m_direction ) }
{
	m_pMonsterTexture = std::move(toMove.m_pMonsterTexture);
	m_MonsterTexturePaths = std::move(toMove.m_MonsterTexturePaths);
}

Enemy::~Enemy()
{
	delete m_pNavigator;
	delete m_pHealthManager;
	m_pNavigator = nullptr;
	m_pHealthManager = nullptr;
}

Enemy& Enemy::operator=(const Enemy& rhs)
{
	if (&rhs != this) 
	{
		m_TexIndex = rhs.m_TexIndex;
		m_timer = rhs.m_timer;
		m_direction = rhs.m_direction;

		m_pMonsterTexture.clear();
		m_MonsterTexturePaths.clear();
		m_MonsterTexturePaths.reserve(rhs.m_MonsterTexturePaths.size());
		for (const std::string& path : rhs.m_MonsterTexturePaths)
		{
			m_MonsterTexturePaths.push_back(path);
		}
		LoadTextures();
	}
	return *this; 
}
Enemy& Enemy::operator=(Enemy&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_TexIndex = rhs.m_TexIndex;
		m_timer = rhs.m_timer;
		m_direction = rhs.m_direction;
		m_pMonsterTexture = std::move(rhs.m_pMonsterTexture);
		m_MonsterTexturePaths = std::move(rhs.m_MonsterTexturePaths);
	}
	return *this;
}



void Enemy::UpdateThing(const PLayer& eyeObject, float elapsSec)
{
	if (m_pNavigator->m_Alive)
	{
		if (m_pNavigator->m_hasLineOfSight &&
			!m_pNavigator->m_IsShooting &&
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
					if (m_pNavigator->m_hasLineOfSight)
					{
						pPlayerHealth->Harm(5);
					}
					//ShootPlayer(&eyeObject);
					m_TexIndex = 0;
					m_pNavigator->m_IsShooting = false;
					m_pNavigator->m_CanShootAgain = false;
				}
				m_TexIndex++;
				m_timer = 0;
			}
			else if (m_pNavigator->m_Dying)
			{
				m_pNavigator->m_CanShootAgain = false;
				m_TexIndex++;
				if (m_TexIndex >= 5)
				{
					m_TexIndex = 0;
				}
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
		if (m_pHealthManager->GetHeath() <= 0 && !m_pNavigator->m_Dying)
		{
			m_timer = 0;
			m_pNavigator->m_Dying = true;
		}
		if (m_pNavigator->m_Dying)
		{
			int index = m_TexIndex + (7 * 5);
			if (index == (7 * 5) + 4)
			{
				m_pNavigator->m_Alive = false;
				
				m_pNavigator->m_Dying = false;

			}
		}
		
	}

	Thing::UpdateThing(eyeObject, elapsSec);
	
	//m_location.point2D = eyeObject.PerspCalc(m_location, m_location.point3D);
}

void Enemy::DrawThing(Texture* tex)
{
	/*float scale{ 0.001 };
	float width{ m_pMonsterTexture[m_TexIndex].GetWidth() / (m_location.Dist3D * scale) };
	float height{ m_Height / (m_location.Dist3D * scale) };
	Rectf dest{ m_location.point2D.x - width / 2, m_location.point2D.y, width, height };*/
	//utils::SetColor(1, 1, 1, 1);
	//utils::DrawRect(dest);
	int index{ m_TexIndex * 5 };
	if (m_pNavigator->m_IsShooting)
	{
		index += (4 * 5);
	}
	if (m_pNavigator->m_Dying)
	{
		index = m_TexIndex + (7 * 5);
	}
	if (!m_pNavigator->m_Alive)
	{
		index = (7 * 5) + 4;
	}
	float scale{ 0.001 };
	float width{ m_pMonsterTexture[index].GetWidth() / (m_location.Dist3D * scale) };
	float height{ m_pMonsterTexture[index].GetHeight() / (m_location.Dist3D * scale) };
	Rectf dest{ m_location.point2D.x - width / 2, m_location.point2D.y, width, height };
	m_pMonsterTexture[index].Draw(dest);
}

void Enemy::AssignPlayer(HealthManager& playerHealth)
{
	pPlayerHealth = &playerHealth;
}

void Enemy::LoadTextures()
{
	m_pMonsterTexture.reserve(m_MonsterTexturePaths.size());
	for (const std::string& path : m_MonsterTexturePaths)
	{
		m_pMonsterTexture.emplace_back(Texture(path));
	}
}

//void Enemy::ShootPlayer(DoomCamera* eyeObject)
//{
//	eyeObject-> ChangeHealth(-1);
//}