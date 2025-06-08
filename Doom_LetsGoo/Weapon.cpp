#include "pch.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Texture.h"
#include <iostream>
#include "utils.h"

Weapon::Weapon(int startAmo, int maxAmo, float range, std::string soundFile)
	:m_MaxAmo{ maxAmo },
	m_Amo{startAmo},
	m_Range{range},
	m_GunShootSound{ soundFile }
{
}

Weapon::~Weapon()
{
	if (m_WeaponImages[0] != nullptr)
	{
		for (int i{}; i < s_WeaponTexAmount; i++)
		{
			delete m_WeaponImages[i];
			m_WeaponImages[i] = nullptr;
		}
	}
}

#pragma region shoot
void Weapon::Shoot(std::vector<Shootable*>& shootables, float windowWidth)
{
	//SOUND m_GunShootSound
	
	m_IsShooting = true;
	int closest{ -1 };
	float closestDist{ GetRange() };
	for (int index{}; index < shootables.size(); index++)
	{
		float xDiff = shootables[index]->GetLocationPoint().point2D.x - windowWidth / 2;
		if (closestDist > shootables[index]->GetLocationPoint().Dist3D
			&& abs(xDiff) < GetScreenRange())
		{
			closest = index;
			closestDist = shootables[index]->GetLocationPoint().Dist3D;
		}
	}

	if (closest >= 0)
	{
		Weapon::Shoot(shootables[closest]);
	}
}

#pragma region shoot helpers
void Weapon::Shoot(Shootable* object)
{
	if (m_Amo > 0)
	{
		object->BeShot(GetDamageOutput());

	}
}

int Weapon::GetDamageOutput() const
{
	return (rand() % 11) + 5;
}

float Weapon::GetRange() const
{
	return m_Range;
}

float Weapon::GetScreenRange() const
{
	return m_ScreenRange;
}

bool Weapon::GetIsShooting() const
{
	return m_IsShooting;
}


#pragma endregion shoot helpers
#pragma endregion shoot

#pragma region interactions
int Weapon::GetMaxAmo() const
{
    return m_MaxAmo;
}

int Weapon::GetAmo() const
{
    return m_Amo;
}

void Weapon::ChangeAmoBy(int amount)
{
	m_Amo += amount;
	if (m_Amo < 0)
	{
		m_Amo = 0;
	}
	else if (m_Amo > m_MaxAmo)
	{
		m_Amo = m_MaxAmo;
	}
}

void Weapon::IncreaseMaxAmo(int amount)
{
	m_MaxAmo += amount;
}

#pragma endregion interactions
