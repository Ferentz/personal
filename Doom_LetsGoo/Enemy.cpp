#include "pch.h"

#include "Enemy.h"
#include "Navigator.h"

#include "Player.h"
#include "Texture.h"
#include "Level.h"

#include <string>
#include <filesystem>

#include "SoundEffect.h"
#include "utils.h"

//TODO :: texture manager?
Enemy::Enemy(Vector2f& pos, float height, std::string& folder)
	:Shootable{ pos, height, folder, 20 },
	m_pNavigator{ new Navigator(m_location.point3D) }
{
	for (const auto& entry : std::filesystem::directory_iterator(folder))
	{
		std::string path{ entry.path().string() };
		m_MonsterTexturePaths.emplace_back(path);
	}
	LoadTextures();
	m_SubClass = ThingSubclass::enemy;
	m_PrevHealth = m_HealthManager.GetHealth();
	if (!s_EnemyAmount)
	{
		s_pRoarSound = new SoundEffect{"sounds/DSPOSIT2.wav"};
		s_pHurtSound = new SoundEffect{ "sounds/DSDMPAIN.wav" };
	}
	s_EnemyAmount++;
}

Enemy::~Enemy()
{
	delete m_pNavigator;
	m_pNavigator = nullptr;
	s_EnemyAmount--;
	if(!s_EnemyAmount)
	{
		delete s_pRoarSound;
		delete s_pHurtSound;
		s_pRoarSound = nullptr;
		s_pHurtSound = nullptr;
	}
}

#pragma region rule of 5

Enemy::Enemy(const Enemy& toCopy)
	:Shootable{ Shootable(toCopy) },
	m_pNavigator{ new Navigator(*toCopy.m_pNavigator) }
{
	m_timer = toCopy.m_timer;
	m_IsShooting = toCopy.m_IsShooting;
	m_CanShootAgain = toCopy.m_CanShootAgain;
	m_SpawnWeapon = toCopy.m_SpawnWeapon;

	m_TexIndex = toCopy.m_TexIndex;
	
	m_MonsterTexturePaths = toCopy.m_MonsterTexturePaths;
	LoadTextures();

	m_PrevHealth = toCopy.m_PrevHealth;
	m_HadLineOfSight = toCopy.m_HadLineOfSight;
}

Enemy::Enemy(Enemy&& toMove) noexcept
	:Shootable(std::move(Shootable( toMove )) ),
	m_pNavigator{ std::move(toMove.m_pNavigator) }
{
	MoveEnemyMembers(std::move(toMove));
}

Enemy& Enemy::operator=(Enemy&& rhs) noexcept
{
	if (&rhs != this)
	{
		Shootable::operator=(std::move(rhs));
		m_pNavigator = rhs.m_pNavigator;
		rhs.m_pNavigator = nullptr;
		MoveEnemyMembers(std::move(rhs));
	}
	return *this;
}

//private rule of 5 helper
void Enemy::MoveEnemyMembers(Enemy&& toMove)
{
	m_timer = std::move(toMove.m_timer);
	m_IsShooting = std::move(toMove.m_IsShooting);
	m_CanShootAgain = std::move(toMove.m_CanShootAgain);
	m_SpawnWeapon = std::move(toMove.m_SpawnWeapon);

	m_TexIndex = std::move(toMove.m_TexIndex);
	m_MonsterTexturePaths = std::move(toMove.m_MonsterTexturePaths);

	m_DeathRow = std::move(toMove.m_DeathRow);
	m_HurtRow = std::move(toMove.m_HurtRow);
	m_AttackRow = std::move(toMove.m_AttackRow);
	m_pMonsterTexture = std::move(toMove.m_pMonsterTexture);

	m_PrevHealth = std::move(toMove.m_PrevHealth);
	m_HadLineOfSight = std::move(toMove.m_HadLineOfSight);
}

#pragma endregion rule of 5

#pragma region Thing expansion
void Enemy::UpdateThing(const Player& player, float elapsSec)
{
	UpdateEnemy(elapsSec);
	Thing::UpdateThing(player, elapsSec);
}

void Enemy::DrawThing()
{
	int index{ m_TexIndex * m_TextureColums };
	switch (m_State)
	{
	case ShootableStates::alive:
		if (m_IsShooting)
		{
			index += (m_AttackRow * m_TextureColums);
		}
		break;
	case ShootableStates::harmed:
		index = (m_HurtRow * m_TextureColums);
		break;
	case ShootableStates::dying:
		index = m_TexIndex + (m_DeathRow * m_TextureColums);
		break;
	case ShootableStates::dead:
		index = (m_DeathRow * m_TextureColums) + m_TextureColums - 1;
		break;
	default:
		break;
	}

	float scale{ 0.001f };
	float width{ m_pMonsterTexture[index].GetWidth() * m_ScreenRatio / (m_location.Dist3D * scale) };
	float height{ m_pMonsterTexture[index].GetHeight() * m_ScreenRatio / (m_location.Dist3D * scale) };
	Rectf dest{ m_location.point2D.x - width / 2, m_location.point2D.y, width, height };
	m_pMonsterTexture[index].Draw(dest);
}
#pragma endregion Thing expansion

#pragma region enemy behaviour

void Enemy::UpdateEnemy(float elapsSec)
{
	if (m_pNavigator->m_hasLineOfSight != m_HadLineOfSight)
	{
		if (!m_HadLineOfSight )
		{
			//SOUND
			utils::PlaySoundwPriority(*s_pRoarSound, 2, false);
		}
		m_HadLineOfSight = m_pNavigator->m_hasLineOfSight;
	}
	if (m_HealthManager.GetHealth() < m_PrevHealth)
	{
		m_PrevHealth = m_HealthManager.GetHealth();
		//SOUND
		utils::PlaySoundwPriority(*s_pHurtSound, 1, false);
	}
	
	if (m_State != ShootableStates::dead)
	{
		m_timer += elapsSec;
		if (m_timer >= m_IntervalTime) // the time interval to swithc textures/perform actions
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
							s_pPlayerHealth->Harm(5);
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
				if (m_HealthManager.GetHealth() <= 0)
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
}

bool Enemy::Navigate( Level* pLevel, const Player& player, const float elapsSec)
{
	if (!m_IsShooting && m_pNavigator->m_Mobile) // if he is not shooting and alive
	{
		m_pNavigator->CalcNextPos(player.GetPos(), pLevel->GetWalls(), elapsSec);

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

Navigator* Enemy::GetpNavigator() const
{
	return m_pNavigator;
}

#pragma endregion enemy behaviour

#pragma region statics
void Enemy::AssignPlayerHealth(HealthManager& playerHealth)
{
	s_pPlayerHealth = &playerHealth;
}

HealthManager* Enemy::s_pPlayerHealth{};
#pragma endregion link to player health

void Enemy::LoadTextures()
{
	m_pMonsterTexture.reserve(m_MonsterTexturePaths.size());
	for (const std::string& path : m_MonsterTexturePaths)
	{
		m_pMonsterTexture.emplace_back(Texture(path));
	}
	int rows{ int(m_pMonsterTexture.size()) / m_TextureColums };
	m_DeathRow = rows - 1 - 1;
	m_HurtRow = m_DeathRow - 1;
	m_AttackRow = m_HurtRow - 2;
}

SoundEffect* Enemy::s_pRoarSound{nullptr};
SoundEffect* Enemy::s_pHurtSound{nullptr};
int Enemy::s_EnemyAmount{0};
