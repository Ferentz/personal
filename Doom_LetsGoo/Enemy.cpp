#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include <string>
#include <filesystem>
#include <iostream>

#include "PLayer.h"
#include "Navigator.h"
#include "Interactor.h"
#include "Level.h"


namespace fs = std::filesystem;
HealthManager* Enemy::pPlayerHealth{};

Enemy::Enemy(Vector2f& pos, float height, std::string& folder)
	:Shootable{ pos, height , folder, 20 },
	m_TexIndex{},
	m_timer{},
	m_direction{},
	m_pNavigator{ new Navigator(m_location.point3D) }
{
	
	//m_pHealthManager = new HealthManager(20);
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
	m_SubClass = ThingSubclass::enemy;
}

Enemy::Enemy(const Enemy& toCopy)
	:Shootable{ Shootable(toCopy) },
	m_TexIndex{ toCopy.m_TexIndex },
	m_timer{ toCopy.m_timer },
	m_direction{ toCopy.m_direction },
	m_pNavigator{ new Navigator(*toCopy.m_pNavigator) }
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
	:Shootable(Shootable( toMove ) ),
	m_TexIndex{ std::move( toMove.m_TexIndex ) },
	m_timer{ std::move( toMove.m_timer )},
	m_direction{ std::move( toMove.m_direction ) },
	m_pNavigator{ std::move(toMove.m_pNavigator) }
{
	m_pMonsterTexture = std::move(toMove.m_pMonsterTexture);
	m_MonsterTexturePaths = std::move(toMove.m_MonsterTexturePaths);
}

Enemy::~Enemy()
{
	delete m_pNavigator;
	//delete m_pHealthManager;
	m_pNavigator = nullptr;
	//m_pHealthManager = nullptr;
}

//Enemy& Enemy::operator=(const Enemy& rhs)
//{
//	if (&rhs != this) 
//	{
//		Shootable::operator=(rhs);
//		m_TexIndex = rhs.m_TexIndex;
//		m_timer = rhs.m_timer;
//		m_direction = rhs.m_direction;
//
//		m_pMonsterTexture.clear();
//		m_MonsterTexturePaths.clear();
//		m_MonsterTexturePaths.reserve(rhs.m_MonsterTexturePaths.size());
//		for (const std::string& path : rhs.m_MonsterTexturePaths)
//		{
//			m_MonsterTexturePaths.push_back(path);
//		}
//		LoadTextures();
//		*m_pNavigator = *rhs.m_pNavigator;
//	}
//	return *this; 
//}

Enemy& Enemy::operator=(Enemy&& rhs) noexcept
{
	if (&rhs != this)
	{
		Shootable::operator=(rhs);
		m_TexIndex = rhs.m_TexIndex;
		m_timer = rhs.m_timer;
		m_direction = rhs.m_direction;
		m_pMonsterTexture = std::move(rhs.m_pMonsterTexture);
		m_MonsterTexturePaths = std::move(rhs.m_MonsterTexturePaths);
		m_pNavigator = rhs.m_pNavigator;
		rhs.m_pNavigator = nullptr;
	}
	return *this;
}

void Enemy::UpdateThing(const Player& player, float elapsSec)
{
	UpdateEnemy(elapsSec);
	Thing::UpdateThing(player, elapsSec);
	
	//m_location.point2D = eyeObject.PerspCalc(m_location, m_location.point3D);
}

void Enemy::UpdateEnemy(float elapsSec)
{

	if (m_State != ShootableStates::dead)
	{
		// add the timer
		m_timer += elapsSec;
		if (m_timer >= 0.5) // the time interval to swithc textures/perform actions
		{
			switch (m_State)
			{
			case ShootableStates::alive:
			{
				// if he can shoot ->shoot
				if (m_pNavigator->m_hasLineOfSight &&
					!m_IsShooting &&
					m_CanShootAgain)
				{
					m_IsShooting = true;
					//prime animation to shoot
					m_timer = 0;
					m_TexIndex = 0;
				}

				// if he's shooting
				if (m_IsShooting)
				{
					// if it's the end of shooting animation
					if (m_TexIndex >= 1)
					{
						// if still has line of sight over player, harm the player 
						if (m_pNavigator->m_hasLineOfSight)
						{
							pPlayerHealth->Harm(5);
						}
						//reset shooting + animation
						m_TexIndex = 0;
						m_IsShooting = false;
						m_CanShootAgain = false;
					}
					//continue animation
					m_TexIndex++;
					m_timer = 0;
				}

				else {
					m_CanShootAgain = true;
					m_TexIndex++;
					if (m_TexIndex >= 4)
					{
						m_TexIndex = 0;
					}
					m_timer = 0;
				}
				break;
			}
			case ShootableStates::harmed:
			{
				if (m_pHealthManager.GetHeath() <= 0)
				{
					m_timer = 0;
					m_pNavigator->m_Mobile = false;
					m_State = ShootableStates::dying;
					break;
				}
				m_State = ShootableStates::alive;
				break;
			}
			case ShootableStates::dying:
			{
				m_CanShootAgain = false;
				m_TexIndex++;
				if (m_TexIndex >= 5)
				{
					m_TexIndex = 0;
				}
				m_timer = 0;

				int index = m_TexIndex + (7 * 5);
				if (index == (7 * 5) + 4)
				{
					m_State = ShootableStates::dead;
					m_SpawnWeapon = true;

				}
			}
			break;
			}
		}
	}
	//if (m_pNavigator->m_Mobile)
	//{
	//	if (m_pNavigator->m_hasLineOfSight &&
	//		!m_IsShooting &&
	//		m_CanShootAgain)
	//	{
	//		m_IsShooting = true;
	//
	//		m_timer = 0;
	//		m_TexIndex = 0;
	//	}
	//	m_timer += elapsSec;
	//	if (m_timer >= 0.5)
	//	{
	//		if (m_IsShooting)
	//		{
	//			if (m_TexIndex >= 1)
	//			{
	//				if (m_pNavigator->m_hasLineOfSight)
	//				{
	//					pPlayerHealth->Harm(5);
	//				}
	//				//ShootPlayer(&eyeObject);
	//				m_TexIndex = 0;
	//				m_IsShooting = false;
	//				m_CanShootAgain = false;
	//			}
	//			m_TexIndex++;
	//			m_timer = 0;
	//		}
	//		else if (m_pNavigator->m_Dying)
	//		{
	//			m_CanShootAgain = false;
	//			m_TexIndex++;
	//			if (m_TexIndex >= 5)
	//			{
	//				m_TexIndex = 0;
	//			}
	//			m_timer = 0;
	//
	//		}
	//		else {
	//			m_CanShootAgain = true;
	//			m_TexIndex++;
	//			if (m_TexIndex >= 4)
	//			{
	//				m_TexIndex = 0;
	//			}
	//			m_timer = 0;
	//		}
	//	}
	//	if (m_pHealthManager.GetHeath() <= 0 && !m_pNavigator->m_Dying)
	//	{
	//		m_timer = 0;
	//		m_pNavigator->m_Dying = true;
	//	}
	//	if (m_pNavigator->m_Dying)
	//	{
	//		int index = m_TexIndex + (7 * 5);
	//		if (index == (7 * 5) + 4)
	//		{
	//			m_pNavigator->m_Mobile = false;
	//
	//			m_SpawnWeapon = true;
	//
	//		}
	//	}
	//}
}

bool Enemy::Navigate( Level* pLevel, const Player& player, const float elapsSec)
{
	if (!m_IsShooting && m_pNavigator->m_Mobile) // if he is not shooting and alive
	{
		m_pNavigator->CalcNextPos(player.m_Pos, pLevel->GetWalls(), elapsSec);

		m_pNavigator->ColideWLevel(pLevel);
	
		m_pNavigator->PushPosition();
		return true;
	}
	else if (m_SpawnWeapon)
	{
		pLevel->SpawnWeapon(m_location);
		m_SpawnWeapon = false;
	}
	return false;
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
	switch (m_State)
	{
	case ShootableStates::alive:
		if (m_IsShooting)
		{
			index += (4 * 5);
		}
		break;
	case ShootableStates::harmed:
		index = (6 * 5);
		break;
	case ShootableStates::dying:
		index = m_TexIndex + (7 * 5);
		break;
	case ShootableStates::dead:
		index = (7 * 5) + 4;
		break;
	default:
		break;
	}
	
	/*if (m_pNavigator->m_IsShooting)
	{
		index += (4 * 5);
	}
	if (m_pNavigator->m_Dying)
	{
		index = m_TexIndex + (7 * 5);
	}
	if (!m_pNavigator->m_Mobile)
	{
		index = (7 * 5) + 4;
	}*/
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